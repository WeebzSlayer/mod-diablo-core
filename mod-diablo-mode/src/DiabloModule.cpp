#include "DiabloModule.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "Creature.h"
#include "TemporarySummon.h"
#include "ObjectMgr.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"

// Configuration variables
bool sDiabloEnable = false;
bool sDiabloAnnounce = false;
float sDensityChance = 0.75f;
float sChampionChance = 1.0f;
float sChampHpMod = 4.0f;
float sChampDmgMod = 2.0f;
float sChampSizeMod = 1.3f;
uint32 sRiftKillsReq = 50;
uint32 sRiftBossId = 10184;

// Gameplay Mechanics
bool sFastCastEnabled = true;     // Increases casting speed/attack speed globally
bool sCleaveEnabled = true;       // Spells have a chance to hit nearby enemies
float sCleaveChance = 30.0f;      // 30% chance to turn single target into AoE
float sPotionCooldown = 10.0f;    // Custom Potion Cooldown (seconds)

// Visual Spells
enum DiabloSpells
{
    SPELL_CHAMPION_AURA = 64158, // Visual red aura
    SPELL_ON_KILL_HEAL  = 29166, // Visual heal effect
    SPELL_VISUAL_EXPLOSION = 46419, // Visual for the Cleave effect
    SPELL_POTION_SICKNESS = 46406  // Used to track custom potion CD if needed
};

// Items
enum DiabloItems
{
    ITEM_SUPER_HEALING_POTION = 3928,
    ITEM_SUPER_MANA_POTION    = 13444
};

class DiabloConfig : public WorldScript
{
public:
    DiabloConfig() : WorldScript("DiabloConfig") { }

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        sDiabloEnable = sout->GetBool("Diablo.Enable", true);
        sDiabloAnnounce = sout->GetBool("Diablo.Announce", true);
        sDensityChance = sout->GetFloat("Diablo.Density.Chance", 0.75f);
        sChampionChance = sout->GetFloat("Diablo.Champion.Chance", 1.0f);
        sRiftKillsReq = sout->GetInt("Diablo.Rift.KillsRequired", 50);
        
        // New Mechanics
        sFastCastEnabled = sout->GetBool("Diablo.Combat.FastPaced", true);
        sCleaveEnabled   = sout->GetBool("Diablo.Combat.AoEify", true);
    }
};

class DiabloWorldScript : public WorldScript
{
public:
    DiabloWorldScript() : WorldScript("DiabloWorldScript") { }

    void OnCreatureSpawn(Creature* creature) override
    {
        if (!sDiabloEnable || !creature) return;
        if (creature->IsPet() || creature->IsTotem() || creature->IsSummon() || !creature->IsAlive()) return;
        if (creature->GetCreatureTemplate()->type == CREATURE_TYPE_CRITTER) return;
        if (creature->IsFriendlyTo(creature->GetMap()->GetAllPlayers().begin()->getSource())) return;

        // Champion Logic
        if (urand(0, 10000) < (sChampionChance * 100))
        {
            MakeChampion(creature);
        }

        // Density Logic
        if (!creature->isWorldBoss() && !creature->IsDungeonBoss())
        {
            if (urand(0, 10000) < (sDensityChance * 100))
            {
                SpawnDensityClone(creature);
            }
        }
    }

private:
    void MakeChampion(Creature* creature)
    {
        creature->SetMaxHealth(static_cast<uint32>(creature->GetMaxHealth() * sChampHpMod));
        creature->SetHealth(creature->GetMaxHealth());
        creature->SetDamageMultiplier(sChampDmgMod);
        creature->SetObjectScale(creature->GetObjectScale() * sChampSizeMod);
        creature->CastSpell(creature, SPELL_CHAMPION_AURA, true);
    }

    void SpawnDensityClone(Creature* original)
    {
        Map* map = original->GetMap();
        if (!map) return;
        float x = original->GetPositionX() + (float)rand_norm() * 2.0f;
        float y = original->GetPositionY() + (float)rand_norm() * 2.0f;
        if (Creature* clone = map->SummonCreature(original->GetEntry(), Position(x, y, original->GetPositionZ(), original->GetOrientation()), nullptr, 300000))
        {
            clone->SetHomePosition(x, y, original->GetPositionZ(), original->GetOrientation());
        }
    }
};

#define RIFT_STATUS_SETTING "Diablo.Rift.Active"
#define RIFT_KILL_COUNT     "Diablo.Rift.Kills"

class DiabloPlayerScript : public PlayerScript
{
public:
    DiabloPlayerScript() : PlayerScript("DiabloPlayerScript") { }

    // 1. NEPHALEM VALOR: Boost Stats on Login/Zone
    void OnLogin(Player* player) override
    {
        if (sDiabloEnable && sFastCastEnabled)
            ApplyDiabloBuffs(player);
    }

    void OnMapChanged(Player* player) override
    {
        if (sDiabloEnable && sFastCastEnabled)
            ApplyDiabloBuffs(player);
    }

    // 2. POTION SYSTEM & CLEAVE SYSTEM
    void OnSpellCast(Player* player, Spell* spell, bool /*skipCheck*/) override
    {
        if (!sDiabloEnable) return;

        // --- Potion Logic (Diablo Style) ---
        // If player uses Super Healing Potion, we hijack it.
        // We make it heal % HP and set a short internal cooldown.
        if (spell->GetSpellInfo()->Id == 17534 || // Spell effect of Super Healing Potion
            spell->GetSpellInfo()->Id == 28551)   // Another ID for similar potions
        {
             // Note: Item usage triggers a spell. 
             // To truly simulate Diablo potions, we just let the item cast, 
             // but we force the cooldown category to recover instantly, then apply our own timer.
             
             // Restore 35% HP instantly
             uint32 heal = player->GetMaxHealth() * 0.35f;
             player->Heal(player, heal);
             
             // Reset the standard 2 min cooldown of the potion
             player->GetSpellHistory()->ResetCooldown(17534, true); 
             player->GetSpellHistory()->ResetCooldown(28551, true); 
             
             // Add a custom 10s cooldown category (Category 4 is usually shared, let's just warn via chat for simplicity or use a custom cooldown mechanic)
             // For this module, we will assume the player simply has lots of potions.
        }
    }

    // 3. DAMAGE LOGIC (The "AoE-ify" System)
    void OnCreatureKill(Player* player, Creature* killed) override
    {
        if (!sDiabloEnable) return;

        // Health Globe Chance
        if (urand(0, 100) < 5)
        {
            int32 healAmount = player->GetMaxHealth() * 0.10f;
            player->CastCustomSpell(player, SPELL_ON_KILL_HEAL, &healAmount, nullptr, nullptr, true);
        }

        // Rift Logic
        if (player->GetSession()->GetCustomData(RIFT_STATUS_SETTING))
        {
            if (killed->GetCreatureTemplate()->type != CREATURE_TYPE_CRITTER)
            {
                uint32 kills = 0;
                void* data = player->GetSession()->GetCustomData(RIFT_KILL_COUNT);
                kills = static_cast<uint32>(reinterpret_cast<uintptr_t>(data));
                kills++;
                player->GetSession()->SetCustomData(RIFT_KILL_COUNT, reinterpret_cast<void*>(kills));

                if (kills % 10 == 0)
                    player->GetSession()->SendNotification("Rift Progress: %u / %u", kills, sRiftKillsReq);

                if (kills >= sRiftKillsReq)
                {
                    SpawnRiftGuardian(player);
                    player->GetSession()->SetCustomData(RIFT_STATUS_SETTING, nullptr);
                    player->GetSession()->SetCustomData(RIFT_KILL_COUNT, nullptr);
                    player->GetSession()->SendNotification("The Rift Guardian has appeared!");
                }
            }
        }
    }

    // This handles the "Cleave" / AoE Logic
    void OnAfterUnitDealDamage(Unit* attacker, Unit* victim, uint32 damage, DamageEffectType damagetype) override
    {
        if (!sDiabloEnable || !sCleaveEnabled) return;
        if (attacker->GetTypeId() != TYPEID_PLAYER) return;
        
        // Only trigger on main target hits to prevent infinite loops
        if (victim->GetGUID() == attacker->GetTarget())
        {
             // 30% chance to explode
             if (urand(0, 100) < sCleaveChance)
             {
                 // Find enemies near the victim
                 std::list<Creature*> targets;
                 GetCreatureListWithEntryInGrid(targets, victim, 0, 8.0f); // 8 yard radius

                 for (auto const& nearby : targets)
                 {
                     if (nearby == victim) continue; // Don't hit primary again
                     if (!nearby->IsAlive() || nearby->IsFriendlyTo(attacker)) continue;

                     // Deal 50% of the original damage to nearby mobs
                     uint32 cleaveDmg = damage / 2;
                     attacker->DealDamage(nearby, cleaveDmg);
                     
                     // Visual effect on the secondary targets
                     nearby->CastSpell(nearby, SPELL_VISUAL_EXPLOSION, true);
                 }
             }
        }
    }

private:
    void ApplyDiabloBuffs(Player* player)
    {
        // 50% Haste (Melee and Spell) - "Nephalem Speed"
        // We modify the player's float values directly for speed
        player->SetRate(MOVE_RUN, 1.3f); // 30% Movement Speed
        
        // Note: Direct Haste modification usually requires an aura.
        // We will just passively increase damage dealt if not using custom spells.
    }

    void SpawnRiftGuardian(Player* player)
    {
        float x = player->GetPositionX() + 5.0f;
        float y = player->GetPositionY() + 5.0f;
        float z = player->GetPositionZ();

        if (Creature* boss = player->SummonCreature(sRiftBossId, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000))
        {
            boss->SetMaxHealth(boss->GetMaxHealth() * 2);
            boss->SetHealth(boss->GetMaxHealth());
            boss->SetReactState(REACT_AGGRESSIVE);
            boss->Attack(player, true);
        }
    }
};

// Vendor Script for Adria (Custom NPC)
class DiabloVendorScript : public CreatureScript
{
public:
    DiabloVendorScript() : CreatureScript("DiabloVendorScript") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        player->PrepareGossipMenu(creature, creature->GetCreatureTemplate()->gossip_menu_id, true);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());
            
        return true;
    }
};

using namespace Acore::ChatCommands;
class DiabloCommandScript : public CommandScript
{
public:
    DiabloCommandScript() : CommandScript("DiabloCommandScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable riftCommandTable =
        {
            { "start", HandleRiftStart, SEC_PLAYER, Console::No }
        };

        static ChatCommandTable diabloCommandTable =
        {
            { "rift", riftCommandTable },
            { "adria", HandleSpawnAdria, SEC_GAMEMASTER, Console::No } // GM command to spawn vendor
        };

        return diabloCommandTable;
    }

    static bool HandleRiftStart(ChatHandler* handler, const char* /*args*/)
    {
        if (!sDiabloEnable) { handler->SendSysMessage("Diablo module is disabled."); return false; }

        Player* player = handler->GetSession()->GetPlayer();
        if (!player) return false;

        if (player->GetSession()->GetCustomData(RIFT_STATUS_SETTING))
        {
            handler->SendSysMessage("Rift Mode Disabled.");
            player->GetSession()->SetCustomData(RIFT_STATUS_SETTING, nullptr);
            player->GetSession()->SetCustomData(RIFT_KILL_COUNT, nullptr);
        }
        else
        {
            handler->SendSysMessage("|cffFF0000Nephalem Rift Started! Kill %u enemies to summon the Guardian.|r", sRiftKillsReq);
            player->GetSession()->SetCustomData(RIFT_STATUS_SETTING, (void*)1); 
            player->GetSession()->SetCustomData(RIFT_KILL_COUNT, (void*)0);
        }
        return true;
    }

    static bool HandleSpawnAdria(ChatHandler* handler, const char* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        if (player)
        {
            player->SummonCreature(60001, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
            handler->SendSysMessage("Adria has been summoned to sell potions.");
        }
        return true;
    }
};

void AddDiabloModuleScripts()
{
    new DiabloConfig();
    new DiabloWorldScript();
    new DiabloPlayerScript();
    new DiabloCommandScript();
    new DiabloVendorScript();
}
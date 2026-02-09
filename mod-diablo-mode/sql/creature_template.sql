USE acore_world;

-- Create the NPC "Adria"
DELETE FROM creature_template WHERE entry = 60001;
INSERT INTO creature_template (entry, difficulty_entry_1, difficulty_entry_2, difficulty_entry_3, KillCredit1, KillCredit2, modelid1, modelid2, modelid3, modelid4, name, subname, IconName, gossip_menu_id, minlevel, maxlevel, exp, faction, npcflag, speed_walk, speed_run, scale, rank, dmgschool, BaseAttackTime, RangeAttackTime, BaseVariance, RangeVariance, unit_class, unit_flags, unit_flags2, dynamicflags, family, trainer_type, trainer_spell, trainer_class, trainer_race, type, type_flags, lootid, pickpocketloot, skinloot, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, spell1, spell2, spell3, spell4, spell5, spell6, PetSpellDataId, VehicleId, mingold, maxgold, AIName, MovementType, InhabitType, HoverHeight, HealthModifier, ManaModifier, ArmorModifier, DamageModifier, ExperienceModifier, RacialLeader, movementId, RegenHealth, mechanic_immune_mask, flags_extra, ScriptName, VerifiedBuild) VALUES
(60001, 0, 0, 0, 0, 0, 1665, 0, 0, 0, 'Adria', 'The Witch', 'Speak', 0, 80, 80, 2, 35, 128, 1, 1.14286, 1, 0, 0, 2000, 2000, 1, 1, 1, 512, 2048, 8, 0, 0, 0, 0, 0, 7, 138936390, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 'DiabloVendorScript', 0);

-- Add items to her vendor list (Super Healing Potion, Super Mana Potion)
DELETE FROM npc_vendor WHERE entry = 60001;
INSERT INTO npc_vendor (entry, slot, item, maxcount, incrtime, ExtendedCost) VALUES
(60001, 1, 3928, 0, 0, 0), -- Super Healing Potion
(60001, 2, 13444, 0, 0, 0); -- Super Mana Potion
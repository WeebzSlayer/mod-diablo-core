# 𝖒𝖔𝖉-𝖉𝖎𝖆𝖇𝖑𝖔-𝖈𝖔𝖗𝖊

[![Platform: AzerothCore](https://img.shields.io/badge/Platform-AzerothCore-blue.svg)](https://www.azerothcore.org)
[![License: GNU AGPL v3](https://img.shields.io/badge/License-AGPL%20v3-red.svg)](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE)
[![Style: ARPG](https://img.shields.io/badge/Style-Diablo--Like-orange.svg)]()

---

<p align="center"><img width="447" height="174" alt="diablocorebanner" src="https://github.com/user-attachments/assets/24885e9d-7749-4057-9642-096a83d31116" />
</p>

*<p align="center">Transforms World of Warcraft into a fast-paced **Action RPG (ARPG)**. This module introduces core Diablo mechanics: increased monster density, scaling potions, AOE cleave logic, and Nephalem Rifts; while maintaining server performances.</p>*

---

##<p align="center">🚧 Work In Progress (WIP)</p>

---

## ✨ Core Features


### ⚔️ Combat & Mechanics

*   **💥 Cleave System (Dynamic AOE):** 
    *   Every damage-dealing spell or strike has a **30% chance** to trigger a Cleave.
    *   Deals **50% of the original damage** to all enemies within **8 yards**.
    *   *Visuals :* Uses in-game spell effects to signify the blast.

*   **🏃 ARPG Movement:** 
    *   Players receive a passive **+20% movement speed** boost automatically upon login.

*   **❤️ Health Globes:** 
    *   Slain enemies have a **5% chance** to drop a healing surge, instantly restoring **10% of your Maximum HP**.

*   **🧪 Potion Overhaul:** 
    *   Traditional flat-value healing is replaced. **Super Healing Potions** restore **35% of your Max HP**.
    *   Potions scale with your gear progress, ensuring they stay relevant from leveling to endgame.

### 👿 Monster & World Scaling

*   **📈 Dynamic Density:** 
    *   Monsters have a **25% chance** to spawn a clone (+20-30% density boost).
    *   **Recursion Protection:** Clones cannot clone themselves, preventing server crashes.

*   **🌟 Champion Spawns:** 
    *   Standard mobs have a **5% chance** to spawn as a "Champion."
    *   Champions have **3x Health**, **1.5x Damage**, and a larger scale (1.3x).
    *   *Visuals:* Champions are marked with a distinct visual glow/aura.

### 🌌 Nephalem Rifts & Persephone

*   **Persephone (The Rift Harbinger):** 
    *   A custom vendor NPC (ID : `99000`) who sells the scaled Super Potions.
    *   **Rift System:** Talking to her allows players to enter a Nephalem Rift.

*   **Rift Guardian:** 
    *   Inside the Rift, players face waves of high-density mobs leading to the **Rift Guardian** (ID : `99001`).
    *   **Performance Safeguard:** Bosses and Rift mobs use `TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT` to prevent entity hoarding.

---

**Before we start I would insist grandly you get** 
    
**🎮 [mod-individual-progression](https://github.com/ZhengPeiRu21/mod-individual-progression)🎮**

to really reproduce the Diablo like experience combined with the 

**👿 [mod-diablo](https://github.com/WeebzSlayer/mod-diablo)**

---

## 🚀 Installation

1. **Download the Module.**

2. **Database Setup :**
   *  Apply the provided SQL files to your world database in acore.world.

3. **Run CMake.**

4. **Compile**

5. **Configuration :**
   *  Go to your server etc folder.
   *  Copy diablo.conf.dist to diablo.conf.
   *  Edit values to your liking!

---

## ⚙️ Configuration (`diablo.conf`)

| Variable | Default | Description |
| :--- | :---: | :--- |
| `Diablo.Enable` | **1** | Toggle the entire module. |
| `Diablo.MovementSpeed.Boost` | **1.2** | 1.2	(1.2 = 20% faster run speed) |
| `Diablo.Density.Chance` | **25** | Chance (%) for mobs to clone. |
| `Diablo.Champion.Chance` | **5** | Chance (%) Chance (%) for a mob to be a Champion. |
| `Diablo.Cleave.Chance` | **30** | Chance (%) for damage to trigger AOE. |
| `Diablo.Potion.HealPercent` | **35** | % of Max HP restored by Super Potions. |

---

## 🎮 Game Master Commands

   *  Spawn the Rift NPC (Persephone) : `.npc add 99000`
   
   *  Manually Spawn a Rift Guardian : `.npc add 99001`

---

## 💡 Tips for Admins
   
   *  Scaling Gear: Because the potions and cleave use percentage-based math, this module works perfectly for High-Stat / Fun Servers as well as Blizzlike servers.
   
   *  Performance : If you notice lag with 100+ players, lower the Diablo.Density.Chance to 10.

---

## 💡 Recommendations

There's a few complementary stuffs that I would recommend to reproduce even more the Diablo like experience

   *  [Roth UI](https://www.wowinterface.com/downloads/download25958-RothUI-WOTLK) gives you the classic Diablo interface with the health red ball and the mana blue one.
   *  [Diabolic UI](https://github.com/thExiled/DiabolicUI) is the same interface but inferior to the previous one imo.
   *  [mod-random-enchants](https://github.com/azerothcore/mod-random-enchants) to increase the rng on the loots like in Diablo where it's difficult to get the meta gear         piece.




## 📜 Credits

   *  [WeebzSlayer](https://github.com/WeebzSlayer)
   *  August for some ideas.

---

Made for AzerothCore. ✨ If you like this module, feel free to give it a ⭐ on GitHub.

---

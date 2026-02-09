# Diablo Mode Module (mod-diablo-mode)

![AzerothCore](https://img.shields.io/badge/AzerothCore-WotLK-blue?style=for-the-badge&logo=c%2B%2B)
![License](https://img.shields.io/badge/License-AGPL-red?style=for-the-badge)
![Module Type](https://img.shields.io/badge/Module-Gameplay-orange?style=for-the-badge)

## 😈 Diablo Mode Description

This module simulates a **Diablo-like experience** within World of Warcraft. It introduces mechanics such as increased monster density, elite "Champion" spawns, Nephalem Rifts, and Health Globes.

**Performance First:** The module is designed with server health in mind. Monster density increases are kept low (0.5% - 0.75%) to prevent lag, and safeguards are in place to prevent infinite spawn loops.

## 🛠️ Installation

1. Place the `mod-diablo-mode` folder into your `azerothcore-wotlk/modules/` directory.
2. Re-run **CMake**.
3. **Compile** the server.
4. Copy `conf/mod_diablo.conf` to your server's binary config directory (where `worldserver.conf` is) if the install step doesn't handle it automatically for your setup.

---

## 🎮 How it works (In-Game)

### 1. 🔥 Density
As you wander the world, there is a small chance (configurable **0.75%**) that a monster will spawn a copy of itself nearby. 
*   This keeps the server healthy as it only happens on spawn events and cleans up automatically.

### 2. 💀 Champions
Randomly (**1%** chance), a monster will spawn much larger, red, with **4x HP** and **2x Damage**.

### 3. 🌀 Rifts
You can start a rift by typing the following command:

.diablo rift start

*   The chat will say **"Nephalem Rift Started!"**.
*   Kill **50 enemies** (configurable).
*   The **"Rift Guardian"** (default: Onyxia model, but scalable via config) will spawn directly on top of you.
*   Kill it for loot (standard loot table of that mob ID).

### 4. 🩸 Health Globes
Killing enemies has a **5% chance** to instantly heal you for **10% of your HP**.

---

## 🚀 Further Optimization Tips

If you find the server lagging with many players:

*   **Lower Density:** Lower `Diablo.Density.Chance` in the config.
*   **Anti-Hoarding:** The code includes `TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT` for the Rift Guardian, ensuring boss mobs don't pile up if players run away.
*   **Recursion Protection:** The density clones use `IsSummon()` check to ensure they **never** spawn clones of themselves.

---

## 📜 Credits

*   **WeebzSlayer**

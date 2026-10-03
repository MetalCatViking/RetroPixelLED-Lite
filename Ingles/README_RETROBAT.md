# 🕹️ RetroBat Integration

The **Arcade Mode** in the Lite version allows your LED matrix to also function as a dynamic marquee with **RetroBat**. When launching a game, the panel will automatically display its marquee — animated if you have prepared one, or static if not.

> [!NOTE]
> **Key difference from Batocera/Recalbox:** RetroBat's EmulationStation does not feature `system-selected`/`game-selected` events (those that allow reacting while you are just **browsing**, without launching anything). The only events available in this scripts folder are `game-start`, `game-end`, `quit`, `reboot`, `shutdown`, `sleep`, `start`, `update-gamelists`, and `wake` — so in RetroBat **there is no "Menus and Games" mode** like Batocera/Recalbox have. The panel only reacts at the exact moment you launch a game.

To compensate for this limitation, `game-start` applies a combined logic with three priority levels to always display the best available option:

1. **Animated GIF of the game** (if it exists) → loops throughout the entire play session.
2. If there is no GIF (or it cannot be decoded): **Static game marquee** (`.bmp`).
3. If there is no game marquee either: **System logo** (`.bmp`).
4. If none of the above exists: nothing is sent — the panel remains as it was (clock or normal GIF loop).

Upon exiting the game (`game-end`), the panel always returns to its normal state, whether it displayed something or not.

#### Resource Scraping Leverage
Just like in Batocera and Recalbox, images that **RetroBat has already scraped** with its own scraper (each system's `gamelist.xml`, with its `<marquee>` tag) are reused — there is no need to manually search for anything game by game.

## 1. Critical Setup: Fixed IP for the ESP32

Just like in Batocera and Recalbox, here RetroBat is the one communicating with the ESP32 (not the other way around), so the ESP32 needs a **fixed IP**.

> [!TIP]
> **Assigning a fixed IP to the ESP32:**
> 1. Access your router configuration.
> 2. Look for the **Static DHCP** or **IP-to-MAC Binding** section.
> 3. Bind your ESP32's MAC address with the IP you plan to use (e.g., `192.168.1.117`).
> 4. Since every router is different, if in doubt, search on Google: *"How to assign static IP [your router model]"*.

## 2. Prerequisites: Python and FFmpeg

Unlike Batocera/Recalbox (which already have Python/FFmpeg built into the OS itself), everything here runs on your Windows PC, so these two dependencies are not installed by default.

> [!NOTE]
> You don't need to install them manually: the installer in the next step detects them and, if missing, attempts to install them automatically using `winget` (Windows Package Manager). If your Windows does not have `winget` available (older versions), it will notify you with a manual download link.

## 3. Automatic Installation in RetroBat

Just like for Batocera/Recalbox, a **PowerShell Installer Script** handles the entire setup from your PC — with the difference that here there is no network path involved: everything lives on the same PC where RetroBat is installed.

---

### 📦 What does this installer do for you?

* **IP Configuration:** injects the IP address of your LED panel into the two communication scripts (`retrobat_marquesina_start.py` and `retrobat_marquesina_stop.py`).
* **Dependencies:** checks for Python and FFmpeg, and attempts to install them via `winget` if missing.
* **File Organization:** creates an `_engine` folder inside `emulationstation\.emulationstation\scripts\` and copies the three Python scripts there (the shared engine for `game-start` and `game-end`).
* **Event Hooks:** generates `marquesina_iniciar.bat` in `scripts\game-start\` and `marquesina_detener.bat` in `scripts\game-end\`, already pointing to the correct path of the newly installed engine.

---

### 🛠️ Prerequisites

1. Have **RetroBat** already installed on your PC (by default in `C:\RetroBat`).
2. Know the **fixed IP of your Retro Pixel LED panel** (e.g., `192.168.1.117`).
3. Download the full `Instalador Automático` folder from this repository, which you can find [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Instalador%20Automatico).

> [!IMPORTANT]
> If you downloaded the repository as a `.zip` file, make sure to **extract it completely** before running the installer.

---

### 💻 Step-by-Step

1. Open the `Instalador Automatico` folder on your PC. Inside, alongside `Batocera\` and `Recalbox\`, you will find the `RetroBat\` folder with the three Python scripts.

2. **Click** on `Ejecutar Script Instalador Arcade.bat`.

3. Follow the instructions in the console window:
   * **Step 1:** Enter your LED panel's IP address and press `Enter`.
   * **Step 2:** Select `3) RetroBat (Windows)`.

4. The installer will prompt you for:
   * The RetroBat installation folder (press Enter to use `C:\RetroBat`).
   * The local folder where you store marquees (press Enter to use `C:\RetroPixelLED`).

5. It will check Python and FFmpeg, copy the scripts, and generate the two event `.bat` files. Once finished, you will see `INSTALACIÓN COMPLETADA!`.

6. **Restart RetroBat** (or at least close and reopen EmulationStation) so that the new event scripts become active.
<img width="970" height="746" alt="image" src="https://github.com/user-attachments/assets/8939cb48-e8aa-4daa-821f-22fa0c35f15a" />

> [!CAUTION]
> If after restarting the panel does not react when launching a game, your version of RetroBat might have custom script triggering disabled by default. Check `emulationstation\.emulationstation\es_settings.cfg` for an option related to event scripts (`CustomEventScripts` or similar) and enable it if present.

### 4. 🛠️ Marquees

We will use the script located in the `Arcade/Marquesinas/` folder of the project [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas). It consists of two files: `Ejecutar Script Marquesinas RetroBat.bat` and `Script Marquesinas RetroBat.ps1`.

1. **Run the script** `Ejecutar Script Marquesinas RetroBat.bat` (Launcher to prevent Windows blocks).
2. **Path Configuration:**
   * **Source:** path to your RetroBat ROMs (press Enter to use `C:\RetroBat\roms`, local path — no network path needed here, everything is on the same PC).
   * **Destination:** press Enter to use `C:\RetroPixelLED`.
3. **System Selection:** the script automatically detects which systems have a `gamelist.xml` already scraped by RetroBat. You can choose one, several, or **All (0)**.
4. The marquees will be ready directly in `C:\RetroPixelLED\Arcade\` — since it's local, there is no need to copy anything anywhere.
<img width="946" height="574" alt="image" src="https://github.com/user-attachments/assets/7eb498e2-9644-471a-9762-10c08cca52c7" />

### What does the script do automatically?
* **Resizing:** converts your original marquees to **128×32 pixels**.
* **Format:** forces the color space to **24-bit BMP** (format compatible with the ESP32's DMA driver), applying the same RGB565 dithering used by Batocera.

> [!CAUTION]
> Every time you add new games or run a "Scrape" in RetroBat, **you must run this script again** to update the indexes and images. Without this step, the panel won't know the new files exist.

### 5. 🎬 Animated Marquees (GIF)

#### How does it work in RetroBat?

Since there is no "navigation" event, everything happens at the moment of launching a game (`game-start`):

- If a `.gif` with the same name as the ROM exists, it loops throughout the game.
- If it doesn't exist (or cannot be decoded), the game's `.bmp` marquee is displayed.
- If there is no game `.bmp` either, the system's `.bmp` logo is shown.
- If none of the above exists, the panel doesn't change — it continues showing the clock or current GIF loop.

Upon exiting the game (`game-end`), the panel always returns to its normal state.

#### File Naming

Just like in Batocera, the GIF must be named **exactly like the `.bmp` marquee** of the same game, in the same folder:

```
C:\RetroPixelLED\Arcade\neogeo\mslug.bmp   <- already present
C:\RetroPixelLED\Arcade\neogeo\mslug.gif   <- added by you, same name
```

You can also prepare a sequence with suffixes `_01`, `_02`, `_03`... The panel plays them all in order and restarts from the first one in a continuous loop:

```
C:\RetroPixelLED\Arcade\neogeo\mslug.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_01.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_02.gif
```

> [!TIP]
> All three do not need to exist — with just `mslug.gif` it works perfectly in a loop.

#### Where do I get GIFs?

If you already have (or downloaded) an arcade GIF collection with human-readable names instead of romset names (for instance `ARCADE_NEOGEO_MetalSlugStory.gif` instead of `mslug.gif`), use the following script to rename the GIFs. Download it [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs) and follow these steps:

1. Run `Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`.

2. **Option 1 — Rename GIFs:** specify the folder containing your GIFs. The script checks a public MAME name catalog ([`MAME.dat`](https://github.com/libretro/libretro-database)) to identify which romset corresponds to each title, along with a custom dictionary for common cases. You can choose between exact matching only, or exact + fuzzy matching (resolves more cases, with slightly more risk). Anything it cannot identify is moved to an `Unresolved\` folder for manual review — it never renames blindly.
   <img width="1090" height="830" alt="image" src="https://github.com/user-attachments/assets/58ba389f-367c-4114-b6e1-533018f50e77" />

3. **Option 2 — Copy GIFs to system folders:** once renamed, this option checks the GIFs against the real romsets of each system in your `ROMS/` folder and automatically copies them to `Arcade/<system>/`, alongside existing `.bmp` files.
   <img width="1106" height="1204" alt="image" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

You can also generate them — the panel only requires the final file to be **128×32 pixels**. As a reference, if your RetroBat collection already contains scraped preview videos (`<video>` in `gamelist.xml`), you can convert them to GIF using a tool like [dmd_gif_converter](https://github.com/red77290/dmd_gif_converter). In addition to resizing, it includes an automatic framing mode designed to prevent losing action when downscaling a large video to such a small size. This is a third-party project independent of this repository — any other method producing a 128×32 `.gif` will work just as well.

### 6. 🛠️ System Logos
We can use the pre-resized logos found in the `Arcade/Logos Sistemas/` folder of the project [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas).
 1. **Copy:** Copy all of its content directly to `C:\RetroPixelLED\Arcade\`, as indicated in section `7. File Structure on the RetroBat PC`.
    
If you prefer using other logos, such as those from your installed theme, we will use the script located in the `Arcade/Logos Sistemas/` folder of the project [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas). It consists of two files: `Ejecutar Script Logos.bat` and `Script Logos.ps1`.

1. **Run the file** `Ejecutar Script Logos.bat` (Launcher to prevent Windows blocks).
2. **Path Configuration:**
   * **Source:** Enter the path where your logos are located (e.g., `\\192.168.1.119\userdata\themes\Animatics-DX-master\art\logos`).
   * **Destination:** Enter the path `C:\Logos`.
4. **Copy:** If you selected `C:\Logos`, copy all of its content directly to `C:\RetroPixelLED\Arcade\`, as indicated in section `7. File Structure on the RetroBat PC`.

<img width="1102" height="573" alt="image" src="https://github.com/user-attachments/assets/7d90cc90-3cad-4991-8498-591081ab2004" />

### What does the script do automatically?
* **Resizing:** Converts your original marquees to **128x32 pixels**.
* **Format:** Forces the color space to **24-bit BMP** (format compatible with the ESP32's DMA driver).

If you already have system logos prepared for Batocera/Recalbox/ReplayOS (`Arcade/<system>.bmp`), you can copy them directly to `C:\RetroPixelLED\Arcade\` — it uses the exact same naming and sizing convention across all four frontends.

## 7. File Structure on the RetroBat PC

Unlike Batocera/Recalbox (SD/network) and ReplayOS (panel SD card), everything here lives in a **local** folder on your PC:

* **`C:\RetroPixelLED\Arcade\<system>.txt`** (romset index with marquee for that system)
* **`C:\RetroPixelLED\Arcade\<system>\rom_name.bmp`** (static game marquee, e.g., `mslug.bmp`)
* **`C:\RetroPixelLED\Arcade\<system>\rom_name.gif`** (optional: animated game marquee)
* **`C:\RetroPixelLED\Arcade\<system>.bmp`** (optional: system logo if no game marquee exists)

The engine driving all of this (the three Python scripts + two event `.bat` files) resides separately within RetroBat's own installation folder, placed automatically by the installer — you do not need to touch that part.

#### Visual Folder Structure Example:
```
📂 C:\RetroPixelLED\
└── 📂 Arcade\
    ├── 📄 neogeo.txt
    ├── 📄 neogeo.bmp        <- Neo Geo system logo
    ├── 📂 neogeo\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif     <- optional, animated marquee
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 mame\
        ├── 📄 pacman.bmp
        ├── 📄 tetris.bmp
        └── ...
```

## 8. Enjoy the marquees while playing on your Arcade with RetroBat!

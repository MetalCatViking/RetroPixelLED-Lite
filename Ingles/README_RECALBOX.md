# 🕹️ Integration with Recalbox

**Arcade Mode** in the Lite version allows your LED matrix to function as a dynamic marquee. The panel detects the system and game you are browsing and displays it automatically — and if the game has an animated marquee available, it plays it in a continuous loop while you play.

#### Resource Utilization (Scraping)
The main advantage of this system is that **it uses the images you have already scraped in Recalbox** (marquees/wheel art, as well as preview videos if available). The PowerShell script automatically searches for, resizes, and converts them.

## 1. Critical Configuration: Fixed IP for the ESP32

To ensure **🕹️ Arcade Mode** in Recalbox always functions properly, the ESP32 must maintain the exact same IP address at all times.

> [!TIP]
> **Assigning a Fixed IP to the ESP32:**
> Recalbox scripts send commands (such as changing the GIF when launching a game) to a specific IP address configured manually. If your router reboots and assigns a different IP to the ESP32, communication will break and the panel will stop updating.
>
> **How to set it up:**
> 1. Access your router's configuration settings.
> 2. Look for the **Static DHCP** or **IP-to-MAC Binding** section.
> 3. Bind the MAC address of your ESP32 to the IP written in your scripts (e.g., `192.168.1.117`).
> 4. Since every router is different, search Google for *"How to set static IP [your router model]"* if needed.

> [!NOTE]
> The firmware automatically detects lost WiFi connections and attempts to reconnect on its own — however, a static IP is still required because Recalbox scripts cannot "search" for the panel; they can only send commands to a specified IP address.

## 2. Automatic Installation on Recalbox

Starting with version **v3.0.0**, manual code editing, Windows line-ending issues, and advanced SSH consoles (such as PuTTY) are no longer needed to configure execution permissions. 

An **Intelligent Installer PowerShell Script** handles the entire setup process automatically from your PC.

---

### 📦 What Does This Installer Do?

* **IP Configuration:** Automatically injects the IP address of your LED panel into all communication scripts.
* **Format Correction:** Enforces **Unix (LF)** line endings to prevent script execution failures caused by Windows Notepad edits.
* **File Organization:** Creates the necessary directory structure on Recalbox and copies files to their correct locations.
* **Auto-Permissions (No PuTTY Required):** Configures Recalbox scripts (`Recalbox_1(permanent).sh` / `Recalbox_2(permanent).sh`) to execute automatically on boot without manual `chmod` commands.
* **Animated Marquees:** Installs the GIF rendering engine (`pixel_stream.py`), responsible for detecting and playing animated marquees when a game starts.

---

### 🛠️ Prerequisites

1. Connect your **PC** and **Recalbox** to the same local network (or connect Recalbox's physical storage directly to your PC).
2. Know the **local IP address of your Retro Pixel LED panel** (e.g., `192.168.1.117`).
3. Download the full `Instalador Automático` folder from this repository [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Instalador%20Automatico).
   
> [!IMPORTANT]
> If you downloaded the repository as a `.zip` file, **extract it completely** before running the installer.

---

### 💻 Step-by-Step

1. Open the `Instalador Automatico` folder on your PC. Inside you will find two files and two folders:
   * `Ejecutar Script Instalador Arcade.bat`
   * `Script_Instalador_Arcade.ps1`
   * `Batocera`
   * `Recalbox`

2. **Click** on `Ejecutar Script Instalador Arcade.bat`.

3. Follow the console prompt instructions:
   * **Step 1:** Enter the IP of your LED panel and press `Enter`.
   * **Step 2:** Enter the path to your Recalbox system. This can be a network path (e.g., `\\192.168.1.118`) or a physical drive letter if connected directly to your PC (e.g., `E:`).

4. The script will request the following options:
   * System selection: Select **2 Recalbox**.
   * Preferred operating mode:
     * **Option 1:** Menus and Games (Displays system logos while browsing + marquees when playing).
     * **Option 2:** Games Only (Displays fixed marquee/clock in menus, switches only when playing).

> [!NOTE]
> **Animated** marquees function identically in both modes — the difference between Option 1 and Option 2 is limited to system navigation behavior and does not affect game launching.

5. The script processes all files in seconds. Upon completion, the message `INSTALACIÓN COMPLETADA!` will appear. Press any key to exit.

<img width="1102" height="532" alt="image" src="https://github.com/user-attachments/assets/3e367c0a-d305-475e-95c9-ed9d3ae352e9" />

6. **Reboot your Recalbox system completely.**
> [!CAUTION]
> A complete system reboot is **mandatory**. Once restarted, the panel will automatically respond whenever you navigate menus, launch games, or exit games.

### 3. 🛠️ Marquees Setup
Use the script located in the project's `Arcade/Marquesinas/` folder [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas). It consists of two files: `Ejecutar Script Marquesinas Recalbox.bat` and `Script Marquesinas Recalbox.ps1`.

1. **Execute** `Ejecutar Script Marquesinas Recalbox.bat` (Launcher script designed to prevent Windows execution blocks).
2. **Path Configuration:**
    * **Source:** Enter the path to your Recalbox ROMs (e.g., `\\192.168.1.118\share\roms`).
    * **Destination:** Enter `C:\marquesinas`.
3. **Image Selection:** Select the image type to use for marquees.
4. **System Selection:** The script automatically detects systems containing a `gamelist.xml` file. Select individual systems by number, multiple systems, or **All (0)**.
5. **Copying Files:** If using `C:\marquesinas`, copy the `marquesinas` folder and its contents to your Recalbox SD/SSD under `share\`, as described in section `6. File Structure on Recalbox SD or SSD`.

<img width="1098" height="630" alt="image" src="https://github.com/user-attachments/assets/f2a99ce2-0b83-40cc-84bc-d24962f2c83e" />

### What Does the Script Do Automatically?
* **Resizing:** Automatically resizes original marquees to **128x32 pixels**.
* **Format Conversion:** Converts color formats to **24-bit BMP** (compatible with the ESP32 DMA driver).

> [!CAUTION]
> **Network Access (Samba):**
> If the script cannot access your network path, log into your Recalbox share using Windows File Explorer using default credentials:
> Access path e.g.: `\\192.168.1.120\share\roms`
> * **Username:** `root`
> * **Password:** `recalboxroot`

> [!CAUTION]
> Every time you add new games or run a "Scrape" in Recalbox, **you must re-run the PowerShell script** on your PC to update image indexes. Otherwise, new files will not be recognized by the ESP32.

### 4. 🎬 Animated Marquees (GIF)

In addition to static images, the panel can play **animated GIFs** when launching a game — creating a moving marquee during gameplay.

#### How It Works

- While **browsing** systems and games, the panel behaves identically to static mode.
- When you **launch** a game, the panel checks if a `.gif` file with the exact same name as the static marquee exists in the same folder.
- **If found, it loops the GIF continuously** for the duration of your game session, reverting to normal GIF/clock mode when you exit.
- **If not found, it defaults to the static marquee** without errors. GIFs can be added incrementally over time.

#### File Naming

GIF files must share the **exact same name as the `.bmp` marquee** in the same folder:

```
share/marquesinas/Arcade/neogeo/mslug.bmp   <- Existing static marquee
share/marquesinas/Arcade/neogeo/mslug.gif   <- Corresponding animated GIF
```

Multiple GIFs can be sequenced for a single game using suffixes `_01`, `_02`, `_03`... The panel plays them sequentially in a continuous loop:

```
share/marquesinas/Arcade/neogeo/mslug.gif
share/marquesinas/Arcade/neogeo/mslug_01.gif
share/marquesinas/Arcade/neogeo/mslug_02.gif
```

> [!TIP]
> Sequenced files are optional; a single `mslug.gif` file loops seamlessly.

#### Where to Find GIFs?

If you have a collection of arcade GIFs named with title strings instead of MAME romset shortnames (e.g., `ARCADE_NEOGEO_MetalSlugStory.gif` instead of `mslug.gif`), download the renaming tool from [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs) and follow these steps:

1. Execute `Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`.

2. **Option 1 — Rename GIFs:** Point to your GIF folder. The script cross-references a public MAME database ([`MAME.dat`](https://github.com/libretro/libretro-database)) and an internal dictionary to identify corresponding romset names. Unidentified files move to a `SinResolver\` folder for manual review.
   <img width="1090" height="830" alt="image" src="https://github.com/user-attachments/assets/58ba389f-367c-4114-b6e1-533018f50e77" />

3. **Option 2 — Copy GIFs to System Folders:** Matches renamed GIFs against active ROMs in your `ROMS/` directory and copies them directly to `Arcade/<system>/` alongside existing `.bmp` files.
   <img width="1106" height="1204" alt="image" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

You can also generate your own — files must be **128×32 pixels**. If your system includes scraped preview videos (`<video>` tags in `gamelist.xml`), tools like [dmd_gif_converter](https://github.com/red77290/dmd_gif_converter) can convert and automatically frame videos down to 128×32 resolution.

 ### 5. 🛠️ System Logos
 Pre-sized logos are available in the `Arcade/Logos Sistemas/` repository directory [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas).
 1. **Copying:** Copy the `Logos` folder and contents to `share\marquesinas` on your Recalbox storage as indicated in section 6.

To process custom logos (e.g., from an active system theme), use the script in `Arcade/Logos Sistemas/` [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas):

1. Execute `Ejecutar Script Logos.bat`.
2. **Path Configuration:**
    * **Source:** Enter your theme logo path (e.g., `\\192.168.1.119\share\themes\Animatics-DX-master\art\logos`).
    * **Destination:** Enter `C:\Logos`.
3. **Copying:** Copy `C:\Logos` and contents to `share/marquesinas/` on Recalbox.

<img width="1102" height="573" alt="image" src="https://github.com/user-attachments/assets/7d90cc90-3cad-4991-8498-591081ab2004" />

### What Does the Script Do Automatically?
* **Resizing:** Converts system logos to **128x32 pixels**.
* **Format Conversion:** Enforces **24-bit BMP** color formatting.

> [!CAUTION]
> **Network Access (Samba):**
> If access fails, open File Explorer and log into Recalbox credentials:
> Path e.g.: `\\192.168.1.120\share\themes\Animatics-DX-master\art\logos`
> * **Username:** `root`
> * **Password:** `recalboxroot`

## 6. File Structure on Recalbox SD or SSD

Place the `marquesinas` directory inside `share/`:
* **`share/marquesinas/Arcade/system/rom_name.bmp`** (Static game marquee, e.g., `mslug.bmp`)
* **`share/marquesinas/Arcade/system/rom_name.gif`** (Optional: Animated game marquee, e.g., `mslug.gif`)
* **`share/marquesinas/Logos/system_name.bmp`** (Processed system logo, e.g., `mame.bmp`)

#### Folder Tree Example:
```
📂 share/
├── 📂 marquesinas/
│   └── 📂 Arcade/
│   │   └── 📂 neogeo/
│   │   │   ├── 📄 mslug.bmp
│   │   │   ├── 📄 mslug.gif       <- Optional animated marquee
│   │   │   ├── 📄 kof98.bmp
│   │   │   └── ...
│   │   └── 📂 mame/
│   │       ├── 📄 pacman.bmp
│   │       ├── 📄 tetris.bmp
│   │       └── ...
│   └── 📂 Logos/
│       ├── 📄 atari2600.bmp
│       ├── 📄 mame.bmp
│       └── ...
```
## 7. Enjoy your marquees while playing on your Arcade cabinet!

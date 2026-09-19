# 🕹️ Integration with ReplayOS

**Arcade Mode** in the Lite version allows your LED matrix to function as a dynamic marquee with **ReplayOS**. The panel will automatically detect the system and game you are currently playing, display it, and if the game has an animated marquee available, play it in a continuous loop while you play.

> [!NOTE]
> **Key difference from Batocera/Recalbox:** ReplayOS does not have a scripting/event system like those frontends, so **nothing is installed on the ReplayOS device**. Instead, the ESP32 itself periodically queries the ReplayOS **REST API** to check which system and game are currently active, and searches for the corresponding marquee on its own SD card. All content preparation (images, GIFs, list files) is done on your PC using the PowerShell tools in this section, and the result is copied to the panel's SD card.

#### Resource Harvesting (Scraping)
Just like in Batocera, there is no need to search for marquees manually game by game: the tools in this section download images from **ArcadeDB** (or **TheGamesDB**) and automatically convert them to the format required by the panel.

## 1. Critical Configuration: REST API and ReplayOS Token

For **🕹️ Arcade** mode to work, the ESP32 needs to be able to query ReplayOS about what is being played. This requires three things:

1. **Enable network control in ReplayOS:** under `REPLAY OPTIONS > SYSTEM`, enable the **`NET CONTROL`** (`system_net_control`) option. This opens the API server on port `55356`.
2. **Obtain the Token (Net Control Code):** go to `REPLAY OPTIONS > INFORMATION > NET CONTROL CODE`. It is a 6-digit numeric code — you will need it to configure the ESP32.
3. **Static IP for your ReplayOS:** the ESP32 always queries the same IP address, so it must be fixed.

> [!TIP]
> **Assigning a static IP to your ReplayOS:**
> 1. Access your router's configuration page.
> 2. Look for the **Static DHCP** or **IP-to-MAC Binding** section.
> 3. Bind the MAC address of your Raspberry Pi/mini PC running ReplayOS to a static IP (e.g., `192.168.1.120`).
> 4. Since every router model is different, search Google if you have doubts: *"How to assign a static IP [your router model]"*.

4. **Configure the ESP32:** You can configure it either via the APP or the config.ini file (*the APP is recommended*).
- **APP:** Open settings, navigate to the *ARCADE* section, select `ReplayOS`, and in the *REPLAYOS* section enter the IP and Token.
  
  <img width="428" height="945" alt="image" src="https://github.com/user-attachments/assets/1724a3a3-f292-4b11-8e54-b547c4ece0fa" />
  <img width="412" height="938" alt="image" src="https://github.com/user-attachments/assets/e649cf1b-1a98-423e-b3c8-5386d39fd0f1" />

- **Config.ini:** Open the config.ini file, in the *[LOGIC]* section set `ARCADE_ENABLE=3`, and in the *[REPLAY_OS]* section specify the assigned ReplayOS IP under `IP=xxx.xxx.xxx.xxx` and the token under `TOKEN=xxxxxx`.
  <img width="683" height="155" alt="image" src="https://github.com/user-attachments/assets/ec850c72-c111-46ab-bb0b-1c6bd9b24073" /> <img width="510" height="141" alt="image" src="https://github.com/user-attachments/assets/18c8d28c-2b52-48f0-b384-d9e66196467d" />

## 2. PowerShell Tools

Unlike Batocera and Recalbox, there is no installer that deploys files onto ReplayOS itself. Instead, there are **two scripts** that prepare all the content on your PC, ready to be copied to the panel's SD card:

* **`Script_Marquesinas_ReplayOS.ps1`** — scrapes assets from ArcadeDB/TheGamesDB, generates `.bmp` images, and creates/audits the `.txt` lists that the ESP32 uses to quickly locate which romsets have marquees.
* **`Script_RetroPixelLED_GIF_Renamer.ps1`** — if you already have a collection of animated arcade GIFs (for instance, from a third-party pack) with "human-readable" names instead of MAME romset names, this script renames them automatically and places them in the corresponding system folder.

### 🛠️ Prerequisites

1. Have access from your PC to both your **ROMS** folder and the panel's **SD card** (inserted into the PC or accessible as a drive).
2. Download the marquee scripts **`Ejecutar Script Marquesinas_ReplayOS.bat`** and **`Script_Marquesinas_ReplayOS.ps1`**, available [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas).
3. Download the GIF renamer scripts **`Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`** and **`Script_RetroPixelLED_GIF_Renamer.ps1`**, available [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs).
> [!IMPORTANT]
> If you downloaded the repository as a `.zip` archive, make sure to **extract it completely** before running the scripts.

### 💻 Step-by-Step: Scraping, Images, and Lists

1. Run `Ejecutar Script Marquesinas_ReplayOS.bat`.
2. **Option 1 — Scrape system(s):** enter the path to your `ROMS` folder and the path where you want to store the downloaded assets. The script will show you the system subfolders found in the given path; select one, several, or type `TODOS` (ALL).
   <img width="1100" height="740" alt="image" src="https://github.com/user-attachments/assets/684f93c5-ab5c-46e2-a792-e1b3535f9ed6" />
   
3. Choose the provider (ArcadeDB or TheGamesDB) and which assets to download (marquee, logo, decal...). **DECAL** is recommended for creating marquees. The script will save raw assets in a reusable cache folder without generating `.bmp` files yet.
   <img width="1101" height="656" alt="image" src="https://github.com/user-attachments/assets/cfb9c33e-77a9-43ba-91e8-c3a84bcea19a" />

4. **Option 2 — Generate images:** converts cached images offline to 128×32 `.bmp` files with RGB565 dithering applied, placing them in your local `Arcade/<system>/` folder.
   <img width="1098" height="875" alt="image" src="https://github.com/user-attachments/assets/110fc03e-e99e-4e87-840c-660eed8808ff" />

5. **Option 3 — Generate / audit lists:** scans the `Arcade/<system>/` folder and generates (or updates) the `<system>.txt` file that the ESP32 uses to quickly check which romsets have marquees. If a list already exists, it will notify you of missing or extra files before applying changes.
   <img width="1104" height="619" alt="image" src="https://github.com/user-attachments/assets/19718bdc-b849-44b1-916b-7ac81835f1c3" />

7. **Copy the entire `Arcade` folder to the root of the Retro Pixel LED Lite SD card.**

### 🎬 Animated Marquees (GIF)

#### How does it work?

- The panel displays the static marquee (`.bmp`) of the game while detecting active gameplay, as usual.
- If a `.gif` file with the exact same name exists in the same folder, the panel plays it in a loop during gameplay.
- If it does not exist, nothing breaks — it simply stays on the static marquee without errors.

#### Naming Convention

The GIF must share the exact same name as the static `.bmp` marquee for that game:

```
Arcade/snk_ngo/mslug.bmp   <- existing static marquee
Arcade/snk_ngo/mslug.gif   <- added by you, same filename
```

You can also set up a sequence of multiple GIFs for the same game using the suffixes `_01`, `_02`, `_03`, etc. The panel will play them sequentially and loop back to the first one continuously:

```
Arcade/snk_ng/mslug.gif
Arcade/snk_ng/mslug_01.gif
Arcade/snk_ng/mslug_02.gif
```

> [!TIP]
> You do not need to have all three — having just `mslug.gif` works perfectly in a loop.

#### The `_default` Fallback

If a game has neither a custom marquee nor a system logo, the panel will ultimately look for a `_default.bmp` (or `_default.gif` if you want it animated) in the root of the `Arcade/` folder. This is optional, but it ensures the panel always displays something for games you haven't processed yet.

#### Where do I get GIFs?

If you already have (or downloaded) a collection of arcade GIFs with "human-readable" names instead of romset names (for example `ARCADE_NEOGEO_MetalSlugStory.gif` instead of `mslug.gif`), use the renamer script, downloaded from [here](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs):

1. Run `Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`.

2. **Option 1 — Rename GIFs:** select the folder containing your GIFs. The script queries a public MAME catalog ([`MAME.dat`](https://github.com/libretro/libretro-database)) to map each title to its corresponding romset name, along with an internal dictionary for common cases. You can choose exact matching only, or exact + fuzzy matching (resolves more files with a slight margin of risk). Unresolved files are moved to a `SinResolver\` folder for manual review — it never renames blindly.
   <img width="1090" height="830" alt="image" src="https://github.com/user-attachments/assets/58ba389f-367c-4114-b6e1-533018f50e77" />

3. **Option 2 — Copy GIFs to system folders:** once renamed, this option compares the GIFs against the actual romsets in your `ROMS/` folder and automatically copies them to `Arcade/<system>/`, alongside existing `.bmp` files.
   <img width="1106" height="1204" alt="image" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

> [!NOTE]
> This second option can also copy files over the network to Batocera or Recalbox paths if you prepare GIFs for multiple frontends at once from the same PC.

## 3. SD Card File Structure on Retro Pixel LED Lite

For the integration to function correctly, the `Arcade` folder must be placed at the **root** of the panel's SD card:

* **`Arcade/<system>.txt`** (list of romsets with marquees for that system)
* **`Arcade/<system>/rom_name.bmp`** (static marquee image, e.g., `mslug.bmp`)
* **`Arcade/<system>/rom_name.gif`** (optional: animated marquee image for the game)
* **`Arcade/<system>.bmp`** (optional: system logo if no game-specific marquee exists)
* **`Arcade/_default.bmp`** (optional: fallback image if neither marquee nor logo exists)
* **`Arcade/_default.gif`** (optional: fallback GIF if neither marquee nor logo exists)

#### Visual Folder Structure Example:
```
📂 F:\ (RetroPixelLED SD Card)
└── 📂 Arcade\
    ├── 📄 snk_ng.txt
    ├── 📄 arcade_fbneo.txt
    ├── 📄 arcade_ng.bmp        <- Neo Geo system logo
    ├── 📄 _default.bmp          <- general fallback image
    ├── 📄 _default.gif          <- general fallback animation
    ├── 📂 snk_ng\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif         <- optional animated marquee
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 arcade_fbneo\
        ├── 📄 dino.bmp
        ├── 📄 avsp.bmp
        └── ...
```

## 4. Enjoy your marquees while gaming on your ReplayOS Arcade cabinet!

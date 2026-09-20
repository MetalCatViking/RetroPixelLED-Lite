# ✨ Retro Pixel LED Lite

<p align="center">
  <img alt="Version" src="https://img.shields.io/badge/version-3.1.3-blue">
  <img alt="Platform" src="https://img.shields.io/badge/platform-ESP32-informational">
  <img alt="License" src="https://img.shields.io/badge/license-MIT-green">
  <img alt="Status" src="https://img.shields.io/badge/status-active-success">
</p>

<p align="center">
  <a href="https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/README.md">🇪🇸 Español</a> ·
  <a href="https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Frances/README.md">🇫🇷 Français</a> ·
   <a href="https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Ingles/README.md">🇬🇧 English</a> ·
  <a href="https://t.me/RetroPixelLed">✈️ Telegram Group</a>
</p>

<p align="center">
  <a href="https://paypal.me/fjgordillo"><img alt="Donate with PayPal" src="https://img.shields.io/badge/☕_Buy_me_a_coffee-PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white"></a>
</p>

> 💛 If Retro Pixel LED is brightening up your home retro corner, you can buy me a coffee using the button above. Every bit helps to keep bringing new features!

---

## 💡 Project Description

**Retro Pixel LED Lite** is the high-performance edition of Retro Pixel LED, designed for those seeking absolute stability, instant speed, and a maintenance-free system. Unlike the standard version, the **LITE** firmware ditches the embedded web server and continuous network overhead to dedicate 100% of the ESP32’s processing power to one main task: rendering GIFs flawlessly.

While the 2.x.x branch introduced the OSD Menu, **v3.0.0** was the definitive leap towards hardware independence: the panel became an autonomous smart device that does not require connection to a PC for setup or maintenance. `config.ini` and playlists can be edited directly via Windows File Explorer or an FTP client, turning your SD card into a wireless network drive. It also includes native IR remote control support: browse the OSD Menu, adjust brightness, and toggle power right from your couch.

**Starting from v3.1.0, you can control the panel from a web app (PWA)**, and since **v3.1.2**, via **Home Assistant** as well. 🏠

---

## 📑 Table of Contents

1. [🆕 What's New in the Current Version](#-whats-new-in-v313-lite)
2. [🚀 Quick Start Guide](#-quick-start-guide)
3. [🎛️ Key Features](#️-key-features)
   - [🖥️ OSD Menu](#️-osd-menu-smart-navigation)
   - [📱 PWA Remote Control App](#-pwa--remote-control-app)
   - [🏠 Home Assistant](#-home-assistant)
   - [🕹️ Arcade Mode](#️-arcade-mode-batocera-recalbox--replayos)
   - [🕒 Clock & Weather](#-clock--weather)
   - [⏰ Timer](#-timer)
   - [🌐 Multi-language Support](#-multi-language-support)
   - [📂 FTP Server](#-ftp-server)
   - [🔄 OTA Updates](#-ota-updates)
4. [⚙️ Setup & Configuration](#️-setup--configuration)
   - [1. Flashing the ESP32](#1--flashing-the-esp32-web-installer)
   - [2. Preparing the SD Card](#2--sd-card-preparation)
   - [3. The `config.ini` File](#3--configuration-via-configini)
   - [4. Time Zone (TZ) Setup](#4--time-zone-tz-configuration)
   - [5. Weather API Key](#5--how-to-get-your-weather-api-key)
5. [📖 Playlist Generator (Windows)](#-playlist-generator-windows)
6. [🕹️ Arcade Integration](#️-arcade-integration-batocera-recalbox-or-replayos)
7. [🏠 Home Assistant Integration](#-home-assistant-integration-complete-guide)
8. [🧠 Internal Architecture / Core Lite](#-internal-architecture--core-lite)
9. [📜 Detailed Changelog](#-detailed-changelog-v300--v313)
10. [🛒 Bill of Materials](#-bill-of-materials)
11. [🔌 Pinout & Wiring](#-pinout--wiring)
12. [🛠️ Roadmap](#️-roadmap)
13. [⚖️ License & Acknowledgments](#️-license--acknowledgments)

---

## 🆕 What's New in v3.1.3 Lite

- **🕹️ RePlayOS Support:** automatic playback of GIFs and retro marquees upon switching games via integration with the RePlayOS frontend.
- **⏰ New Clock Visual Styles:** additional customization options for rendering the clock display on the LED panel.
- **📡 IR Remote Mapping via PWA:** capture, assign, and configure remote control buttons directly from the web interface.

For details on previous releases (Arcade GIF marquees, WiFi reconnection, IP display in OSD menu...), check the [Detailed Changelog](#-detailed-changelog-v300--v313).

---

## 🚀 Quick Start Guide

If this is your first time installing Retro Pixel LED Lite, follow these steps to get started quickly:

1. **Flash the firmware** using the [Web Installer](#1--flashing-the-esp32-web-installer) — all you need is Chrome or Edge; no PC software installation required.
2. **Prepare the MicroSD card** formatted in FAT32 using the [contents of the `Contenido SD` folder](#2--sd-card-preparation).
3. **Edit `config.ini`** with your WiFi details and preferences — this is the only file you need to edit to get running ([full reference](#3--configuration-via-configini)).
4. **Power on the panel.** It will synchronize the time, load your GIFs, and be ready to go. ✨
5. *(Optional)* Install the [PWA](#-pwa--remote-control-app) to control it from your phone, or [integrate it with Home Assistant](#-home-assistant-integration-complete-guide) for home automation.
6. *(Optional)* If you run Batocera, Recalbox, or ReplayOS, follow the [Arcade Integration Guide](#️-arcade-integration-batocera-recalbox-or-replayos) for dynamic game marquees.

The remainder of this document serves as a detailed reference guide for each feature. 🙂

---

## 🎛️ Key Features

This section provides an overview of **what** each part of the system does. For step-by-step setup instructions, jump to [⚙️ Setup & Configuration](#️-setup--configuration).

### 🖥️ OSD Menu (Smart Navigation)

The system is operated via a **single push button** (or IR remote) using input logic that adapts dynamically based on the current context:

- **Short Press:**
  - **In menus:** move cursor / navigate down.
  - **In sleep mode:** wakes up the panel instantly.
- **Long Press:**
  - **General action:** enter submenus or confirm selections.
  - **In Timer config:** subtracts **-5 minutes** from the current value.
- **Extra Long Press (> 4 sec):**
  - **Manual override:** forces sleep mode, overriding the timer schedule until the next cycle.
- **Continuous Hold:**
  - **In Timer config:** continuously increments by **+5 minutes** in a loop while held.

```text
🏠 MAIN MENU
├── 📂 Playlists
│   ├── 📄 Favorites
│   ├── 📄 Arcade
│   ├── 📄 ...
│   └── 🔙 Back
├── 📂 Playback
│   ├── 🖼️ Mode: [GIFs / Clock]
│   ├── 🔀 Shuffle: [YES / NO]
│   ├── 🕹️ Arcade: [OFF / Batocera / Recalbox / ReplayOS]
│   ├── 💬 Text: [YES / NO]
│   └── 🔙 Back
├── ☀️ Brightness
│   └── Level: [5% - 100%]
├── 📶 WiFi: [ON / OFF]
│   ├── 🔄 Toggle: [YES / NO]
│   ├── 🔎 Show IP: [YES / NO]
│   ├── 🏷️ IP: [192.168.1.117]
│   ├── 📱 App Control: [YES / NO]
│   └── 🔙 Back
├── 🕒 Clock: [ON / OFF]
│   ├── 🔄 Toggle: [YES / NO]
│   ├── 🖼️ Interval: Every [1...20] GIFs
│   ├── ⏳ Duration: Show [5...30] sec
│   ├── 🎨 Clock Style: [Matrix, Solid, Rainbow, Pulse, Gradient]
│   ├── 🎨 Color: [White, Red, Green, Blue, Yellow, Cyan, Magenta, Orange, Pink]
│   ├── 🔄 Transition: [YES / NO]
│   └── 🔙 Back
├── 🌡️ Weather: [ON / OFF]
│   ├── 🔄 Toggle: [YES / NO]
│   └── 🔙 Back
├── 🕒 Timer: [ON / OFF]
│   ├── 🔄 Toggle: [YES / NO]
│   ├── ⏳ ON Time: [00:00 to 24:00]
│   ├── ⏳ OFF Time: [00:00 to 24:00]
│   └── 🔙 Back
├── ⚙️ Advanced Settings
│   ├── ⚡ I2S Speed: [8, 10, 16, 20MHz]
│   ├── 🔄 Refresh Rate: [30, 60, 90, 120Hz]
│   ├── 🖼️ Buffer: [YES / NO]
│   ├── 👻 AntiGhost: [1, 2, 3, 4]
│   ├── 🎮 IR Mapping: [On, Off, Menu, Select, Up, Down, Bright+, Bright-]
│   ├── ⚠️ Reset
│   └── 🔙 Back
├── 🚀 Updates
│   ├── 🔄 Check OTA
│   ├── 🔤 Download Languages
│   └── 🔙 Back
├── 📂 SD Explorer
│   ├── 🔄 Start FTP
│   └── 🔙 Back
├── 🌐 Language
│   ├── [ES] Español
│   ├── [EN] English
│   ├── [FR] Français
│   ├── ...
│   └── 🔙 Back
├── 💾 Save
└── 🔙 Exit
```

---

### 📱 PWA — Remote Control App

**[👉 Install or test Retro Pixel LED Control](https://fjgordillo86.github.io/RetroPixelLED-Lite/control/)**

A modern web app installable on any smartphone, tablet, or desktop connected to the same local network as your display. It does not rely on cloud servers and works offline once loaded. 📴

https://github.com/user-attachments/assets/f5231448-7862-4476-901e-ac25ac7f4248

The interface is structured into **5 main sections**:

**1️⃣ Main Dashboard (Home)**
- **☀️ Brightness Control:** 0–100% instant adjustment slider with zero delay and no reboot required.
- **🎛️ Mode Switcher:** Real-time toggling between GIF, Clock, and Text modes.
  - **GIF Mode:** displays active playlist with shuffle controls.
  - **Clock Mode:** select from 5 styles (Matrix, Solid, Rainbow, Pulse, Gradient) and 9 color themes.
  - **Text Mode:** live matrix preview while typing, customizable text color, font, and scroll speed.
- **🔌 Connection Status:** visual status indicator (green/red) for local WiFi connectivity.

**2️⃣ Schedule & Timer ⏰**
- Toggle automatic schedule on/off.
- Set power-on and power-off times (24h format).
- Instant manual power toggle override.
- Real-time power indicator: active (✓ green) or sleeping (● gray).

**3️⃣ Ticker Text Mode**
- Real-time 26×7 LED matrix preview while typing.
- Color palette picker (9 presets) or custom Hex input.
- **Font selector:** `Bold`, `SemiBold`, `Regular`, `Light` — *(New in v3.1.2)*.
- Speed slider: adjust from 5–200ms per step with live animation preview.
- Quick action controls: "▶ Send" and "■ Stop".

**4️⃣ System Updates 🔄**
- **Firmware OTA:** checks GitHub releases and updates automatically over the air.
- **Language Downloads:** fetches local translation files (`.json`) directly to `/idioma` on the SD card.

**5️⃣ System Settings 🛠**

Remote configuration manager for `config.ini`, split into 7 modules: WiFi Settings, Hardware Setup, Arcade Mode, Ticker Text, Clock Options, Weather Integration, and Language Selection.

Triggers an automatic reboot upon saving *only* if hardware changes require it.

#### ⚙️ PWA Installation & Connection

1. Open <https://fjgordillo86.github.io/RetroPixelLED-Lite/control/> on your device.
2. Tap the connection icon (⚙) and enter your display panel's local IP (e.g., `192.168.1.117`).
3. *(Optional)* Install as a standalone web application: Chrome/Edge will prompt automatically; otherwise, use the browser menu (⋮) → "Install app". On iOS Safari, tap Share (↗) → "Add to Home Screen".
4. Ready — launches like a native app without typing URLs. 🎉

#### 📝 Prerequisites

- Device and ESP32 must be connected to the same local WiFi network.
- `CONFI_APP_ENABLE=1` must be set in `config.ini` to allow API access.
- `TEXT_ENABLE=1` must be enabled to send scrolling text messages.
- Active internet connectivity on the panel for firmware/language updates (GitHub access).
- Language files are saved locally to the SD card for offline menu usage.

---

### 🏠 Home Assistant

Complete local control from your Home Assistant dashboard via **Local REST API** — no cloud, no internet dependency required. Features:
- 🟢 **Power toggle** via a native `switch`.
- 📊 **Status monitoring** (current display mode, active playlist, etc.).
- 🔄 **Instant mode & playlist switching** on demand.
- 💬 **Scrolling text sender** with options for custom colors, speeds, and **fonts**.

See the full [Home Assistant Integration Guide](#-home-assistant-integration-complete-guide) below for configuration examples.

---

### 🕹️ Arcade Mode (Batocera, Recalbox & ReplayOS)

Transforms your panel into an active dynamic marquee that responds live to your current game. Features two integration modes: local script execution (**Batocera / Recalbox**) or background network polling (**ReplayOS**).

Displays automatically:
1. **Game Marquee:** 24-bit `.bmp` image or an **animated GIF** (including sequentially queued multiple GIFs looping seamlessly).
2. **System Logo:** fallbacks to system `.bmp` graphics while browsing games.

Enable via `Menu → Playback → Arcade`. For setup instructions, refer to the [Arcade Integration Section](#️-arcade-integration-batocera-recalbox-or-replayos).

---

### 🕒 Clock & Weather

- **Clock Display:** 5 styles (Matrix, Solid, Rainbow, Pulse, Gradient) and 9 color presets, featuring particle explosion transitions. Automatically interrupts the GIF playback stream every *x* GIFs for *x* seconds, resuming precisely where it left off.
- **Weather Display:** integrated via free OpenWeatherMap API key. Displays current temperature, animated status icon, and a customizable text header (`WEATHER_MSG`). See [How to Get Your Weather API Key](#5--how-to-get-your-weather-api-key).

---

### ⏰ Timer

Automated power schedules with manual override options via hardware push button or web PWA. Pressing the button temporarily overrides the automated schedule until the next cycle.

---

### 🌐 Multi-language Support

Features **Dynamic Dictionary Memory Allocation**: translation files are loaded into RAM *only* while browsing the OSD Menu and freed immediately upon exit, preserving maximum heap memory for GIF decoding.

- **Storage path:** `/idioma/` on the SD card. The file name (without extension) dictates the OSD label: `/idioma/EN.json` → "EN".
- **JSON Structure:** broken down into `MENU`, `SUBMENU_XXX`, `ESTADOS`, and `CONFIG_INI` blocks.
- **Formatting Rules:**
  - 🚫 Avoid special accent marks or unsupported unicode characters.
  - 📏 Keep menu strings under 21 characters for proper auto-centering at 128px.
  - 🔡 Include trailing colons and spaces if required by context (e.g., `"mode": "Mode: "`).
  - 💾 Save files encoded as UTF-8 (without BOM).
- **Remote Sync:** download latest translation packs directly from GitHub using the PWA or OSD menu (`Updates → Download Languages`).

---

### 📂 FTP Server

Integrated wireless file management server for performing updates without removing the MicroSD card.

> [!IMPORTANT]
> Designed specifically for editing **`config.ini`**, language dictionaries (`.json`), and **playlists** (`.txt`). Transferring large GIF image collections over FTP is not recommended due to speed constraints compared to a dedicated SD reader.

**To enable:** navigate to `OSD Menu → SD Explorer → Start FTP`. The panel pauses GIF rendering and outputs its local IP address.

**Recommended Client Setup — FileZilla:**

| Parameter | Recommended Value |
| :--- | :--- |
| Protocol | Plain FTP (Insecure) |
| Host | Local IP shown on display |
| User / Password | `admin` / `admin` |
| Port | `21` |
| Max Connections | 1 (strictly enforced) |
| Upload/Download Limit | 20 KiB/s |

You may also map it as a Network Location in Windows File Explorer (`ftp://<IP>`, user `admin`). FileZilla is recommended for reliable transfers.

**Safety Notes:** GIF playback is suspended while FTP server mode is active to dedicate system resources to file transfer operations. Exit via physical button or remote "OK" key. Avoid disconnecting power while writing files via FTP.

Check the detailed [Step-by-Step FTP Setup](#-sd-explorer-ftp--details) below.

---

### 🔄 OTA Updates

Update system firmware wirelessly:
1. Connect panel to local WiFi via `config.ini`.
2. Go to `OSD Menu → Updates → Check OTA` (or trigger from the PWA).
3. The panel downloads the latest compiled binary from GitHub and reboots automatically. 🔃

> [!WARNING]
> Do not power off the display during an active OTA update cycle.

---

## ⚙️ Setup & Configuration

### 1. 🚀 Flashing the ESP32 (Web Installer)

Flash the system without installing toolchains or drivers:

**[👉 Launch Retro Pixel LED Lite Web Installer](https://fjgordillo86.github.io/RetroPixelLED-Lite/)**

1. Open the page in **Google Chrome** or **Microsoft Edge**.
2. Connect your ESP32 board via USB.
3. Click **"Install"** and choose the correct serial COM port.
4. **Important:** check **"Erase device"** to perform a clean flash and avoid heap memory issues.

> 💡 **ESP32 not recognized?** If no COM port appears, install the corresponding USB driver for your board:
> - **CP2102 Chip:** [Silicon Labs Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
> - **CH340/CH341 Chip:** [SparkFun Drivers](https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all)

### 2. 📂 SD Card Preparation

Format your MicroSD card to **FAT32** and copy the contents of the [Contenido SD](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Contenido%20SD) directory to the root level:

```text
/ (SD Root)
├── gifs/                        <-- Folders containing your GIF collections (Arcade, Consoles, etc.)
├── idioma/                      <-- JSON language files for menu translations.
│   ├── ES.json
│   ├── EN.json
│   └── FR.json
├── playlists/                   <-- Text lists generated by the "Playlist Generator" script.
│   ├── Arcade.txt
│   ├── Computers.txt
│   ├── Consoles.txt
│   └── All.txt
├── config.ini                   <-- System and WiFi configuration file.
└── Generador de Playlists.bat   <-- Windows script to generate custom playlists.
```

> [!IMPORTANT]
> If you add, delete, or reorganize GIF files inside `/gifs/`, re-run `Generador de Playlists.bat` to update directory indexes.

### 3. 📝 Configuration via `config.ini`

The default configuration file is provided in the [Contenido SD](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Contenido%20SD) folder. Copy it to the root of your SD card and modify parameters as needed:

```ini
# ============================================================
# 🕹️ RETRO PIXEL LED LITE v3.1.3 - CONFIGURATION FILE
# ============================================================
# Note: Do not leave spaces around '=' signs.
# Correct example: BRIGHTNESS=40

[WIFI_NTP]
# Configure your WiFi network credentials
WIFI_ENABLE=1
SSID=Your_WiFi_SSID
PASS=Your_WiFi_Password
# Set your local POSIX Timezone string
TZ=CET-1CEST,M3.5.0,M10.5.0/3

[HARDWARE]
# Number of chained LED panels
PANEL_CHAIN=2
# Panel color order: RGB, RBG, or GBR
COLOR_ORDER=RGB
# Master Brightness level (0 to 255)
BRIGHTNESS=38
# I2S Bus Speed: 0=8MHz, 1=10MHz, 2=16MHz, 3=20MHz (Turbo)
I2S_SPEED=2
# Minimum Refresh Rate (Hz): 30 to 120
REFRESH_MIN=120
# Double Buffering: 0=OFF, 1=ON (Eliminates flickering)
DOUBLE_BUFF=0
# Anti-Ghosting Latch Blanking: 1 to 4
LATCH_BLANK=1

[LOGIC]
# Display Mode: 0=GIFs, 1=Clock Only
PLAY_MODE=0
# Enable App Configuration Interface: 0=OFF, 1=ON (Requires WiFi)
CONFI_APP_ENABLE=1
# Arcade Integration: 0=OFF, 1=Batocera, 2=Recalbox, 3=ReplayOS
ARCADE_ENABLE=0
# Enable Ticker Text: 0=OFF, 1=ON (Requires WiFi)
TEXT_ENABLE=1
# Enable Clock Overlay: 0=OFF, 1=ON (Requires WiFi)
CLOCK_ENABLE=1
# GIF Playback Order: 0=Sequential, 1=Random/Shuffle
RANDOM_MODE=1
# Clock Interval: Show clock every X GIFs
AUTO_CLOCK_INT=6
# Clock Duration: Show clock for X seconds
CLOCK_DURATION=10
# Clock Visual Style: 0=Matrix, 1=Solid, 2=Rainbow, 3=Pulse, 4=Gradient
CLOCK_STYLE=2
# Particle Explosion Transition Effect: 0=OFF, 1=ON
TRANSITION_ENABLE=1
# Clock Color Palette (0=White, 1=Red, 2=Green, 3=Blue, 4=Yellow, 5=Cyan, 6=Magenta, 7=Orange, 8=Pink)
CLOCK_COLOR=4

[WEATHER]
# Enable Weather Features: 0=OFF, 1=ON (Requires WiFi)
WEATHER_ENABLE=1
# Target City (No spaces, use '+' for multi-word cities: London,UK or New+York,US)
CITY=Madrid,ES
# Free OpenWeatherMap API Key
API_KEY=xxxxxxxxxxxxxxxxxxxxxxx
# Update Interval in MINUTES
WEATHER_INT=60
# Custom text string displayed above the clock
WEATHER_MSG=Game Room

[LANGUAGE]
# Language Code (Matches file name in /idioma without .json: ES, EN, FR...)
LANGUAGE=EN

[IR_REMOTE]
# IR Remote Codes (Automatically set and stored via OSD menu)
BTN_ON=F20DFF00
BTN_OFF=E01FFF00
BTN_BRILLO_UP=F609FF00
BTN_BRILLO_DOWN=E21DFF00
BTN_MENU=EA15FF00
BTN_OK=ED12FF00
BTN_SUBIR=E41BFF00
BTN_BAJAR=B34CFF00

[REPLAY_OS]
# ReplayOS Host IP Address
IP=192.168.1.101
# ReplayOS Token: SYSTEM > INFORMATION > NET CONTROL CODE
TOKEN=xxxxxx

[END]
```

### 4. 🌍 Time Zone (TZ) Setup

For accurate **clock** and **timer** operation, the `TZ` environment string must adhere to the POSIX timezone standard format.

- **UK / Ireland / Portugal:** `TZ=GMT0BST,M3.5.0/1,M10.5.0`
- **Central Europe (Spain, France, Germany, Italy):** `TZ=CET-1CEST,M3.5.0,M10.5.0/3`
- **US Eastern Time:** `TZ=EST5EDT,M3.2.0,M11.1.0`
- **US Pacific Time:** `TZ=PST8PDT,M3.2.0,M11.1.0`

👉 Reference **[ESP32 TZ Database](https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv)** to locate the string for your region.

### 5. ☁️ How to Get Your Weather API Key

1. Visit [OpenWeatherMap.org](https://openweathermap.org/) and create a free user account.
2. Navigate to your account profile → **"My API Keys"**.
3. Generate a new key (e.g., named "RetroPixel").
4. **Note:** Newly generated API keys take 30 to 120 minutes to activate globally. If the display shows `0.0C`, allow time for key activation. ⏳
5. Copy the generated key string into `API_KEY=` inside `config.ini`.

**🔍 API Validation Check:** open `http://api.openweathermap.org/data/2.5/weather?q=YOUR_CITY&appid=YOUR_API_KEY` in your browser. A returned JSON payload indicates correct configuration; 401/404 errors indicate pending key activation or incorrect city formatting.

---

## 📖 Playlist Generator (Windows)

The batch utility `Generador de Playlist v1.0.1.bat` automates playlist creation without manually compiling text files. Located inside [Contenido SD](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Contenido%20SD).

1. **Setup:** place the `.bat` file in the root directory of your SD card next to the `/gifs/` folder.
2. **Execution:** double-click to launch the Windows command tool.
3. **Selection:** the utility scans and lists subdirectories inside `/gifs/`. Input selection numbers separated by commas (e.g., `3,4,10`) or type `TODO` for all folders.
4. **Naming:** provide a custom name for the playlist file (e.g., `MyFavorites`).
5. **Output:** generates `playlists/MyFavorites.txt` containing formatted paths.
6. **Playback:** insert the SD card into the panel; it will automatically start playing the first available list. Change active lists via `OSD Menu → Playlists`. 🎞️

<img width="514" height="565" alt="Script PlayList" src="https://github.com/user-attachments/assets/3c600615-5539-4430-af7b-26cd219fc7fe" />

---

## 🕹️ Arcade Integration (Batocera, Recalbox or ReplayOS)

Navigate to `Menu → Playback → Arcade` to set up dynamic game marquee response:

```text
🏠 MAIN MENU
├── 📂 Playback
│   ├── 🖼️ Mode: [GIFs / Clock]
│   ├── 🔀 Shuffle: [YES / NO]
│   ├── 🕹️ Arcade: [OFF / Batocera / Recalbox / ReplayOS]   <-- SELECT HERE
│   └── 🔙 Back
```

> [!IMPORTANT]
> To configure script installation and synchronize ROM metadata, follow the guide for your distribution:
>
> **[👉 Batocera Setup Instructions](https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Ingles/README_BATOCERA.md)**
>
> **[👉 Recalbox Setup Instructions](https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Ingles/README_RECALBOX.md)**
>
> **[👉 ReplayOS Setup Instructions](https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Ingles/README_REPLAYOS.md)**

---

## 🏠 Home Assistant Integration (Complete Guide)

Integrate **Retro Pixel LED Lite** into **Home Assistant** using local REST API endpoints without cloud dependencies.

Features provided:
- 🟢 **Power state control** (`switch`).
- 📊 **Real-time telemetry monitoring** (active playback mode, loaded playlist, IP info).
- 🔄 **On-the-fly mode and playlist selection**.
- 💬 **Live scrolling text broadcast** with selectable speed, custom colors, and **font styles**.

### 📦 Adding Configuration to Home Assistant

If using package-based configuration structures, copy `retropixel.yaml` from the **[Home Assistant Directory](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Home%20Asisstant)** into `/config/packages/`. Alternatively, paste the entries directly into your `configuration.yaml`.

> ⚠️ **Important:** update the placeholder IP address `192.168.31.210` to match your ESP32 display's assigned IP, and update playlist names accordingly.

A ready-to-use Lovelace card file `entities.yaml` is also included.

<img width="822" height="1005" alt="Captura HA" src="https://github.com/user-attachments/assets/9294b479-f428-4c68-9fcc-c871ad2e88e4" />

---

## 🧠 Internal Architecture / Core Lite

Engine design and technical specifications:

- **📡 Dynamic IR Remote Mapping:** user-configurable infrared button mapping saved straight to `config.ini` via the OSD Menu.
- **📂 Wireless FTP Maintenance:** network editing of parameters and playlists without SD card removal.
- **Anti-Panic RAM Guard:** active real-time dynamic heap memory allocation guard. If WiFi allocation prevents DMA double-buffering, it dynamically degrades to single-buffering to avoid system crash events.
- **Binary Search Engine (Arcade):** dynamic binary search index parsing thousands of marquee assets in milliseconds directly off the SD card.
- **Adaptive Memory Management:** uses Double Buffering for high frame rate GIF playback and smoothly transitions to Single Buffering during high-resolution bitmap rendering in Arcade mode.
- **Real-Time HTTP API:** embedded REST request handlers for external synchronization with Batocera, Recalbox, ReplayOS, and Home Assistant.
- **Smart Text Centering Engine:** calculates string pixel length and centers text automatically across panel bounds (`offset + 64px`).
- **WiFi Stealth Mode:** network interfaces activate transiently for NTP time sync and weather fetches before going **100% offline**, achieving **0 ms latency** in rendering pipelines.
- **Dynamic Status Bar:** when weather is active, clock positioning adjusts down (`startY=9`) to display `WEATHER_MSG`, icon graphics, and temperature.
- **Advanced Day/Night Iconography:** pixel art icons (Sun, Moon, Clouds, Rain, Snow, Thunderstorm, Fog) mapped to match local time.
- **Dynamic Playlist Pipeline:** handles custom lists from `/playlists/` on demand through OSD controls.
- **Auto-Interrupt Clock Engine:** suspends GIF execution cycles every *x* GIFs for *x* seconds, resuming playback without losing sequence positions.
- **Offline Fallback Resilience:** bypasses NTP polling on network loss, maintaining clock scheduling using internal chip RTC timers.
- **Double-Buffered DMA Engine:** leverages ESP32 direct memory access capabilities to eliminate flickering artifacts during image transitions.

---

## 📜 Detailed Changelog (v3.0.0 → v3.1.3)

| Feature | Technical Details | Benefit |
| :--- | :--- | :--- |
| **🕹️ RePlayOS Integration** | Native API client integration with the RePlayOS frontend for game marquees and system GIFs. | **Dynamic Arcade Experience.** Automatically updates marquees when changing games in RePlayOS. |
| **⏰ New Clock Styles** | Added new clock rendering options and visual themes. | **Enhanced Customization** matching different preferences and environments. |
| **📡 IR Remote Mapping via PWA** | PWA interface to capture, map, and store remote control button codes to `config.ini`. | **Intuitive Remote Setup** now manageable via web interface. |
| **🏠 Home Assistant Integration** | Exposed REST API endpoints (`GET /status`, `POST /control`, `/playlist`, `/texto`, `/timer/toggle`) and complete YAML integration package. | **Home Automation Control.** Power control, mode selection, playlists, and ticker text directly from HA. |
| **🔤 Font Selector for Text** | 4 selectable typography fonts (`Bold`, `SemiBold`, `Regular`, `Light`) via HTTP endpoints, PWA, and REST API. | **Visual Customization** for scrolling ticker text. |
| **🎛️ PWA Control Panel** | Progressive Web App with 5 control modules and full remote `config.ini` editing. | **Full Control from Any Device** without external server requirements. |
| **☀️ Real-time Brightness** | Live 0-100% slider with instant hardware application. | **Smooth Adaptation** to ambient lighting conditions. |
| **🔤 UTF-8 Ticker Text** | On-the-fly UTF-8 to Latin-1 decoder supporting extended international characters. | Full support for accented characters and special symbols. |
| **🎨 Mode Switcher** | Seamless mode switching (GIF / Clock / Text) via PWA without system reboots. | Instant visual transitions. |
| **🎞️ Dynamic Playlists** | Hot-swappable active playlists via PWA interface. | Switch collections seamlessly during operation. |
| **⏰ Smart Schedule Timer** | Programmable power schedules with physical button or PWA manual overrides. | Full automated power management. |
| **🔄 Remote Updates (OTA & Languages)** | Over-the-air firmware and translation file downloads from GitHub without extracting the SD. | Completely wireless system maintenance. |
| **⚙️ Full Remote Configuration** | Remote config editor for WiFi, hardware, playback, weather, and language settings. | Total wireless setup flexibility. |
| **💬 Ticker Text Interface** | HTTP POST endpoints + PWA for custom text messaging, color selection, and scroll speeds. | Broadcast real-time messages on demand. |
| **💥 Particle Transitions** | Particle explosion engine for clock entry and exit transitions. | Fluid visual flow without static cuts. |
| **🎨 OSD Color Customization** | Interactive menu with IR mapping and persistent storage to EEPROM/SD. | Change clock colors on the fly without editing `config.ini`. |
| **⚡ Flicker-Free Clock** | Optimized *Single Buffer* clock rendering routines. | Zero flickering during fast screen updates. |
| **🧠 Dynamic Heap Optimization** | Converted `String` variables to `char[]` and extended `PSTR()` / `F()` macro usage. | Zero heap fragmentation; maximizes RAM allocation for double-buffered DMA rendering. |
| **🛡️ Anti-Panic System** | Automatic evaluation of `display->begin()` with single-buffer fallback. | Prevents system crashes (`StoreProhibited`) caused by memory allocation loss after WiFi activity. |
| **🖱️ Long Press Logic** | Contextual long-press confirmation timers. | Prevents accidental menu selections. |
| **📂 Integrated FTP Server** | Direct wireless file access to the SD card. | Edit playlists, `.ini`, and `.json` files without removing the card. |
| **📡 IR Remote Navigation** | Dynamic function mapping for wireless navigation. | Controls brightness, power state, and menu navigation from a remote. |
| **🎨 Color Order Setting** | Dynamic `colorOrder` configuration (RGB/RBG/GBR) via `config.ini`. | Broad compatibility across HUB75 matrix hardware without recompiling code. |

---

## 🛒 Bill of Materials

Hardware components tested during development:

- **Microcontroller:** [ESP32 DevKit V1 (30 pins) - AliExpress](https://es.aliexpress.com/item/1005005704190069.html)
- **LED Matrix Panel (HUB75):** [P2.5 / P4 RGB Matrix Panel - AliExpress](https://es.aliexpress.com/item/1005008479388445.html)
- **SD Card Module:** [Micro SD Card Adapter Module (SPI) - AliExpress](https://es.aliexpress.com/item/1005005591145849.html)
- **ESP32 to Panel Shield:** [DMDos Board V3 - Mortaca](https://www.mortaca.com/) *(optional, solderless solution with built-in SD slot)*
- **IR Receiver:** [Universal Infrared Receiver Sensor - AliExpress](https://es.aliexpress.com/item/1005005343424296.html)
- **Push Button:** [DS-316 Momentary Push Button Switch - AliExpress](https://es.aliexpress.com/item/4000888761296.html)
- **Power Supply:** 5V DC power supply (minimum 2A recommended for 64×32 LED panels).

---

## 🔌 Pinout & Wiring

If using the **DMDos Board V3**, wiring is pre-routed — skip to the next section.

#### 📂 Micro SD Card Reader (SPI Interface)
| SD Pin | ESP32 Pin | Function |
| :--- | :--- | :--- |
| **CS** | GPIO 5 | Chip Select |
| **CLK** | GPIO 18 | Clock |
| **MOSI** | GPIO 23 | Master Out Slave In |
| **MISO** | GPIO 19 | Master In Slave Out |
| **VCC** | 3.3V | Power Supply |
| **GND** | GND | Ground |

#### 🖼️ RGB LED Matrix Panel (HUB75 Interface)
| Panel Pin | ESP32 Pin | Function |
| :--- | :--- | :--- |
| **R1** | GPIO 25 | Red Data (Top Half) |
| **G1** | GPIO 26 | Green Data (Top Half) |
| **B1** | GPIO 27 | Blue Data (Top Half) |
| **R2** | GPIO 14 | Red Data (Bottom Half) |
| **G2** | GPIO 12 | Green Data (Bottom Half) |
| **B2** | GPIO 13 | Blue Data (Bottom Half) |
| **A** | GPIO 33 | Row Address Line A |
| **B** | GPIO 32 | Row Address Line B |
| **C** | GPIO 22 | Row Address Line C |
| **D** | GPIO 17 | Row Address Line D |
| **E** | GND | Ground |
| **CLK** | GPIO 16 | Clock |
| **LAT** | GPIO 4 | Latch |
| **OE** | GPIO 15 | Output Enable (Brightness Control) |

#### 🕹️ Control Interfaces (Push Button & Infrared)
| Component | ESP32 Pin | Function |
| :--- | :--- | :--- |
| **Button (PIN)** | GPIO 21 | **Multifunction:** Short Click (Navigate) / Long Press (Select - Power Toggle). |
| **Button (GND)** | GND | Ground Reference. |
| **IR Receiver (Data)** | GPIO 34 | Signal Input (NEC Protocol). |
| **IR Receiver (VCC)** | 3.3V | Module Power Supply. |
| **IR Receiver (GND)** | GND | Ground Reference. |

<img width="769" height="716" alt="image" src="https://github.com/user-attachments/assets/11fef006-59f3-405f-b00a-a32c9bba7bc5" />

---

### 📂 SD Explorer (FTP) — Setup Details

This feature enables an embedded wireless FTP server to manage files on the SD card over your local network without removing the card.

> [!IMPORTANT]
> **Recommended usage:** updating `config.ini`, translation packs (`.json`), playlist files (`.txt`), and small assets. Due to ESP32 network throughput limitations, **it is not recommended for transferring entire GIF libraries**.

**Activating the FTP Server:**
1. Open `OSD Menu → SD Explorer`.
2. Select **Start FTP**.
3. The display pauses GIF rendering and outputs its assigned **IP address** (e.g., `192.168.1.109`).

**FileZilla Connection Settings:**
- **Protocol:** Plain FTP (Insecure).
- **Host:** IP address displayed on the panel.
- **Logon Type:** Normal.
- **User / Password:** `admin` / `admin`.
- **Port:** `21`.
- **Max Connections:** set to 1.

<img width="545" height="227" alt="image" src="https://github.com/user-attachments/assets/1b537615-3e39-48ba-9eb0-48b03931c5f9" />
<img width="544" height="193" alt="image" src="https://github.com/user-attachments/assets/ba4c85bc-920a-48c9-83d8-99b96ecbc57f" />

**In FileZilla Options → Transfers:**
- Maximum simultaneous transfers: 1.
- Speed limits enabled: 20 KiB/s download and upload.

<img width="841" height="522" alt="image" src="https://github.com/user-attachments/assets/e90d3e84-9c93-45c0-b942-8b601db40041" />

**Alternative (Windows File Explorer)** *(Not recommended due to potential transfer drops)*:
1. Open **This PC** → right-click → **"Add a network location"**.
2. Enter address: `ftp://<PANEL_IP>` (e.g., `ftp://192.168.1.109`).
3. Uncheck "Log on anonymously" and enter username `admin`.
4. Provide a label for the network drive.

**Important Operational Notes:**
- GIF playback remains suspended while the FTP server is active.
- To exit FTP mode, press the hardware button or the "OK" key on your remote.
- Do not disconnect power while writing files via FTP to prevent file corruption.

---

## 🛠️ Roadmap

### ⚡ Performance & Core Features

*(TBD)*

### 🎨 Visual & Connectivity Enhancements

*(TBD)*

---

## ⚖️ License & Acknowledgments

This project is open-source under the **MIT License**.

Special thanks to the authors and maintainers of the underlying libraries:
- **Bitbank2** for the `AnimatedGIF` library.
- **Mrfaptastic** for the high-performance ESP32 DMA HUB75 Matrix engine.
- **DMDos Telegram Community**, whose hardware showcases inspired the development of **Retro Pixel LED**.
- **RpiTe@m** for sharing the free 600 GIF starter pack and their 11,000 GIF collection available [here](https://www.neo-arcadia.com/forum/viewtopic.php?t=67065).
- **shan-aya** for the French translation and the [DMD_GIF_converter](https://github.com/shan-aya/DMD_GIF_converter) utility.
- **joseAveleira** for the clock particle animation effect. [GitHub](https://github.com/joseAveleira/RelojPixel/tree/main)

---

<p align="center">Made with ❤️ and plenty of retro GIFs. If you like this project, consider giving the repository a ⭐!</p>

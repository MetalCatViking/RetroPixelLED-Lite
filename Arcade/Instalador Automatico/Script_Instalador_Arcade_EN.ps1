# ==================================================================
#   AUTOMATIC ARCADE INSTALLER - RETRO PIXEL LED (v3.2)
# ==================================================================
$Host.UI.RawUI.WindowTitle = "Retro Pixel Universal Installer v3.2"
$OutputEncoding = [System.Text.Encoding]::UTF8

Write-Host "===================================================" -ForegroundColor Magenta
Write-Host "      RETRO PIXEL LED Lite ARCADE INSTALLER" -ForegroundColor White
Write-Host "===================================================" -ForegroundColor Magenta

# 1. REQUEST THE PANEL IP (shared by all three systems)
$IP_PANEL = Read-Host "1. Enter your LED panel IP (e.g. 192.168.1.117)"
$IP_PANEL = $IP_PANEL.Trim()

# 2. OPERATING SYSTEM SELECTION
# Ask this BEFORE requesting a path, because the kind of path depends on the answer:
# a network path/IP for Batocera and Recalbox, or a
# LOCAL folder on this PC for RetroBat.
Write-Host ""
Write-Host "2. Which system do you want to configure?" -ForegroundColor Cyan
Write-Host "  1) Batocera" -ForegroundColor White
Write-Host "  2) Recalbox" -ForegroundColor White
Write-Host "  3) RetroBat (Windows)" -ForegroundColor White
$opcion = Read-Host "Select an option (1, 2, or 3)"

# --- PATH DETECTION (only for Batocera/Recalbox, which are network-based) ---
$RUTA_SYSTEM = ""
$RUTA_USERSCRIPTS = ""
$RUTA_BATOCERA_SCRIPTS = ""

if ($opcion -eq "1" -or $opcion -eq "2") {
    $INPUT_RUTA = Read-Host "Enter the console path or IP (e.g. \\192.168.1.119 or D:)"
    $INPUT_RUTA = $INPUT_RUTA.Trim([char]34).TrimEnd([char]92)

    if (Test-Path -LiteralPath (Join-Path $INPUT_RUTA "share\system")) {
        $RUTA_SYSTEM = Join-Path $INPUT_RUTA "share\system"
        $RUTA_USERSCRIPTS = Join-Path $INPUT_RUTA "share\userscripts"
        $RUTA_BATOCERA_SCRIPTS = Join-Path $INPUT_RUTA "share\system\configs\emulationstation\scripts"
    } elseif (Test-Path -LiteralPath (Join-Path $INPUT_RUTA "recalbox\system")) {
        $RUTA_SYSTEM = Join-Path $INPUT_RUTA "recalbox\system"
        $RUTA_USERSCRIPTS = Join-Path $INPUT_RUTA "recalbox\userscripts"
    } elseif (Test-Path -LiteralPath (Join-Path $INPUT_RUTA "system")) {
        $RUTA_SYSTEM = Join-Path $INPUT_RUTA "system"
        $RUTA_BATOCERA_SCRIPTS = Join-Path $INPUT_RUTA "system\configs\emulationstation\scripts"
        if (Test-Path -LiteralPath (Join-Path $INPUT_RUTA "userscripts")) {
            $RUTA_USERSCRIPTS = Join-Path $INPUT_RUTA "userscripts"
        } else {
            $RUTA_USERSCRIPTS = Join-Path (Split-Path $INPUT_RUTA -Parent) "userscripts"
        }
    } else {
        Write-Host ""
        Write-Host "[ERROR] Could not find the console folder structure." -ForegroundColor Red
        Read-Host "Press Enter to exit"
        exit
    }
}

# --- INTERNAL UNIX SCRIPT INSTALLER (Batocera/Recalbox) ---
# $SubCarpeta: LOCAL subfolder (next to this .ps1) containing the source script,
# e.g. "Batocera" or "Recalbox". This does not affect the installed filename on the console.
function Instalar-Script-Unix($FileName, $DestinoPath, $SubCarpeta = "") {
    $CurrentDir = $PSScriptRoot
    if ([string]::IsNullOrEmpty($CurrentDir)) { $CurrentDir = Get-Location }

    if ([string]::IsNullOrEmpty($SubCarpeta)) {
        $PathOrigen = Join-Path $CurrentDir $FileName
    } else {
        $PathOrigen = Join-Path (Join-Path $CurrentDir $SubCarpeta) $FileName
    }

    # FIX: Use -LiteralPath so square brackets [ ] are not interpreted as wildcard patterns.
    if (Test-Path -LiteralPath $PathOrigen) {
        if (!(Test-Path -LiteralPath $DestinoPath)) {
            $null = New-Item -ItemType Directory -Force -Path $DestinoPath
        }

        # FIX: Get-Content also needs -LiteralPath for filenames containing square brackets.
        $Contenido = Get-Content -LiteralPath $PathOrigen -Raw

        # Safely inject the IP using direct ASCII characters.
        $Quote = [char]34
        $StringReemplazo = "IP_ESP32=" + $Quote + $IP_PANEL + $Quote
        $Contenido = $Contenido -replace ("IP_ESP32=" + $Quote + ".*" + $Quote), $StringReemplazo

        # Force Linux line endings (LF).
        $ContenidoFinal = $Contenido -replace "`r`n", "`n"
        $PathFinal = Join-Path $DestinoPath $FileName

        $Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
        [System.IO.File]::WriteAllText($PathFinal, $ContenidoFinal, $Utf8NoBom)

        Write-Host "   [OK] Configured and installed: $FileName" -ForegroundColor Green
        return $true
    } else {
        Write-Host ""
        Write-Host "   [ERROR] Could not find $FileName in the '$SubCarpeta' folder next to the installer." -ForegroundColor Red
        Write-Host "   Make sure the .sh/.py files are in the Batocera\ or Recalbox\ subfolders." -ForegroundColor Yellow
        return $false
    }
}

# --- INTERNAL WINDOWS SCRIPT INSTALLER (RetroBat) ---
# Same pattern as Instalar-Script-Unix, with two differences: line endings are not forced
# (Windows .py/.bat files are preserved as they are), and
# if $ArcadeRootValue is provided, the ARCADE_ROOT line is also replaced
# along with IP_ESP32 (used by retrobat_marquesina_start.py/stop.py).
function Instalar-Script-Windows($FileName, $DestinoPath, $SubCarpeta = "", $ArcadeRootValue = $null) {
    $CurrentDir = $PSScriptRoot
    if ([string]::IsNullOrEmpty($CurrentDir)) { $CurrentDir = Get-Location }

    if ([string]::IsNullOrEmpty($SubCarpeta)) {
        $PathOrigen = Join-Path $CurrentDir $FileName
    } else {
        $PathOrigen = Join-Path (Join-Path $CurrentDir $SubCarpeta) $FileName
    }

    if (-not (Test-Path -LiteralPath $PathOrigen)) {
        Write-Host ""
        Write-Host "   [ERROR] Could not find $FileName in the '$SubCarpeta' folder next to the installer." -ForegroundColor Red
        return $false
    }

    if (-not (Test-Path -LiteralPath $DestinoPath)) {
        $null = New-Item -ItemType Directory -Force -Path $DestinoPath
    }

    $Contenido = Get-Content -LiteralPath $PathOrigen -Raw
    $Quote = [char]34

    $StringReemplazoIP = "IP_ESP32=" + $Quote + $IP_PANEL + $Quote
    $Contenido = $Contenido -replace ("IP_ESP32=" + $Quote + ".*" + $Quote), $StringReemplazoIP

    if ($ArcadeRootValue) {
        $StringReemplazoArcade = "ARCADE_ROOT = r" + $Quote + $ArcadeRootValue + $Quote
        $Contenido = $Contenido -replace ("ARCADE_ROOT = r" + $Quote + ".*" + $Quote), $StringReemplazoArcade
    }

    $PathFinal = Join-Path $DestinoPath $FileName
    $Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($PathFinal, $Contenido, $Utf8NoBom)

    Write-Host "   [OK] Configured and installed: $FileName" -ForegroundColor Green
    return $true
}

# --- FUNCTION: check/install a Windows dependency using winget ---
function Asegurar-Dependencia-Windows($Comando, $WingetId, $Nombre, $UrlManual) {
    $existe = Get-Command $Comando -ErrorAction SilentlyContinue
    if ($existe) {
        Write-Host "   [OK] $Nombre is already installed." -ForegroundColor Green
        return $true
    }

    Write-Host "   $Nombre was not found in PATH. Attempting to install it with winget..." -ForegroundColor Yellow
    $winget = Get-Command winget -ErrorAction SilentlyContinue
    if (-not $winget) {
        Write-Host "   [ERROR] winget is not available on this computer." -ForegroundColor Red
        Write-Host "   Install $Nombre manually from $UrlManual, then run this installer again." -ForegroundColor Yellow
        return $false
    }

    try {
        winget install --id $WingetId -e --silent --accept-package-agreements --accept-source-agreements | Out-Null
    } catch {
        Write-Host "   [ERROR] Automatic installation of $Nombre failed." -ForegroundColor Red
        Write-Host "   Install it manually from $UrlManual, then run this installer again." -ForegroundColor Yellow
        return $false
    }

    # winget may have updated the machine/user PATH without this
    # PowerShell session noticing yet; refresh it so we can
    # detect the command without closing and reopening the window.
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")

    $existeAhora = Get-Command $Comando -ErrorAction SilentlyContinue
    if ($existeAhora) {
        Write-Host "   [OK] $Nombre was installed successfully." -ForegroundColor Green
        return $true
    }

    Write-Host "   [WARNING] $Nombre was installed but is not yet detected in this PowerShell session." -ForegroundColor Yellow
    Write-Host "   Close this window, open a new one, and run this installer again to confirm." -ForegroundColor Yellow
    return $false
}

# 3. EXECUTION FILTERED BY SYSTEM
switch ($opcion) {
    "1" {
        Write-Host ""
        Write-Host "--> Installing on BATOCERA..." -ForegroundColor Cyan

        Write-Host ""
        Write-Host "Which operating mode do you want to enable?" -ForegroundColor Cyan
        Write-Host "  Option 1: Menus and Games (show systems while browsing and the launched game)" -ForegroundColor White
        Write-Host "  Option 2: Games Only (static marquee/clock in menus; changes only during gameplay)" -ForegroundColor White
        $modoBatocera = Read-Host "Select an option (1 or 2)"

        if ($modoBatocera -ne "1" -and $modoBatocera -ne "2") {
            Write-Host ""
            Write-Host "[ERROR] Invalid mode option." -ForegroundColor Red
            Read-Host "Press Enter to exit"
            exit
        }

        $RUTA_GAME_START = Join-Path $RUTA_BATOCERA_SCRIPTS "game-start"
        $RUTA_GAME_SELECTED = Join-Path $RUTA_BATOCERA_SCRIPTS "game-selected"
        $RUTA_SYSTEM_SELECTED = Join-Path $RUTA_BATOCERA_SCRIPTS "system-selected"
        $RUTA_GAME_END = Join-Path $RUTA_BATOCERA_SCRIPTS "game-end"
        $RUTA_QUIT = Join-Path $RUTA_BATOCERA_SCRIPTS "quit"
        $RUTA_SHUTDOWN = Join-Path $RUTA_BATOCERA_SCRIPTS "shutdown"
        $RUTA_REBOOT = Join-Path $RUTA_BATOCERA_SCRIPTS "reboot"

        # The static-image decoder (Python) and animated GIF streamer (FFmpeg)
        # form the shared engine; they are installed once in userscripts
        # (equivalent to /userdata/userscripts on Batocera), and the hooks call them.
        $inst_sender = Instalar-Script-Unix "Batocera_marquesina.py" $RUTA_USERSCRIPTS "Batocera"
        $inst_streamer = Instalar-Script-Unix "pixel_stream.py" $RUTA_USERSCRIPTS "Batocera"

        # Hooks shared by both modes: game launch (animated GIF), game end,
        # and system shutdown/reboot.
        $inst_gamestart = Instalar-Script-Unix "Batocera_game-start.sh" $RUTA_GAME_START "Batocera"
        $inst_gameselected = Instalar-Script-Unix "Batocera_game-selected.sh" $RUTA_GAME_SELECTED "Batocera"
        $inst_gameend = Instalar-Script-Unix "Batocera_game-end.sh" $RUTA_GAME_END "Batocera"
        $inst_quit = Instalar-Script-Unix "Batocera_quit.sh" $RUTA_QUIT "Batocera"
        $inst_shutdown = Instalar-Script-Unix "Batocera_shutdown.sh" $RUTA_SHUTDOWN "Batocera"
        $inst_reboot = Instalar-Script-Unix "Batocera_reboot.sh" $RUTA_REBOOT "Batocera"

        # system-selected is installed only in "Menus and Games" mode.
        $inst_systemselected = $false
        if ($modoBatocera -eq "1") {
            Write-Host ""
            Write-Host " 'Menus and Games' mode: installing system-selected..." -ForegroundColor Yellow
            $inst_systemselected = Instalar-Script-Unix "Batocera_system-selected.sh" $RUTA_SYSTEM_SELECTED "Batocera"
        } else {
            Write-Host ""
            Write-Host " 'Games Only' mode: system-selected will not be installed." -ForegroundColor Yellow
            $conflictivo = Join-Path $RUTA_SYSTEM_SELECTED "Batocera_system-selected.sh"
            if (Test-Path -LiteralPath $conflictivo) {
                Remove-Item -LiteralPath $conflictivo -Force
                Write-Host "   [OK] Removed system-selected left by an earlier 'Menus and Games' installation." -ForegroundColor Green
            }
        }

        if ($inst_gamestart -or $inst_gameselected -or $inst_systemselected -or $inst_gameend -or $inst_quit -or $inst_shutdown -or $inst_reboot) {
            Write-Host " Configuring automatic permissions on Batocera (native service)..." -ForegroundColor Yellow

            # TECHNICAL NOTE: custom.sh is IGNORED starting with Batocera v44, and its
            # execution over SMB is not always reliable on recent versions (including v43.x).
            # Since v38, the officially supported method is
            # "user services" in /userdata/system/services/, which are enabled once
            # from the EmulationStation menu and persist across reboots
            # and reinstalls of this script.
            $RUTA_SERVICES = Join-Path $RUTA_SYSTEM "services"
            if (!(Test-Path -LiteralPath $RUTA_SERVICES)) {
                $null = New-Item -ItemType Directory -Force -Path $RUTA_SERVICES
            }

            $NombreServicio = "retropixelperms"
            $PathServicio = Join-Path $RUTA_SERVICES $NombreServicio

            $LineasServicio = @(
                "#!/bin/bash",
                "# Servicio RetroPixel: otorga permisos de ejecucion a los scripts de marquesinas.",
                "# Gestionado por Script_Instalador_Arcade.ps1 - no editar a mano.",
                'case "$1" in',
                "    start)",
                "        chmod -R +x /userdata/system/configs/emulationstation/scripts/game-start/ 2>/dev/null",
                "        chmod -R +x /userdata/system/configs/emulationstation/scripts/game-selected/ 2>/dev/null",
                "        chmod -R +x /userdata/system/configs/emulationstation/scripts/system-selected/ 2>/dev/null",
                "        chmod -R +x /userdata/system/configs/emulationstation/scripts/game-end/ 2>/dev/null",
                "        chmod -R +x /userdata/system/configs/emulationstation/scripts/quit/ 2>/dev/null",
                "        chmod -R +x /userdata/system/configs/emulationstation/scripts/shutdown/ 2>/dev/null",
                "        chmod -R +x /userdata/system/configs/emulationstation/scripts/reboot/ 2>/dev/null",
                "        chmod -R +x /userdata/userscripts 2>/dev/null",
                "        ;;",
                "    stop)",
                "        ;;",
                "    *)",
                '        echo "Usage: $0 {start|stop}"',
                "        ;;",
                "esac",
                "exit 0"
            )
            $ServicioSH = $LineasServicio -join "`n"

            $Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
            [System.IO.File]::WriteAllText($PathServicio, $ServicioSH, $Utf8NoBom)
            Write-Host "   [OK] Service '$NombreServicio' created successfully." -ForegroundColor Green

            # Cleanup/migration: if an earlier installation left a custom.sh
            # containing RetroPixel chmod lines, remove them to avoid
            # duplicating the mechanism. If the file is left with no useful
            # content, delete it; if the user had other custom content in
            # custom.sh, preserve that content unchanged.
            $PathCustom = Join-Path $RUTA_SYSTEM "custom.sh"
            if (Test-Path -LiteralPath $PathCustom) {
                $ContenidoCustom = Get-Content -LiteralPath $PathCustom
                $LineasFiltradas = $ContenidoCustom | Where-Object {
                    $_ -notmatch "Otorga permisos a los scripts de marquesinas RetroPixel" -and
                    $_ -notmatch "scripts/(game-start|game-selected|system-selected|game-end|quit|shutdown|reboot)/" -and
                    $_ -notmatch "chmod -R \+x /userdata/userscripts"
                }
                $RestoUtil = $LineasFiltradas | Where-Object { $_.Trim() -ne "" -and $_.Trim() -ne "#!/bin/bash" }
                if (-not $RestoUtil) {
                    Remove-Item -LiteralPath $PathCustom -Force
                    Write-Host "   [OK] Old custom.sh (RetroPixel permissions only) removed and replaced by the service." -ForegroundColor Green
                } elseif ($LineasFiltradas.Count -ne $ContenidoCustom.Count) {
                    $NuevoCustom = ($LineasFiltradas -join "`n")
                    [System.IO.File]::WriteAllText($PathCustom, $NuevoCustom, $Utf8NoBom)
                    Write-Host "   [OK] Migrated custom.sh: removed RetroPixel permission commands (now handled by the service) and preserved your other content." -ForegroundColor Green
                }
            }

            Write-Host ""
            Write-Host "   [IMPORTANT] One-time manual step:" -ForegroundColor Yellow
            Write-Host "   After rebooting, open Batocera: MAIN MENU > SYSTEM SETTINGS > SERVICES" -ForegroundColor Yellow
            Write-Host "   and enable 'retropixelperms'. It stays enabled permanently, including after" -ForegroundColor Yellow
            Write-Host "   reinstalling this script; you will not need to configure it over SSH again." -ForegroundColor Yellow
        }
    }

    "2" {
        Write-Host ""
        Write-Host "--> Configuring RECALBOX..." -ForegroundColor Cyan

        Write-Host ""
        Write-Host "Which operating mode do you want to enable?" -ForegroundColor Cyan
        Write-Host "  Option 1: Menus and Games (show systems while browsing and the launched game)" -ForegroundColor White
        Write-Host "  Option 2: Games Only (static marquee/clock in menus; changes only during gameplay)" -ForegroundColor White
        $modoRecalbox = Read-Host "Select an option (1 or 2)"

        # The animated GIF streamer (FFmpeg) is shared by both modes.
        $null = Instalar-Script-Unix "pixel_stream.py" $RUTA_USERSCRIPTS "Recalbox"

        if ($modoRecalbox -eq "1") {
            Write-Host ""
            Write-Host " Installing Option 1 (Permanent)..." -ForegroundColor Yellow
            $null = Instalar-Script-Unix "Recalbox_1(permanent).sh" $RUTA_USERSCRIPTS "Recalbox"

            $conflictivo = Join-Path $RUTA_USERSCRIPTS "Recalbox_2(permanent).sh"
            if (Test-Path -LiteralPath $conflictivo) { Remove-Item -LiteralPath $conflictivo -Force }
        }
        elseif ($modoRecalbox -eq "2") {
            Write-Host ""
            Write-Host " Installing Option 2 (Event-based)..." -ForegroundColor Yellow
            $null = Instalar-Script-Unix "Recalbox_2(permanent).sh" $RUTA_USERSCRIPTS "Recalbox"

            $conflictivo = Join-Path $RUTA_USERSCRIPTS "Recalbox_1(permanent).sh"
            if (Test-Path -LiteralPath $conflictivo) { Remove-Item -LiteralPath $conflictivo -Force }
        }
        else {
            Write-Host ""
            Write-Host "[ERROR] Invalid mode option." -ForegroundColor Red
            Read-Host "Press Enter to exit"
            exit
        }
    }

    "3" {
        Write-Host ""
        Write-Host "--> Installing on RETROBAT (Windows)..." -ForegroundColor Cyan

        $RUTA_RETROBAT = Read-Host "RetroBat installation folder (press Enter to use: C:\RetroBat)"
        if ([string]::IsNullOrWhiteSpace($RUTA_RETROBAT)) { $RUTA_RETROBAT = "C:\RetroBat" }
        $RUTA_RETROBAT = $RUTA_RETROBAT.Trim([char]34).TrimEnd([char]92)

        $RUTA_ES_SCRIPTS = Join-Path $RUTA_RETROBAT "emulationstation\.emulationstation\scripts"
        if (-not (Test-Path -LiteralPath $RUTA_ES_SCRIPTS)) {
            Write-Host ""
            Write-Host "[ERROR] Could not find the EmulationStation scripts folder at that path." -ForegroundColor Red
            Write-Host "Expected location: $RUTA_ES_SCRIPTS" -ForegroundColor Yellow
            Read-Host "Press Enter to exit"
            exit
        }

        $ARCADE_ROOT_RETROBAT = Read-Host "Local Arcade marquee folder (press Enter to use: C:\RetroPixelLED\Arcade)"
        if ([string]::IsNullOrWhiteSpace($ARCADE_ROOT_RETROBAT)) { $ARCADE_ROOT_RETROBAT = "C:\RetroPixelLED\Arcade" }
        $ARCADE_ROOT_RETROBAT = $ARCADE_ROOT_RETROBAT.Trim([char]34).TrimEnd([char]92)

        Write-Host ""
        Write-Host "Checking dependencies (Python and FFmpeg)..." -ForegroundColor Cyan
        $tienePython = Asegurar-Dependencia-Windows "python" "Python.Python.3.12" "Python" "https://www.python.org/downloads/"
        $tieneFFmpeg = Asegurar-Dependencia-Windows "ffmpeg" "Gyan.FFmpeg" "FFmpeg" "https://ffmpeg.org/download.html"

        $RUTA_ENGINE = Join-Path $RUTA_ES_SCRIPTS "_engine"
        $RUTA_GAME_START = Join-Path $RUTA_ES_SCRIPTS "game-start"
        $RUTA_GAME_END = Join-Path $RUTA_ES_SCRIPTS "game-end"

        Write-Host ""
        Write-Host "Installing the engine (Python)..." -ForegroundColor Yellow
        $inst_streamer = Instalar-Script-Windows "pixel_stream_retrobat.py" $RUTA_ENGINE "RetroBat"
        $inst_start = Instalar-Script-Windows "retrobat_marquesina_start.py" $RUTA_ENGINE "RetroBat" $ARCADE_ROOT_RETROBAT
        $inst_stop = Instalar-Script-Windows "retrobat_marquesina_stop.py" $RUTA_ENGINE "RetroBat"

        if ($inst_streamer -and $inst_start -and $inst_stop) {
            Write-Host ""
            Write-Host "Generating the game-start/game-end hooks..." -ForegroundColor Yellow

            if (-not (Test-Path -LiteralPath $RUTA_GAME_START)) { $null = New-Item -ItemType Directory -Force -Path $RUTA_GAME_START }
            if (-not (Test-Path -LiteralPath $RUTA_GAME_END)) { $null = New-Item -ItemType Directory -Force -Path $RUTA_GAME_END }

            $RutaStartPy = Join-Path $RUTA_ENGINE "retrobat_marquesina_start.py"
            $RutaStopPy = Join-Path $RUTA_ENGINE "retrobat_marquesina_stop.py"

            $BatInicio = @(
                "@echo off",
                ('start "" /B pythonw "' + $RutaStartPy + '" "%~1" "%~2"')
            ) -join "`r`n"

            $BatFin = @(
                "@echo off",
                ('start "" /B pythonw "' + $RutaStopPy + '"')
            ) -join "`r`n"

            $Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
            [System.IO.File]::WriteAllText((Join-Path $RUTA_GAME_START "marquesina_iniciar.bat"), $BatInicio, $Utf8NoBom)
            [System.IO.File]::WriteAllText((Join-Path $RUTA_GAME_END "marquesina_detener.bat"), $BatFin, $Utf8NoBom)

            Write-Host "   [OK] marquesina_iniciar.bat installed in game-start\" -ForegroundColor Green
            Write-Host "   [OK] marquesina_detener.bat installed in game-end\" -ForegroundColor Green
        }

        if (-not $tienePython -or -not $tieneFFmpeg) {
            Write-Host ""
            Write-Host "[WARNING] Could not confirm that one or more dependencies were installed (see messages above)." -ForegroundColor Yellow
            Write-Host "The scripts have been copied, but they will not work until Python and FFmpeg" -ForegroundColor Yellow
            Write-Host "are available in this PC's PATH." -ForegroundColor Yellow
        }

        Write-Host ""
        Write-Host "[IMPORTANT] If your version of RetroBat requires a setting to be enabled for" -ForegroundColor Yellow
        Write-Host "EmulationStation to run these scripts (such as CustomEventScripts), check:" -ForegroundColor Yellow
        Write-Host "  $RUTA_RETROBAT\emulationstation\.emulationstation\es_settings.cfg" -ForegroundColor Yellow
    }

    default {
        Write-Host ""
        Write-Host "[ERROR] Invalid system option." -ForegroundColor Red
        Read-Host "Press Enter to exit"
        exit
    }
}

Write-Host ""
Write-Host "=================================================" -ForegroundColor Magenta
Write-Host "             INSTALLATION COMPLETED!" -ForegroundColor Green
Write-Host "=================================================" -ForegroundColor Magenta
if ($opcion -eq "1") {
    Write-Host "IMPORTANT: Reboot Batocera and, the first time only, enable 'retropixelperms' in"
    Write-Host "SYSTEM SETTINGS > SERVICES so permissions are applied automatically."
} elseif ($opcion -eq "2") {
    Write-Host "IMPORTANT: Reboot Recalbox."
} else {
    Write-Host "IMPORTANT: Restart RetroBat (or at least close and reopen EmulationStation)"
    Write-Host "so the game-start/game-end hooks become active."
}
Write-Host "--------------------------------------------------"
Read-Host "Press Enter to finish"

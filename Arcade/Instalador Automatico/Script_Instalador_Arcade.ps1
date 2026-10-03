# ==================================================================
#   INSTALADOR AUTOMATICO ARCADE - RETRO PIXEL LED (v3.2)
# ==================================================================
$Host.UI.RawUI.WindowTitle = "Instalador Retro Pixel Universal v3.2"
$OutputEncoding = [System.Text.Encoding]::UTF8

Write-Host "===================================================" -ForegroundColor Magenta
Write-Host "      INSTALADOR ARCADE RETRO PIXEL LED Lite" -ForegroundColor White
Write-Host "===================================================" -ForegroundColor Magenta

# 1. SOLICITAR IP DEL PANEL (comun a los tres sistemas)
$IP_PANEL = Read-Host "1. Introduce la IP de tu PANEL LED (ej. 192.168.1.117)"
$IP_PANEL = $IP_PANEL.Trim()

# 2. SELECCION DE SISTEMA OPERATIVO
# Se pregunta ANTES de pedir ninguna ruta, porque el tipo de ruta a solicitar
# depende de la respuesta: ruta/IP de red para Batocera y Recalbox, carpeta
# LOCAL de este mismo PC para RetroBat.
Write-Host ""
Write-Host "2. Que sistema vas a configurar?" -ForegroundColor Cyan
Write-Host "  1) Batocera" -ForegroundColor White
Write-Host "  2) Recalbox" -ForegroundColor White
Write-Host "  3) RetroBat (Windows)" -ForegroundColor White
$opcion = Read-Host "Selecciona una opcion (1, 2 o 3)"

# --- DETECCION DE RUTAS (solo para Batocera/Recalbox, que son de red) ---
$RUTA_SYSTEM = ""
$RUTA_USERSCRIPTS = ""
$RUTA_BATOCERA_SCRIPTS = ""

if ($opcion -eq "1" -or $opcion -eq "2") {
    $INPUT_RUTA = Read-Host "Introduce la ruta o IP de la consola (ej. \\192.168.1.119 o D:)"
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
        Write-Host "[ERROR] No se pudo encontrar la estructura de carpetas de la consola." -ForegroundColor Red
        Read-Host "Presiona Enter para salir"
        exit
    }
}

# --- FUNCION INTERNA DE PROCESAMIENTO UNIX (Batocera/Recalbox) ---
# $SubCarpeta: subcarpeta LOCAL (junto al .ps1) donde vive el script de origen,
# p.ej. "Batocera" o "Recalbox". No afecta al nombre con el que se instala en la consola.
function Instalar-Script-Unix($FileName, $DestinoPath, $SubCarpeta = "") {
    $CurrentDir = $PSScriptRoot
    if ([string]::IsNullOrEmpty($CurrentDir)) { $CurrentDir = Get-Location }

    if ([string]::IsNullOrEmpty($SubCarpeta)) {
        $PathOrigen = Join-Path $CurrentDir $FileName
    } else {
        $PathOrigen = Join-Path (Join-Path $CurrentDir $SubCarpeta) $FileName
    }

    # SOLUCION: Usamos -LiteralPath para evitar que los corchetes [ ] rompan la busqueda
    if (Test-Path -LiteralPath $PathOrigen) {
        if (!(Test-Path -LiteralPath $DestinoPath)) {
            $null = New-Item -ItemType Directory -Force -Path $DestinoPath
        }

        # SOLUCION: Get-Content tambien requiere -LiteralPath para archivos con corchetes
        $Contenido = Get-Content -LiteralPath $PathOrigen -Raw

        # Inyeccion segura de IP usando caracteres ASCII directos
        $Quote = [char]34
        $StringReemplazo = "IP_ESP32=" + $Quote + $IP_PANEL + $Quote
        $Contenido = $Contenido -replace ("IP_ESP32=" + $Quote + ".*" + $Quote), $StringReemplazo

        # Forzamos formato Linux (LF)
        $ContenidoFinal = $Contenido -replace "`r`n", "`n"
        $PathFinal = Join-Path $DestinoPath $FileName

        $Utf8NoBom = New-Object System.Text.UTF8Encoding($false)
        [System.IO.File]::WriteAllText($PathFinal, $ContenidoFinal, $Utf8NoBom)

        Write-Host "   [OK] Configurado e Instalado: $FileName" -ForegroundColor Green
        return $true
    } else {
        Write-Host ""
        Write-Host "   [ERROR] No se encontro el archivo $FileName en la carpeta '$SubCarpeta' junto al instalador." -ForegroundColor Red
        Write-Host "   Asegurate de que los .sh/.py esten dentro de las subcarpetas Batocera\ o Recalbox\." -ForegroundColor Yellow
        return $false
    }
}

# --- FUNCION INTERNA DE PROCESAMIENTO WINDOWS (RetroBat) ---
# Mismo patron que Instalar-Script-Unix, con dos diferencias: no se fuerza el
# salto de linea (los .py/.bat de Windows se quedan tal cual ya los tengas) y,
# si se indica $ArcadeRootValue, tambien se sustituye la linea ARCADE_ROOT
# ademas de IP_ESP32 (la usan retrobat_marquesina_start.py/stop.py).
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
        Write-Host "   [ERROR] No se encontro el archivo $FileName en la carpeta '$SubCarpeta' junto al instalador." -ForegroundColor Red
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

    Write-Host "   [OK] Configurado e Instalado: $FileName" -ForegroundColor Green
    return $true
}

# --- FUNCION: comprobar/instalar una dependencia de Windows via winget ---
function Asegurar-Dependencia-Windows($Comando, $WingetId, $Nombre, $UrlManual) {
    $existe = Get-Command $Comando -ErrorAction SilentlyContinue
    if ($existe) {
        Write-Host "   [OK] $Nombre ya esta instalado." -ForegroundColor Green
        return $true
    }

    Write-Host "   $Nombre no se ha encontrado en el PATH. Intentando instalar con winget..." -ForegroundColor Yellow
    $winget = Get-Command winget -ErrorAction SilentlyContinue
    if (-not $winget) {
        Write-Host "   [ERROR] winget no esta disponible en este equipo." -ForegroundColor Red
        Write-Host "   Instala $Nombre manualmente desde $UrlManual y vuelve a ejecutar este instalador." -ForegroundColor Yellow
        return $false
    }

    try {
        winget install --id $WingetId -e --silent --accept-package-agreements --accept-source-agreements | Out-Null
    } catch {
        Write-Host "   [ERROR] Fallo la instalacion automatica de $Nombre." -ForegroundColor Red
        Write-Host "   Instalalo manualmente desde $UrlManual y vuelve a ejecutar este instalador." -ForegroundColor Yellow
        return $false
    }

    # winget puede haber actualizado el PATH de la maquina/usuario sin que esta
    # sesion de PowerShell se entere todavia; lo refrescamos para poder
    # detectar el comando sin tener que cerrar y reabrir la ventana.
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")

    $existeAhora = Get-Command $Comando -ErrorAction SilentlyContinue
    if ($existeAhora) {
        Write-Host "   [OK] $Nombre instalado correctamente." -ForegroundColor Green
        return $true
    }

    Write-Host "   [AVISO] $Nombre se instalo pero no se detecta todavia en esta sesion de PowerShell." -ForegroundColor Yellow
    Write-Host "   Cierra esta ventana, abre una nueva, y relanza este instalador para confirmarlo." -ForegroundColor Yellow
    return $false
}

# 3. EJECUCION FILTRADA SEGUN EL SISTEMA
switch ($opcion) {
    "1" {
        Write-Host ""
        Write-Host "--> Instalando en BATOCERA..." -ForegroundColor Cyan

        Write-Host ""
        Write-Host "Que modo de funcionamiento deseas activar?" -ForegroundColor Cyan
        Write-Host "  Opcion 1: Menus y Juegos (Muestra sistemas al navegar + juego lanzado)" -ForegroundColor White
        Write-Host "  Opcion 2: Solo Juegos (Marquesina fija/reloj en menus, cambia solo al jugar)" -ForegroundColor White
        $modoBatocera = Read-Host "Selecciona una opcion (1 o 2)"

        if ($modoBatocera -ne "1" -and $modoBatocera -ne "2") {
            Write-Host ""
            Write-Host "[ERROR] Opcion de modo invalida." -ForegroundColor Red
            Read-Host "Presiona Enter para salir"
            exit
        }

        $RUTA_GAME_START = Join-Path $RUTA_BATOCERA_SCRIPTS "game-start"
        $RUTA_GAME_SELECTED = Join-Path $RUTA_BATOCERA_SCRIPTS "game-selected"
        $RUTA_SYSTEM_SELECTED = Join-Path $RUTA_BATOCERA_SCRIPTS "system-selected"
        $RUTA_GAME_END = Join-Path $RUTA_BATOCERA_SCRIPTS "game-end"
        $RUTA_QUIT = Join-Path $RUTA_BATOCERA_SCRIPTS "quit"
        $RUTA_SHUTDOWN = Join-Path $RUTA_BATOCERA_SCRIPTS "shutdown"
        $RUTA_REBOOT = Join-Path $RUTA_BATOCERA_SCRIPTS "reboot"

        # El decodificador de imagen estatica (Python) y el streamer de gif animado (ffmpeg)
        # son el motor comun; se instalan una sola vez en userscripts
        # (que en Batocera equivale a /userdata/userscripts) y los hooks los invocan.
        $inst_sender = Instalar-Script-Unix "Batocera_marquesina.py" $RUTA_USERSCRIPTS "Batocera"
        $inst_streamer = Instalar-Script-Unix "pixel_stream.py" $RUTA_USERSCRIPTS "Batocera"

        # Hooks comunes a los dos modos: juego lanzado (gif animado), fin de juego,
        # y apagado/reinicio del sistema.
        $inst_gamestart = Instalar-Script-Unix "Batocera_game-start.sh" $RUTA_GAME_START "Batocera"
        $inst_gameselected = Instalar-Script-Unix "Batocera_game-selected.sh" $RUTA_GAME_SELECTED "Batocera"
        $inst_gameend = Instalar-Script-Unix "Batocera_game-end.sh" $RUTA_GAME_END "Batocera"
        $inst_quit = Instalar-Script-Unix "Batocera_quit.sh" $RUTA_QUIT "Batocera"
        $inst_shutdown = Instalar-Script-Unix "Batocera_shutdown.sh" $RUTA_SHUTDOWN "Batocera"
        $inst_reboot = Instalar-Script-Unix "Batocera_reboot.sh" $RUTA_REBOOT "Batocera"

        # system-selected solo se instala en el modo "Menus y Juegos".
        $inst_systemselected = $false
        if ($modoBatocera -eq "1") {
            Write-Host ""
            Write-Host " Modo 'Menus y Juegos': instalando system-selected..." -ForegroundColor Yellow
            $inst_systemselected = Instalar-Script-Unix "Batocera_system-selected.sh" $RUTA_SYSTEM_SELECTED "Batocera"
        } else {
            Write-Host ""
            Write-Host " Modo 'Solo Juegos': no se instala system-selected." -ForegroundColor Yellow
            $conflictivo = Join-Path $RUTA_SYSTEM_SELECTED "Batocera_system-selected.sh"
            if (Test-Path -LiteralPath $conflictivo) {
                Remove-Item -LiteralPath $conflictivo -Force
                Write-Host "   [OK] Eliminado system-selected de una instalacion anterior en modo 'Menus y Juegos'." -ForegroundColor Green
            }
        }

        if ($inst_gamestart -or $inst_gameselected -or $inst_systemselected -or $inst_gameend -or $inst_quit -or $inst_shutdown -or $inst_reboot) {
            Write-Host " Configurando auto-permisos en Batocera (servicio nativo)..." -ForegroundColor Yellow

            # NOTA TECNICA: custom.sh queda IGNORADO a partir de Batocera v44, y su
            # ejecucion via SMB no siempre es fiable en versiones recientes (v43.x
            # incluida). El metodo soportado oficialmente desde v38 son los
            # "user services" en /userdata/system/services/, que se activan una
            # vez desde el menu de EmulationStation y persisten entre reinicios
            # y entre reinstalaciones de este script.
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
            Write-Host "   [OK] Servicio '$NombreServicio' generado con exito." -ForegroundColor Green

            # Limpieza/migracion: si una instalacion anterior dejo un custom.sh
            # con las lineas de chmod de RetroPixel, las retiramos para no
            # duplicar el mecanismo. Si el archivo se queda vacio de contenido
            # util, lo eliminamos; si el usuario tenia otras cosas propias en
            # custom.sh, se conservan intactas.
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
                    Write-Host "   [OK] custom.sh antiguo (solo permisos RetroPixel) eliminado; sustituido por el servicio." -ForegroundColor Green
                } elseif ($LineasFiltradas.Count -ne $ContenidoCustom.Count) {
                    $NuevoCustom = ($LineasFiltradas -join "`n")
                    [System.IO.File]::WriteAllText($PathCustom, $NuevoCustom, $Utf8NoBom)
                    Write-Host "   [OK] custom.sh migrado: se retiraron los permisos RetroPixel (ahora los gestiona el servicio) y se conservo el resto de tu contenido." -ForegroundColor Green
                }
            }

            Write-Host ""
            Write-Host "   [IMPORTANTE] Paso manual (solo la primera vez):" -ForegroundColor Yellow
            Write-Host "   Tras reiniciar, ve a Batocera: MENU PRINCIPAL > AJUSTES DEL SISTEMA > SERVICIOS" -ForegroundColor Yellow
            Write-Host "   y activa 'retropixelperms'. Quedara activo para siempre (tambien tras" -ForegroundColor Yellow
            Write-Host "   reinstalar este script), sin necesidad de tocarlo por SSH otra vez." -ForegroundColor Yellow
        }
    }

    "2" {
        Write-Host ""
        Write-Host "--> Configuracion para RECALBOX..." -ForegroundColor Cyan

        Write-Host ""
        Write-Host "Que modo de funcionamiento deseas activar?" -ForegroundColor Cyan
        Write-Host "  Opcion 1: Menus y Juegos (Muestra sistemas al navegar + juego lanzado)" -ForegroundColor White
        Write-Host "  Opcion 2: Solo Juegos (Marquesina fija/reloj en menus, cambia solo al jugar)" -ForegroundColor White
        $modoRecalbox = Read-Host "Selecciona una opcion (1 o 2)"

        # El streamer de gif animado (ffmpeg) es comun a los dos modos.
        $null = Instalar-Script-Unix "pixel_stream.py" $RUTA_USERSCRIPTS "Recalbox"

        if ($modoRecalbox -eq "1") {
            Write-Host ""
            Write-Host " Ejecutando instalacion de Opcion 1 (Permanente)..." -ForegroundColor Yellow
            $null = Instalar-Script-Unix "Recalbox_1(permanent).sh" $RUTA_USERSCRIPTS "Recalbox"

            $conflictivo = Join-Path $RUTA_USERSCRIPTS "Recalbox_2(permanent).sh"
            if (Test-Path -LiteralPath $conflictivo) { Remove-Item -LiteralPath $conflictivo -Force }
        }
        elseif ($modoRecalbox -eq "2") {
            Write-Host ""
            Write-Host " Ejecutando instalacion de Opcion 2 (Por Eventos)..." -ForegroundColor Yellow
            $null = Instalar-Script-Unix "Recalbox_2(permanent).sh" $RUTA_USERSCRIPTS "Recalbox"

            $conflictivo = Join-Path $RUTA_USERSCRIPTS "Recalbox_1(permanent).sh"
            if (Test-Path -LiteralPath $conflictivo) { Remove-Item -LiteralPath $conflictivo -Force }
        }
        else {
            Write-Host ""
            Write-Host "[ERROR] Opcion de modo invalida." -ForegroundColor Red
            Read-Host "Presiona Enter para salir"
            exit
        }
    }

    "3" {
        Write-Host ""
        Write-Host "--> Instalando en RETROBAT (Windows)..." -ForegroundColor Cyan

        $RUTA_RETROBAT = Read-Host "Carpeta de instalacion de RetroBat (Enter para usar: C:\RetroBat)"
        if ([string]::IsNullOrWhiteSpace($RUTA_RETROBAT)) { $RUTA_RETROBAT = "C:\RetroBat" }
        $RUTA_RETROBAT = $RUTA_RETROBAT.Trim([char]34).TrimEnd([char]92)

        $RUTA_ES_SCRIPTS = Join-Path $RUTA_RETROBAT "emulationstation\.emulationstation\scripts"
        if (-not (Test-Path -LiteralPath $RUTA_ES_SCRIPTS)) {
            Write-Host ""
            Write-Host "[ERROR] No se encontro la carpeta de scripts de EmulationStation en esa ruta." -ForegroundColor Red
            Write-Host "Se esperaba en: $RUTA_ES_SCRIPTS" -ForegroundColor Yellow
            Read-Host "Presiona Enter para salir"
            exit
        }

        $ARCADE_ROOT_RETROBAT = Read-Host "Carpeta local con las marquesinas Arcade (Enter para usar: C:\RetroPixelLED\Arcade)"
        if ([string]::IsNullOrWhiteSpace($ARCADE_ROOT_RETROBAT)) { $ARCADE_ROOT_RETROBAT = "C:\RetroPixelLED\Arcade" }
        $ARCADE_ROOT_RETROBAT = $ARCADE_ROOT_RETROBAT.Trim([char]34).TrimEnd([char]92)

        Write-Host ""
        Write-Host "Comprobando dependencias (Python y FFmpeg)..." -ForegroundColor Cyan
        $tienePython = Asegurar-Dependencia-Windows "python" "Python.Python.3.12" "Python" "https://www.python.org/downloads/"
        $tieneFFmpeg = Asegurar-Dependencia-Windows "ffmpeg" "Gyan.FFmpeg" "FFmpeg" "https://ffmpeg.org/download.html"

        $RUTA_ENGINE = Join-Path $RUTA_ES_SCRIPTS "_engine"
        $RUTA_GAME_START = Join-Path $RUTA_ES_SCRIPTS "game-start"
        $RUTA_GAME_END = Join-Path $RUTA_ES_SCRIPTS "game-end"

        Write-Host ""
        Write-Host "Instalando el motor (Python)..." -ForegroundColor Yellow
        $inst_streamer = Instalar-Script-Windows "pixel_stream_retrobat.py" $RUTA_ENGINE "RetroBat"
        $inst_start = Instalar-Script-Windows "retrobat_marquesina_start.py" $RUTA_ENGINE "RetroBat" $ARCADE_ROOT_RETROBAT
        $inst_stop = Instalar-Script-Windows "retrobat_marquesina_stop.py" $RUTA_ENGINE "RetroBat"

        if ($inst_streamer -and $inst_start -and $inst_stop) {
            Write-Host ""
            Write-Host "Generando los hooks de game-start/game-end..." -ForegroundColor Yellow

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

            Write-Host "   [OK] marquesina_iniciar.bat instalado en game-start\" -ForegroundColor Green
            Write-Host "   [OK] marquesina_detener.bat instalado en game-end\" -ForegroundColor Green
        }

        if (-not $tienePython -or -not $tieneFFmpeg) {
            Write-Host ""
            Write-Host "[AVISO] Alguna dependencia no se pudo confirmar instalada (revisa los mensajes de arriba)." -ForegroundColor Yellow
            Write-Host "Los scripts ya estan copiados, pero no funcionaran hasta que Python y FFmpeg" -ForegroundColor Yellow
            Write-Host "esten disponibles en el PATH de este PC." -ForegroundColor Yellow
        }

        Write-Host ""
        Write-Host "[IMPORTANTE] Si tu version de RetroBat requiere activar algun ajuste para que" -ForegroundColor Yellow
        Write-Host "EmulationStation ejecute estos scripts (tipo CustomEventScripts), revisalo en:" -ForegroundColor Yellow
        Write-Host "  $RUTA_RETROBAT\emulationstation\.emulationstation\es_settings.cfg" -ForegroundColor Yellow
    }

    default {
        Write-Host ""
        Write-Host "[ERROR] Opcion de sistema invalida." -ForegroundColor Red
        Read-Host "Presiona Enter para salir"
        exit
    }
}

Write-Host ""
Write-Host "=================================================" -ForegroundColor Magenta
Write-Host "             INSTALACION COMPLETADA!" -ForegroundColor Green
Write-Host "=================================================" -ForegroundColor Magenta
if ($opcion -eq "1") {
    Write-Host "IMPORTANTE: Reinicia Batocera y, la primera vez, activa 'retropixelperms' en"
    Write-Host "AJUSTES DEL SISTEMA > SERVICIOS para que los permisos se apliquen solos."
} elseif ($opcion -eq "2") {
    Write-Host "IMPORTANTE: Reinicia Recalbox."
} else {
    Write-Host "IMPORTANTE: Reinicia RetroBat (o al menos cierra y reabre EmulationStation)"
    Write-Host "para que los hooks de game-start/game-end queden activos."
}
Write-Host "--------------------------------------------------"
Read-Host "Presiona Enter para finalizar"

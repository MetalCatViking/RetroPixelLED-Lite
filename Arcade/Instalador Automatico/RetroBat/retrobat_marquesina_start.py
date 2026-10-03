#!/usr/bin/env python3
r"""
retrobat_marquesina_start.py - Pegamento entre el evento game-start de RetroBat
y pixel_stream_retrobat.py (marquesinas al ESP32 por TCP: GIF > BMP juego > BMP sistema).

RetroBat llama a este script (via el .bat de scripts\game-start\) pasandole:
    %1 = ruta completa de la rom  (ej: C:\RetroBat\roms\ports\mrboom.libretro)
    %2 = nombre del romset        (ej: mrboom)

Este script:
    1) Deduce el sistema a partir de la ruta (carpeta que cuelga de "roms").
    2) Para cualquier streamer anterior que siga vivo (por si game-end no se
       ejecuto) y limpia la bandera de stop.
    3) Lanza pixel_stream_retrobat.py en segundo plano, desligado de este
       proceso, para que RetroBat continue sin esperar.
"""
import sys
import os
import subprocess
import time

# ============================================================
#   CONFIGURACION - AJUSTA ESTOS VALORES
# ============================================================
IP_ESP32="192.168.1.117"                        # IP fija del panel RetroPixelLED (la rellena el instalador)
ESP32_PORT = 8888                                # Mismo puerto que para Batocera/Recalbox
ARCADE_ROOT = r"C:\RetroPixelLED\Arcade"         # Carpeta local con Arcade\<sistema>\*.gif

# Marquesinas fijas (BMP). Misma convencion que ReplayOS/Batocera/Recalbox:
#   - del juego:   Arcade\<sistema>\<romset>.bmp
#   - del sistema: Arcade\<sistema>.bmp   (en la RAIZ de Arcade, junto a la carpeta
#                  del sistema, no dentro de ella)

# Ruta completa a ffmpeg.exe. Dejar vacio para buscarlo en el PATH de Windows.
FFMPEG_BIN = ""

# True = escribe un log con los argumentos recibidos (util para comprobar lo
# que pasa RetroBat realmente). Fichero: %TEMP%\retropixel_start.log
LOG_DEBUG = False
# ============================================================

STOP_FLAG = os.path.join(os.environ.get("TEMP", r"C:\Windows\Temp"), "retropixel_stop.flag")


def log(msg):
    if not LOG_DEBUG:
        return
    try:
        ruta = os.path.join(os.environ.get("TEMP", r"C:\Windows\Temp"), "retropixel_start.log")
        with open(ruta, "a", encoding="utf-8") as f:
            f.write(time.strftime("%H:%M:%S ") + msg + "\n")
    except OSError:
        pass


def detectar_sistema(rom_path):
    """Sistema = carpeta que cuelga de 'roms' (soporta subcarpetas dentro del
    sistema). Si no hay carpeta 'roms' en la ruta, usa la carpeta padre."""
    partes = os.path.normpath(rom_path).split(os.sep)
    for i, parte in enumerate(partes[:-1]):
        if parte.lower() == "roms" and i + 1 < len(partes) - 1:
            return partes[i + 1]
    return os.path.basename(os.path.dirname(rom_path))


def elegir_python_sin_consola():
    exe = sys.executable
    candidato = os.path.join(os.path.dirname(exe), "pythonw.exe")
    return candidato if os.path.isfile(candidato) else exe


def main():
    if len(sys.argv) < 3:
        sys.exit(1)

    rom_path = sys.argv[1]
    rom_name = sys.argv[2]
    log(f"args: rom_path={rom_path!r} rom_name={rom_name!r}")

    sistema = detectar_sistema(rom_path)
    carpeta_sistema = os.path.join(ARCADE_ROOT, sistema)
    log(f"sistema={sistema!r} carpeta={carpeta_sistema!r}")

    script_dir = os.path.dirname(os.path.abspath(__file__))
    streamer = os.path.join(script_dir, "pixel_stream_retrobat.py")

    # Parar un streamer anterior que siga vivo (emulador cerrado a la fuerza,
    # game-end que no se ejecuto...) y dejar bandera/PID limpios ANTES de lanzar
    # el nuevo, para no cortarlo nada mas empezar ni tener dos a la vez.
    try:
        from retrobat_marquesina_stop import parar_streamer
        parar_streamer(espera_max=3.0)
    except ImportError:
        if os.path.exists(STOP_FLAG):
            try:
                os.remove(STOP_FLAG)
            except OSError:
                pass

    bmp_juego = os.path.join(carpeta_sistema, rom_name + ".bmp")
    bmp_sistema = os.path.join(ARCADE_ROOT, f"{sistema}.bmp")

    env = os.environ.copy()
    if FFMPEG_BIN:
        env["FFMPEG_BIN"] = FFMPEG_BIN

    # Desligado del proceso actual: sin consola, en su propio grupo, y sin
    # heredar stdin/stdout/stderr.
    flags = 0
    if sys.platform == "win32":
        flags = subprocess.DETACHED_PROCESS | subprocess.CREATE_NEW_PROCESS_GROUP

    subprocess.Popen(
        [
            elegir_python_sin_consola(), streamer,
            IP_ESP32, str(ESP32_PORT), carpeta_sistema, rom_name, STOP_FLAG,
            bmp_juego, bmp_sistema,
        ],
        env=env,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        creationflags=flags,
    )
    log("streamer lanzado")


if __name__ == "__main__":
    main()

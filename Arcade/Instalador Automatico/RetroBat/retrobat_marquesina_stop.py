#!/usr/bin/env python3
r"""
retrobat_marquesina_stop.py - Evento game-end de RetroBat.
RetroBat lo llama (via el .bat de scripts\game-end\) sin argumentos utiles.

Orden de trabajo:
    1) Para el streamer (pixel_stream_retrobat.py): crea la bandera de stop y
       espera a que termine solo (cierra su socket y borra bandera y PID). Si
       no termina a tiempo, lo mata con taskkill.
    2) Solo entonces manda el comando de parada al ESP32 por una conexion
       nueva. Si se enviara antes, el streamer podria seguir mandando frames
       despues del STOP y el panel volveria a pintar la marquesina.
       Este paso tambien limpia el panel cuando la marquesina era un BMP
       (el streamer ya habia terminado y nadie mas la quitaria).

Tambien exporta parar_streamer(), que usa game-start para asegurarse de que no
queda un streamer de una partida anterior.
"""
import sys
import os
import time
import socket
import subprocess

# ============================================================
#   CONFIGURACION - MISMOS VALORES QUE EN retrobat_marquesina_start.py
# ============================================================
IP_ESP32="192.168.1.117"  # la rellena el instalador
ESP32_PORT = 8888

# Comando que entiende el firmware para dejar de mostrar la marquesina.
# Es el mismo que usa Batocera_marquesina.py. Si el firmware espera otro
# texto (por ejemplo b"END"), cambialo aqui.
COMANDO_STOP = b"STOP"

# Segundos maximos que se espera a que el streamer termine solo antes de matarlo
ESPERA_MAX_PARADA = 5.0
# ============================================================

STOP_FLAG = os.path.join(os.environ.get("TEMP", r"C:\Windows\Temp"), "retropixel_stop.flag")
PID_FILE = STOP_FLAG + ".pid"

CREATIONFLAGS = subprocess.CREATE_NO_WINDOW if sys.platform == "win32" else 0


def _leer_pid():
    try:
        with open(PID_FILE) as f:
            return int(f.read().strip())
    except (OSError, ValueError):
        return None


def _proceso_vivo(pid):
    if sys.platform != "win32":
        try:
            os.kill(pid, 0)
            return True
        except OSError:
            return False
    import ctypes
    kernel32 = ctypes.windll.kernel32
    PROCESS_QUERY_LIMITED_INFORMATION = 0x1000
    STILL_ACTIVE = 259
    handle = kernel32.OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, False, pid)
    if not handle:
        return False
    codigo = ctypes.c_ulong()
    ok = kernel32.GetExitCodeProcess(handle, ctypes.byref(codigo))
    kernel32.CloseHandle(handle)
    return bool(ok) and codigo.value == STILL_ACTIVE


def _matar(pid):
    try:
        if sys.platform == "win32":
            subprocess.run(
                ["taskkill", "/F", "/T", "/PID", str(pid)],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                creationflags=CREATIONFLAGS, timeout=5,
            )
        else:
            os.kill(pid, 9)
    except Exception:
        pass


def _borrar(ruta):
    try:
        os.remove(ruta)
    except OSError:
        pass


def parar_streamer(espera_max=ESPERA_MAX_PARADA):
    """Para el streamer si esta corriendo y deja bandera y PID limpios.
    Devuelve True si habia un streamer vivo."""
    pid = _leer_pid()
    habia_vivo = pid is not None and _proceso_vivo(pid)

    if habia_vivo:
        try:
            with open(STOP_FLAG, "w") as f:
                f.write("stop")
        except OSError:
            pass

        limite = time.monotonic() + espera_max
        while time.monotonic() < limite and _proceso_vivo(pid):
            time.sleep(0.1)

        if _proceso_vivo(pid):
            _matar(pid)
            time.sleep(0.2)

    _borrar(STOP_FLAG)
    _borrar(PID_FILE)
    return habia_vivo


def enviar_stop_esp32():
    """Manda el comando de parada al ESP32. Reintenta unas veces porque el
    ESP32 puede tardar un instante en liberar la conexion del streamer."""
    for _ in range(4):
        try:
            with socket.create_connection((IP_ESP32, ESP32_PORT), timeout=2.0) as s:
                s.sendall(COMANDO_STOP)
            return True
        except OSError:
            time.sleep(0.5)
    return False


def main():
    parar_streamer()
    enviar_stop_esp32()


if __name__ == "__main__":
    main()

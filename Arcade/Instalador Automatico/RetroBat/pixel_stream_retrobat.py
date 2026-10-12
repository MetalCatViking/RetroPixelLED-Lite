#!/usr/bin/env python3
"""
pixel_stream_retrobat.py - Marquesina por TCP para Retrobat (solo evento game-start).

Como Retrobat no tiene evento game-select, este script hace TODO al arrancar
el juego, con esta prioridad:

  1. GIF(s) del juego:  <ROM>.gif y <ROM>_*.gif  -> se reproducen en bucle
                        (secuencia si hay más de uno) hasta que llegue STOP_FLAG.
  2. Marquesina fija del juego (BMP)               -> se envía una vez y termina.
  3. Marquesina fija del sistema (BMP)             -> se envía una vez y termina.
  4. Nada de lo anterior                           -> no se hace nada (ni se conecta).

Si hay GIFs pero ninguno se puede decodificar (FFmpeg falla, fichero corrupto),
se cae automáticamente a los pasos 2 y 3.

Uso:
  pixel_stream_retrobat.py IP PUERTO CARPETA_SISTEMA ROM_NAME STOP_FLAG [BMP_JUEGO] [BMP_SISTEMA]

  BMP_JUEGO   (opcional) ruta a la marquesina fija del juego. Si no se pasa, se
              busca CARPETA_SISTEMA/ROM_NAME.bmp. Pasar "" para saltarlo.
  BMP_SISTEMA (opcional) ruta a la marquesina fija del sistema. Si no se pasa,
              no hay paso 3.

Tanto los GIF como los BMP se decodifican con FFmpeg (escalado NEAREST a
128x32 y conversión de color en C), y se envían como frames crudos: el mismo
formato que ya recibe el ESP32 con el script de BMP de Batocera.

Se puede indicar el ejecutable con la variable de entorno FFMPEG_BIN; si no,
se busca 'ffmpeg' en el PATH.
"""
import sys
import os
import socket
import glob
import time
import shutil
import subprocess

ESP32_IP = sys.argv[1]
ESP32_PORT = int(sys.argv[2])
CARPETA_SISTEMA = sys.argv[3]
ROM_NAME = sys.argv[4]
STOP_FLAG = sys.argv[5]
BMP_JUEGO_ARG = sys.argv[6] if len(sys.argv) > 6 else None
BMP_SISTEMA_ARG = sys.argv[7] if len(sys.argv) > 7 else None

ANCHO, ALTO = 128, 32
FPS_DESTINO = 12
INTERVALO_ENVIO = 1.0 / FPS_DESTINO

FFMPEG_BIN = os.environ.get("FFMPEG_BIN") or shutil.which("ffmpeg") or "ffmpeg"

# En Windows evita que salga una ventana de consola al lanzar FFmpeg
CREATIONFLAGS = subprocess.CREATE_NO_WINDOW if sys.platform == "win32" else 0

# Ahora es el script de game-start quien para al streamer anterior (si lo hay) y
# limpia la bandera ANTES de lanzar este. Si el streamer también la borrara al
# arrancar, podría "comerse" la orden de stop dirigida al streamer anterior y
# dejar dos streamers a la vez. Por eso va a False.
LIMPIAR_STOP_AL_ARRANCAR = False

# Fichero con el PID de este proceso mientras está vivo. Lo usan game-start y
# game-end para saber si hay un streamer corriendo y esperar a que termine.
PID_FILE = STOP_FLAG + ".pid"

# False = RGB888 (24 bits - 12.288 bytes/frame)
# True  = RGB565 (16 bits -  8.192 bytes/frame). NO ACTIVAR sin cambiar antes el firmware.
USA_RGB565 = False

if USA_RGB565:
    BYTES_POR_FRAME = ANCHO * ALTO * 2
    PIX_FMT = "rgb565le"
else:
    BYTES_POR_FRAME = ANCHO * ALTO * 3
    PIX_FMT = "rgb24"


# --------------------------------------------------------------------------
# Utilidades de fichero / bandera de stop
# --------------------------------------------------------------------------
def debe_pararse():
    return os.path.exists(STOP_FLAG)


def limpiar_stop_flag():
    if os.path.exists(STOP_FLAG):
        try:
            os.remove(STOP_FLAG)
        except OSError:
            pass


def escribir_pid():
    try:
        with open(PID_FILE, "w") as f:
            f.write(str(os.getpid()))
    except OSError:
        pass


def borrar_pid():
    try:
        os.remove(PID_FILE)
    except OSError:
        pass


def construir_secuencia():
    """Busca únicamente 'rom.gif' y 'rom_*.gif'."""
    base_gif = os.path.join(CARPETA_SISTEMA, f"{ROM_NAME}.gif")
    patron_secuencia = os.path.join(CARPETA_SISTEMA, f"{ROM_NAME}_*.gif")

    secuencia = []
    if os.path.exists(base_gif):
        secuencia.append(base_gif)
    secuencia.extend(sorted(glob.glob(patron_secuencia)))
    return secuencia


def candidatos_estaticos():
    """Lista ordenada de marquesinas fijas existentes: primero la del juego,
    después la del sistema."""
    candidatos = []

    if BMP_JUEGO_ARG is None:
        bmp_juego = os.path.join(CARPETA_SISTEMA, f"{ROM_NAME}.bmp")
    else:
        bmp_juego = BMP_JUEGO_ARG
    if bmp_juego and os.path.isfile(bmp_juego):
        candidatos.append(bmp_juego)

    if BMP_SISTEMA_ARG and os.path.isfile(BMP_SISTEMA_ARG):
        candidatos.append(BMP_SISTEMA_ARG)

    return candidatos


# --------------------------------------------------------------------------
# Decodificación con FFmpeg
# --------------------------------------------------------------------------
def _ffmpeg_a_frames(ruta, filtro, solo_primer_frame=False):
    """Ejecuta FFmpeg y devuelve la lista de frames crudos (bytes) o [] si falla."""
    cmd = [FFMPEG_BIN, '-nostdin', '-loglevel', 'error', '-i', ruta, '-vf', filtro]
    if solo_primer_frame:
        cmd += ['-frames:v', '1']
    cmd += ['-pix_fmt', PIX_FMT, '-f', 'rawvideo', 'pipe:1']

    try:
        proceso = subprocess.Popen(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            creationflags=CREATIONFLAGS,
        )
        raw_video, _ = proceso.communicate()
    except Exception:
        return []

    frames = []
    for i in range(0, len(raw_video), BYTES_POR_FRAME):
        frame = raw_video[i:i + BYTES_POR_FRAME]
        if len(frame) == BYTES_POR_FRAME:
            frames.append(frame)
    return frames


def cargar_gif_ffmpeg(ruta_gif):
    """Todos los fotogramas del GIF, escalados y a FPS_DESTINO."""
    filtro = f'fps={FPS_DESTINO},scale={ANCHO}:{ALTO}:flags=neighbor'
    return _ffmpeg_a_frames(ruta_gif, filtro)


def cargar_imagen_ffmpeg(ruta_img):
    """Un único frame a partir de una imagen fija (BMP)."""
    filtro = f'scale={ANCHO}:{ALTO}:flags=neighbor'
    frames = _ffmpeg_a_frames(ruta_img, filtro, solo_primer_frame=True)
    return frames[0] if frames else None


# --------------------------------------------------------------------------
# Conexión TCP con reconexión y backoff
# --------------------------------------------------------------------------
class ConexionMarquesina:
    """Encapsula el socket TCP con reconexión automática y backoff progresivo
    (3s -> 6s -> 12s -> tope 30s). enviar() reintenta EL MISMO frame tras
    reconectar: nunca lo descarta, solo lo retrasa."""

    PASO_ESPERA = 0.15  # para poder cortar la espera al instante con la bandera de stop

    def __init__(self, ip, puerto):
        self.ip = ip
        self.puerto = puerto
        self.sock = None
        self.fallos_consecutivos = 0
        self._conectar_con_backoff()

    def _crear_socket(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(3.0)
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        return s

    def _esperar_troceado(self, segundos):
        transcurrido = 0.0
        while transcurrido < segundos and not debe_pararse():
            time.sleep(self.PASO_ESPERA)
            transcurrido += self.PASO_ESPERA

    def _conectar_con_backoff(self):
        while not debe_pararse():
            try:
                nuevo_sock = self._crear_socket()
                nuevo_sock.connect((self.ip, self.puerto))
                self.sock = nuevo_sock
                self.fallos_consecutivos = 0
                return True
            except Exception:
                self.fallos_consecutivos += 1
                espera = min(3 * (2 ** min(self.fallos_consecutivos - 1, 3)), 30)
                self._esperar_troceado(espera)
        return False

    def enviar(self, frame_bytes):
        while not debe_pararse():
            try:
                self.sock.sendall(frame_bytes)
                return True
            except Exception:
                try:
                    self.sock.close()
                except Exception:
                    pass
                if not self._conectar_con_backoff():
                    return False
        return False

    def cerrar(self):
        if self.sock:
            try:
                self.sock.close()
            except Exception:
                pass


# --------------------------------------------------------------------------
# Modos de reproducción
# --------------------------------------------------------------------------
def reproducir_gifs(conexion, secuencia):
    """Reproduce la secuencia de GIFs en bucle hasta que llegue la bandera de stop.

    Devuelve True si se reprodujo algo (o si se paró por la bandera), y False si
    tras una pasada completa NINGÚN GIF se pudo decodificar, para que el
    llamador pueda caer a la marquesina fija."""
    cache_frames = {}

    while not debe_pararse():
        gifs_validos_en_pasada = 0

        for ruta_gif in secuencia:
            if debe_pararse():
                break

            if ruta_gif not in cache_frames:
                cache_frames[ruta_gif] = cargar_gif_ffmpeg(ruta_gif)

            frames = cache_frames[ruta_gif]
            if not frames:
                continue  # GIF ilegible: pasamos al siguiente
            gifs_validos_en_pasada += 1

            for frame_bytes in frames:
                if debe_pararse():
                    break

                inicio = time.monotonic()
                if not conexion.enviar(frame_bytes):
                    return True  # bandera de stop durante una reconexión

                restante = INTERVALO_ENVIO - (time.monotonic() - inicio)
                if restante > 0:
                    time.sleep(restante)

        if gifs_validos_en_pasada == 0 and not debe_pararse():
            return False

    return True


def mostrar_estatica(conexion, candidatos):
    """Envía UNA vez la primera marquesina fija que se pueda decodificar."""
    for ruta in candidatos:
        frame = cargar_imagen_ffmpeg(ruta)
        if frame is not None:
            return conexion.enviar(frame)
    return False


# --------------------------------------------------------------------------
def main():
    if LIMPIAR_STOP_AL_ARRANCAR:
        limpiar_stop_flag()

    secuencia = construir_secuencia()
    estaticas = candidatos_estaticos()

    # Prioridad 4: no hay nada que mostrar -> no hacemos nada (ni conectamos)
    if not secuencia and not estaticas:
        sys.exit(0)

    escribir_pid()
    conexion = None
    try:
        conexion = ConexionMarquesina(ESP32_IP, ESP32_PORT)
        if conexion.sock is None:
            # Solo ocurre si llegó la bandera de stop mientras reconectaba
            return

        # Prioridad 1: GIF(s) del juego
        if secuencia and reproducir_gifs(conexion, secuencia):
            return

        # Prioridades 2 y 3: marquesina fija del juego, si no la del sistema
        if estaticas:
            mostrar_estatica(conexion, estaticas)
    finally:
        if conexion is not None:
            conexion.cerrar()
        borrar_pid()
        limpiar_stop_flag()


if __name__ == "__main__":
    main()

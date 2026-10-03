# 🕹️ Integración con RetroBat

El **Modo Arcade** en la versión Lite permite que tu matriz LED funcione también como marquesina dinámica con **RetroBat**. Al lanzar un juego, el panel mostrará automáticamente su marquesina — animada si la tienes preparada, o estática si no.

> [!NOTE]
> **Diferencia clave con Batocera/Recalbox:** el EmulationStation de RetroBat no dispone de los eventos `system-selected`/`game-selected` (los que permiten reaccionar mientras solo **navegas**, sin llegar a lanzar nada). Los únicos eventos disponibles en esta carpeta de scripts son `game-start`, `game-end`, `quit`, `reboot`, `shutdown`, `sleep`, `start`, `update-gamelists` y `wake` — así que en RetroBat **no existe el modo "Menús y Juegos"** que sí tienen Batocera/Recalbox. El panel solo reacciona en el momento exacto en que lanzas un juego.

Para compensar esa limitación, `game-start` aplica una lógica combinada con tres niveles de prioridad, para que siempre se muestre lo mejor disponible:

1. **GIF animado del juego** (si existe) → se reproduce en bucle durante toda la partida.
2. Si no hay GIF (o no se puede decodificar): **marquesina estática del juego** (`.bmp`).
3. Si tampoco hay marquesina del juego: **logo del sistema** (`.bmp`).
4. Si no hay nada de lo anterior: no se envía nada — el panel se queda tal cual estaba (reloj o el bucle de GIFs normal).

Al salir del juego (`game-end`), el panel vuelve siempre a su estado normal, haya mostrado algo o no.

#### Aprovechamiento de Recursos (Scraping)
Igual que en Batocera y Recalbox, se reutilizan las imágenes que **RetroBat ya ha scrapeado** con su propio scraper (el `gamelist.xml` de cada sistema, con su etiqueta `<marquee>`) — no hace falta buscar nada a mano juego por juego.

## 1. Configuración Crítica: IP Fija para el ESP32

Igual que en Batocera y Recalbox, aquí es RetroBat quien le habla al ESP32 (no al revés), así que el ESP32 necesita una **IP fija**.

> [!TIP]
> **Asignar IP fija al ESP32:**
> 1. Accede a la configuración de tu router.
> 2. Busca la sección de **DHCP Estático** o **Asignación de IP por MAC**.
> 3. Vincula la dirección MAC de tu ESP32 con la IP que vayas a usar (ej: `192.168.1.117`).
> 4. Dado que cada router es diferente, si tienes dudas busca en Google: *"Cómo asignar IP fija [modelo de tu router]"*.

## 2. Requisitos Previos: Python y FFmpeg

A diferencia de Batocera/Recalbox (que ya traen Python/FFmpeg integrados en el propio sistema), aquí todo corre en tu PC con Windows, así que estas dos dependencias no vienen puestas de serie.

> [!NOTE]
> No hace falta que las instales tú a mano: el instalador del paso siguiente las detecta y, si faltan, intenta instalarlas automáticamente con `winget` (el gestor de paquetes de Windows). Si tu Windows no tiene `winget` disponible (versiones antiguas), te lo avisará con un enlace de descarga manual.

## 3. Instalación Automática en RetroBat

Igual que para Batocera/Recalbox, un **Script Instalador en PowerShell** se encarga de todo el despliegue desde tu PC — con la diferencia de que aquí no hay ninguna ruta de red de por medio: todo vive en el mismo PC donde tienes RetroBat instalado.

---

### 📦 ¿Qué hace este instalador por ti?

* **Configuración de IP:** inyecta la IP de tu panel LED en los dos scripts de comunicación (`retrobat_marquesina_start.py` y `retrobat_marquesina_stop.py`).
* **Dependencias:** comprueba si tienes Python y FFmpeg, e intenta instalarlos con `winget` si faltan.
* **Organización de archivos:** crea una carpeta `_engine` dentro de `emulationstation\.emulationstation\scripts\` y copia ahí los tres scripts Python (el motor común a `game-start` y `game-end`).
* **Hooks de eventos:** genera `marquesina_iniciar.bat` en `scripts\game-start\` y `marquesina_detener.bat` en `scripts\game-end\`, ya apuntando a la ruta correcta del motor que acaba de instalar.

---

### 🛠️ Requisitos Previos

1. Tener **RetroBat** ya instalado en el PC (por defecto en `C:\RetroBat`).
2. Conocer la **IP fija de tu panel LED** Retro Pixel LED (ej. `192.168.1.117`).
3. Descargar la carpeta completa `Instalador Automático` desde este repositorio, la puedes encontrar [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Instalador%20Automatico).

> [!IMPORTANT]
> Si descargaste el repositorio en un archivo `.zip`, asegúrate de **descomprimirlo por completo** antes de ejecutar el instalador.

---

### 💻 Paso a Paso

1. Abre la carpeta `Instalador Automatico` en tu PC. Dentro, junto a `Batocera\` y `Recalbox\`, encontrarás también la carpeta `RetroBat\` con los tres scripts Python.

2. Haz **clic** sobre `Ejecutar Script Instalador Arcade.bat`.

3. Sigue las instrucciones en la ventana de la consola:
   * **Paso 1:** Introduce la IP de tu panel LED y pulsa `Enter`.
   * **Paso 2:** Selecciona `3) RetroBat (Windows)`.

4. El instalador te pedirá:
   * La carpeta de instalación de RetroBat (Enter para usar `C:\RetroBat`).
   * La carpeta local donde guardas las marquesinas (Enter para usar `C:\RetroPixelLED`).

5. Comprobará Python y FFmpeg, copiará los scripts y generará los dos `.bat` de evento. Al finalizar, verás `INSTALACIÓN COMPLETADA!`.

6. **Reinicia RetroBat** (o al menos cierra y vuelve a abrir EmulationStation) para que los nuevos scripts de evento queden activos.
<img width="970" height="746" alt="image" src="https://github.com/user-attachments/assets/8939cb48-e8aa-4daa-821f-22fa0c35f15a" />

> [!CAUTION]
> Si tras reiniciar el panel no reacciona al lanzar un juego, es posible que tu versión de RetroBat tenga desactivado por defecto el disparo de scripts personalizados. Revisa `emulationstation\.emulationstation\es_settings.cfg` en busca de una opción relacionada con scripts de eventos (`CustomEventScripts` o similar) y actívala si existe.

### 4. 🛠️ Marquesinas

Usaremos el script que se encuentra en la carpeta `Arcade/Marquesinas/` del proyecto [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas): Consta de dos archivos `Ejecutar Script Marquesinas RetroBat.bat` y `Script Marquesinas RetroBat.ps1`.

1. **Ejecuta el script** ``Ejecutar Script Marquesinas RetroBat.bat` (Lanzador para evitar bloqueos de Windows).
2. **Configuración de rutas:**
   * **Origen:** ruta de tus ROMs de RetroBat (Enter para usar `C:\RetroBat\roms`, ruta local — aquí no hace falta ninguna ruta de red, todo está en el mismo PC).
   * **Destino:** Enter para usar `C:\RetroPixelLED`.
3. **Selección de Sistema:** el script detecta automáticamente qué sistemas tienen un `gamelist.xml` ya escrapeado por RetroBat. Puedes elegir uno, varios, o **Todos (0)**.
4. Las marquesinas quedan listas directamente en `C:\RetroPixelLED\Arcade\` — como es local, no hace falta copiar nada a ningún sitio.
<img width="946" height="574" alt="image" src="https://github.com/user-attachments/assets/7eb498e2-9644-471a-9762-10c08cca52c7" />

### ¿Qué hace el script automáticamente?
* **Redimensionado:** convierte tus marquesinas originales a **128×32 píxeles**.
* **Formato:** fuerza el color a **BMP de 24 bits** (formato compatible con el driver DMA del ESP32), con el mismo dithering a RGB565 que usa Batocera.

> [!CAUTION]
> Cada vez que añadas nuevos juegos o hagas un "Scrape" en RetroBat, **debes volver a ejecutar este script** para actualizar los índices y las imágenes. Sin este paso, el panel no sabrá que los nuevos archivos existen.

### 5. 🎬 Marquesinas Animadas (GIF)

#### ¿Cómo funciona en RetroBat?

Como no hay un evento de "navegación", todo ocurre en el instante de lanzar el juego (`game-start`):

- Si existe un `.gif` con el mismo nombre que la rom, se reproduce en bucle durante toda la partida.
- Si no existe (o no se puede decodificar), se muestra la marquesina `.bmp` del juego.
- Si tampoco hay `.bmp` del juego, se muestra el logo `.bmp` del sistema.
- Si no hay nada de lo anterior, el panel no cambia — sigue con el reloj o el bucle de GIFs que tuviera puesto.

Al salir del juego (`game-end`), el panel vuelve siempre a su estado normal.

#### Nombrado de archivos

Igual que en Batocera, el GIF tiene que llamarse **igual que la marquesina `.bmp`** del mismo juego, en la misma carpeta:

```
C:\RetroPixelLED\Arcade\neogeo\mslug.bmp   <- ya la tenías
C:\RetroPixelLED\Arcade\neogeo\mslug.gif   <- la añades tú, mismo nombre
```

También puedes preparar una secuencia con sufijos `_01`, `_02`, `_03`... El panel los reproduce todos en orden y vuelve a empezar por el primero, en bucle continuo:

```
C:\RetroPixelLED\Arcade\neogeo\mslug.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_01.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_02.gif
```

> [!TIP]
> No hace falta que existan los tres — con solo `mslug.gif` ya funciona perfectamente en bucle.

#### ¿De dónde saco los GIFs?

Si ya tienes (o has descargado) una colección de GIFs de arcade con nombres "humanos" en vez de nombres de romset (por ejemplo `ARCADE_NEOGEO_MetalSlugStory.gif` en vez de `mslug.gif`), usa el sigueinte script para renombar los GIFs, descargalo de [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs) y sigue estos pasos:

1. Ejecuta `Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`.

2. **Opción 1 — Renombrar GIFs:** indica la carpeta donde tienes los GIFs. El script consulta un catálogo público de nombres de MAME ([`MAME.dat`](https://github.com/libretro/libretro-database)) para identificar a qué romset corresponde cada título, además de un diccionario propio para los casos más comunes. Puedes elegir entre coincidencia solo exacta, o exacta + aproximada (resuelve más casos, con algo más de riesgo). Lo que no consiga identificar se mueve a una carpeta `SinResolver\` para que lo revises tú a mano — nunca renombra "a ciegas".
   <img width="1090" height="830" alt="image" src="https://github.com/user-attachments/assets/58ba389f-367c-4114-b6e1-533018f50e77" />

3. **Opción 2 — Copiar GIFs a carpetas de sistema:** una vez renombrados, esta opción compara los GIFs contra los romsets reales de cada sistema en tu carpeta `ROMS/` y los copia automáticamente a `Arcade/<sistema>/`, junto a los `.bmp` que ya tengas ahí.
   <img width="1106" height="1204" alt="image" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

También puedes generarlos — el panel solo necesita que el archivo final esté a **128×32 píxeles**. Como referencia, si tu colección de RetroBat ya tiene vídeos de preview scrapeados (`<video>` en el `gamelist.xml`), puedes convertirlos a GIF con una herramienta como [dmd_gif_converter](https://github.com/red77290/dmd_gif_converter), que además de redimensionar incluye un modo de encuadre automático pensado para no perder la acción al reducir un vídeo grande a un tamaño tan pequeño. Es un proyecto de terceros, independiente de este repositorio — cualquier otro método que te deje un `.gif` de 128×32 servirá igual de bien.

### 6. 🛠️ Logos de Sistemas
 Podemos usar los logos ya redimensionados que se encuentran en la carpeta `Arcade/Logos Sistemas/` del proyecto [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas).
 1.  **Copiar:** Copia todo su contenido directamente a `C:\RetroPixelLED\Arcade\``, como se indica en el punto `7. Estructura de archivos en el PC de RetroBat`.
    
 Si prefieres usar otros logos como por ejemplo los del tema que tienes instalado. Usaremos el script se encuentra en la carpeta `Arcade/Logos Sistemas/` del proyecto [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas). Consta de dos archivos `Ejecutar Script Logos.bat` y `Script Logos.ps1`.

1.  **Ejecuta el archivo** ``Ejecutar Script Logos.bat` (Lanzador para evitar bloqueos de Windows).
2.  **Configuración de rutas:**
    * **Origen:** Introduce la ruta donde tienes los logos (ej: `\\192.168.1.119\userdata\themes\Animatics-DX-master\art\logos`).
    * **Destino:** Introduce la ruta `C:\Logos`.
4.  **Copiar:** Si seleccionaste la ruta `C:\Logos` copia todo su contenido directamente a `C:\RetroPixelLED\Arcade\``, como se indica en el punto `7. Estructura de archivos en el PC de RetroBat`.

<img width="1102" height="573" alt="image" src="https://github.com/user-attachments/assets/7d90cc90-3cad-4991-8498-591081ab2004" />


### ¿Qué hace el script automáticamente?
* **Redimensionado:** Convierte tus marquesinas originales a **128x32 píxeles**.
* **Formato:** Fuerza el color a **BMP de 24 bits** (formato compatible con el driver DMA del ESP32).

Si ya tienes logos de sistema preparados para Batocera/Recalbox/ReplayOS (`Arcade/<sistema>.bmp`), puedes copiarlos directamente a `C:\RetroPixelLED\Arcade\` — es la misma convención de nombre y tamaño en los cuatro frontends.

## 7. Estructura de archivos en el PC de RetroBat

A diferencia de Batocera/Recalbox (SD/red) y de ReplayOS (tarjeta SD del panel), aquí todo vive en una carpeta **local** de tu propio PC:

* **`C:\RetroPixelLED\Arcade\<sistema>.txt`** (índice de romsets con marquesina de ese sistema)
* **`C:\RetroPixelLED\Arcade\<sistema>\rom_name.bmp`** (marquesina estática del juego, ej: `mslug.bmp`)
* **`C:\RetroPixelLED\Arcade\<sistema>\rom_name.gif`** (opcional: marquesina animada del mismo juego)
* **`C:\RetroPixelLED\Arcade\<sistema>.bmp`** (opcional: logo del sistema, si no hay marquesina propia del juego)

El motor que sirve todo esto (los tres scripts Python + los dos `.bat` de evento) vive aparte, dentro de la propia instalación de RetroBat, y el instalador se encarga de colocarlo — no necesitas tocar esa parte.

#### Ejemplo visual de carpetas:
```
📂 C:\RetroPixelLED\
└── 📂 Arcade\
    ├── 📄 neogeo.txt
    ├── 📄 neogeo.bmp        <- logo del sistema Neo Geo
    ├── 📂 neogeo\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif     <- opcional, marquesina animada
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 mame\
        ├── 📄 pacman.bmp
        ├── 📄 tetris.bmp
        └── ...
```

## 8. ¡Disfruta de las marquesinas mientras juegas en tu Arcade con RetroBat!

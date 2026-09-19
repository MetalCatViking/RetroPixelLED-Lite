# ¡¡¡¡ EN CONSTRUCCIÓN !!!!

# 🕹️ Integración con ReplayOS

El **Modo Arcade** en la versión Lite permite que tu matriz LED funcione como una marquesina dinámica con **ReplayOS**. El panel detectará el sistema y juego que estás jugando y te lo mostrará automáticamente — y si el juego tiene una marquesina animada preparada, la reproducirá en bucle mientras juegas.

> [!NOTE]
> **Diferencia clave con Batocera/Recalbox:** ReplayOS no tiene un sistema de scripts/eventos como esos frontends, así que aquí **no se instala nada en el dispositivo de ReplayOS**. En su lugar, es el propio ESP32 quien pregunta periódicamente a la **API REST** de ReplayOS qué sistema y juego están activos ahora mismo, y busca la marquesina correspondiente en su propia tarjeta SD. Toda la preparación de contenido (imágenes, GIFs, listados) se hace en tu PC con las herramientas de PowerShell de este apartado, y el resultado se copia a la SD del panel.

#### Aprovechamiento de Recursos (Scraping)
Al igual que en Batocera, no hace falta buscar las marquesinas a mano juego por juego: las herramientas de este apartado descargan las imágenes de **ArcadeDB** (o **TheGamesDB**) y las convierten automáticamente al formato que necesita el panel.

## 1. Configuración Crítica: API REST y Token de ReplayOS

Para que el modo **🕹️ Arcade** funcione, el ESP32 necesita poder preguntarle a ReplayOS qué se está jugando. Esto requiere tres cosas:

1. **Activar el control por red en ReplayOS:** en `REPLAY OPTIONS > SYSTEM`, activa la opción **`NET CONTROL`** (`system_net_control`). Esto abre el servidor de la API en el puerto `55356`.
2. **Obtener el Token (Net Control Code):** ve a `REPLAY OPTIONS > INFORMATION > NET CONTROL CODE`. Es un código numérico de 6 dígitos — lo necesitarás para configurar el ESP32.
3. **IP fija para tu ReplayOS:** el ESP32 le pregunta siempre a la misma dirección IP, así que esta debe ser fija.

> [!TIP]
> **Asignar IP fija a tu ReplayOS:**
> 1. Accede a la configuración de tu router.
> 2. Busca la sección de **DHCP Estático** o **Asignación de IP por MAC**.
> 3. Vincula la dirección MAC de tu Raspberry Pi/mini PC con ReplayOS a una IP fija (ej: `192.168.1.120`).
> 4. Dado que cada router es diferente, si tienes dudas busca en Google: *"Cómo asignar IP fija [modelo de tu router]"*.

4. **Configurar el ESP32:** Podemos configurarlo desde 2 sitios APP o archivo config.ini *se recomienda la APP*.
- **APP:** Entramos en configuración en la sección *ARCADE* seleccionamos `ReplayOS` y en la sección *ReplayOS* indicamos la IP y el Token.
- **Config.ini:** Abrimos el archivo config.ini en la sección *[LOGIC]* indicamos `ARCADE_ENABLE=3` y en la sección *[REPLAY_OS]* indicamos la IP que tiene asignada ReplayOS en `IP=xxx.xxx.xxx.xxx` y el token en `TOKEN=xxxxxx`

## 2. Herramientas de PowerShell

A diferencia de Batocera y Recalbox, aquí no hay un instalador que despliegue nada en el propio ReplayOS. En su lugar hay **dos scripts** que preparan todo el contenido en tu PC, listo para copiar a la SD del panel:

* **`Script_Marquesinas_ReplayOS.ps1`** — scrapea recursos de ArcadeDB/TheGamesDB, genera las imágenes `.bmp` y crea/audita los listados `.txt` que el ESP32 necesita para localizar rápido qué romsets tienen marquesina.
* **`Script_RetroPixelLED_GIF_Renamer.ps1`** — si ya tienes una colección de GIFs animados de arcade (por ejemplo, de un pack de terceros) con nombres "humanos" en vez de nombres de romset de MAME, este script te los renombra automáticamente y los coloca en la carpeta del sistema que corresponda.

### 🛠️ Requisitos Previos

1. Conocer la **IP local** de tu ReplayOS y el **Token** obtenidos en el paso 1.
2. Tener acceso, desde el PC, a tu carpeta de **ROMS** y a la **tarjeta SD** del panel (insertada en el PC, o accesible como unidad).
3. Descargar los script para las marquesinas **`Ejecutar Script Marquesinas_ReplayOS.bat`** y **`Script_Marquesinas_ReplayOS.ps1`**, los puedes encontrar [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas).
4. Descargar los script para renombrar los GIFs **`Ejecutar Script_RetroPixelLED_GIF_Renamer.ps1`** y **`Script_RetroPixelLED_GIF_Renamer.ps1`**, los puedes encontrar [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs).
> [!IMPORTANT]
> Si descargaste el repositorio en un archivo `.zip`, asegúrate de **descomprimirlo por completo** antes de ejecutar los scripts.

### 💻 Paso a Paso: Scrapeo, Imágenes y Listados

1. Ejecuta `Ejecutar Script Marquesinas_ReplayOS.bat`.
2. **Opción 1 — Scrapear sistema(s):** introduce la ruta de tu carpeta `ROMS`. El script te mostrará las subcarpetas de sistema que encuentre ahí — elige una, varias, o escribe `TODOS`.
3. Elige la fuente (ArcadeDB o TheGamesDB) y qué recursos descargar (marquesina, logo, decal...) se recomienda **DECAL** para crear las marquesinas. El script guardará los recursos **sin procesar** en una carpeta de caché reutilizable — no genera todavía ningún `.bmp`.
4. **Opción 2 — Generar imágenes:** convierte, 100% sin conexión, lo que ya tienes en caché a `.bmp` de 128×32 con el dithering RGB565 aplicado, y lo deja en tu carpeta `Arcade/<sistema>/` local.
5. **Opción 3 — Generar / auditar listados:** escanea esa carpeta `Arcade/<sistema>/` y genera (o actualiza) el `<sistema>.txt` que el ESP32 usa para saber, muy rápido, qué romsets tienen marquesina. Si ya existía un listado, te avisa antes de tocarlo de qué archivos sobran o faltan indexar.
6. **Copia la carpeta `Arcade` completa a la raíz de la SD del panel.**

### 🎬 Marquesinas Animadas (GIF)

#### ¿Cómo funciona?

- El panel muestra la marquesina estática (`.bmp`) del juego mientras detecta que estás jugando, igual que siempre.
- Si además existe un `.gif` con el mismo nombre en la misma carpeta, el panel lo reproduce en bucle mientras dura la partida.
- Si no existe, no pasa nada — se queda con la marquesina estática, sin ningún error.

#### Nombrado de archivos

El GIF tiene que llamarse igual que la marquesina `.bmp` del mismo juego:

```
Arcade/snk_ngo/mslug.bmp   <- ya la tenías
Arcade/snk_ngo/mslug.gif   <- la añades tú, mismo nombre
```

También puedes preparar una secuencia de varios GIFs para el mismo juego con el sufijo `_01`, `_02`, `_03`... El panel los reproduce todos en orden y vuelve a empezar por el primero, en bucle continuo:

```
Arcade/snk_ngo/mslug.gif
Arcade/snk_ngo/mslug_01.gif
Arcade/snk_ngo/mslug_02.gif
```

> [!TIP]
> No hace falta que existan los tres — con solo `mslug.gif` ya funciona perfectamente en bucle.

#### El comodín `_default`

Si un juego no tiene ni marquesina propia ni logo de sistema, el panel busca por último un `_default.bmp` (o `_default.gif`, si quieres que también sea animado) en la raíz de `Arcade/`. Es opcional, pero evita que el panel se quede sin nada que mostrar para los juegos que aún no has preparado.

#### ¿De dónde saco los GIFs?

Si ya tienes (o has descargado) una colección de GIFs de arcade con nombres "humanos" en vez de nombres de romset (por ejemplo `Air_Gallet_01.gif` en vez de `agallet.gif`), usa **`Script_RetroPixelLED_GIF_Renamer`**:

1. **Opción 1 — Renombrar GIFs:** indica la carpeta donde tienes los GIFs. El script consulta un catálogo público de nombres de MAME ([`MAME.dat`](https://github.com/libretro/libretro-database)) para identificar a qué romset corresponde cada título, además de un diccionario propio para los casos más comunes. Puedes elegir entre coincidencia solo exacta, o exacta + aproximada (resuelve más casos, con algo más de riesgo). Lo que no consiga identificar se mueve a una carpeta `SinResolver\` para que lo revises tú a mano — nunca renombra "a ciegas".
2. **Opción 2 — Copiar GIFs a carpetas de sistema:** una vez renombrados, esta opción compara los GIFs contra los romsets reales de cada sistema en tu carpeta `ROMS/` y los copia automáticamente a `Arcade/<sistema>/`, junto a los `.bmp` que ya tengas ahí.

> [!NOTE]
> Esta segunda opción también sabe copiar a las rutas de Batocera o Recalbox (por red), si alguna vez preparas GIFs para varios frontends a la vez desde el mismo PC.

## 3. Estructura de archivos en la SD de Retro Pixel LED lite

Para que la integración funcione, la carpeta `Arcade` va en la **raíz** de la SD del panel:

* **`Arcade/<sistema>.txt`** (listado de romsets con marquesina de ese sistema)
* **`Arcade/<sistema>/rom_name.bmp`** (marquesina estática del juego, ej: `mslug.bmp`)
* **`Arcade/<sistema>/rom_name.gif`** (opcional: marquesina animada del mismo juego)
* **`Arcade/<sistema>.bmp`** (opcional: logo del sistema, si no hay marquesina propia del juego)
* **`Arcade/_default.bmp`** (opcional: comodín si no hay ni marquesina ni logo de sistema)
* **`Arcade/_default.gif`** (opcional: comodín si no hay ni marquesina ni logo de sistema)

#### Ejemplo visual de carpetas:
```
📂 F:\ (SD de RetroPixelLED)
└── 📂 Arcade\
    ├── 📄 snk_ngo.txt
    ├── 📄 fbneo.txt
    ├── 📄 arcade_ngo.bmp        <- logo del sistema Neo Geo
    ├── 📄 _default.bmp          <- comodin general
    ├── 📄 _default.gif          <- comodin general
    ├── 📂 snk_ngo\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif         <- opcional, marquesina animada
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 fbneo\
        ├── 📄 dino.bmp
        ├── 📄 avsp.bmp
        └── ...
```

## 4. ¡Disfruta de las marquesinas mientras juegas en tu Arcade con ReplayOS!

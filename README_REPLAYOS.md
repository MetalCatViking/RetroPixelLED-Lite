# 🕹️ Integración con ReplayOS

El **Modo Arcade** en la versión Lite permite que tu matriz LED funcione como una marquesina dinámica con **ReplayOS**. El panel detectará el sistema y juego que estás jugando, te lo mostrará automáticamente y si el juego tiene una marquesina animada preparada, la reproducirá en bucle mientras juegas.

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
- **APP:** Entramos en configuración en la sección *ARCADE* seleccionamos `ReplayOS` y en la sección *REPLAYOS* indicamos la IP y el Token.
  
  <img width="428" height="945" alt="image" src="https://github.com/user-attachments/assets/1724a3a3-f292-4b11-8e54-b547c4ece0fa" />
  <img width="412" height="938" alt="image" src="https://github.com/user-attachments/assets/e649cf1b-1a98-423e-b3c8-5386d39fd0f1" />

- **Config.ini:** Abrimos el archivo config.ini en la sección *[LOGIC]* indicamos `ARCADE_ENABLE=3` y en la sección *[REPLAY_OS]* indicamos la IP que tiene asignada ReplayOS en `IP=xxx.xxx.xxx.xxx` y el token en `TOKEN=xxxxxx`
  <img width="683" height="155" alt="image" src="https://github.com/user-attachments/assets/ec850c72-c111-46ab-bb0b-1c6bd9b24073" /> <img width="510" height="141" alt="image" src="https://github.com/user-attachments/assets/18c8d28c-2b52-48f0-b384-d9e66196467d" />



## 2. Herramientas de PowerShell

A diferencia de Batocera y Recalbox, aquí no hay un instalador que despliegue nada en el propio ReplayOS. En su lugar hay **dos scripts** que preparan todo el contenido en tu PC, listo para copiar a la SD del panel:

* **`Script_Marquesinas_ReplayOS.ps1`** — scrapea recursos de ArcadeDB/TheGamesDB, genera las imágenes `.bmp` y crea/audita los listados `.txt` que el ESP32 necesita para localizar rápido qué romsets tienen marquesina.
* **`Script_RetroPixelLED_GIF_Renamer.ps1`** — si ya tienes una colección de GIFs animados de arcade (por ejemplo, de un pack de terceros) con nombres "humanos" en vez de nombres de romset de MAME, este script te los renombra automáticamente y los coloca en la carpeta del sistema que corresponda.

### 🛠️ Requisitos Previos

1. Tener acceso, desde el PC, a tu carpeta de **ROMS** y a la **tarjeta SD** del panel (insertada en el PC, o accesible como unidad).
2. Descargar los script para las marquesinas **`Ejecutar Script Marquesinas_ReplayOS.bat`** y **`Script_Marquesinas_ReplayOS.ps1`**, los puedes encontrar [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas).
3. Descargar los script para renombrar los GIFs **`Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`** y **`Script_RetroPixelLED_GIF_Renamer.ps1`**, los puedes encontrar [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs).
> [!IMPORTANT]
> Si descargaste el repositorio en un archivo `.zip`, asegúrate de **descomprimirlo por completo** antes de ejecutar los scripts.

### 💻 Paso a Paso: Scrapeo, Imágenes y Listados

1. Ejecuta `Ejecutar Script Marquesinas_ReplayOS.bat`.
2. **Opción 1 — Scrapear sistema(s):** introduce la ruta de tu carpeta `ROMS` y la ruta donde quieres guardar los recursos descargados. El script te mostrará las subcarpetas de sistema que encuentre en la ruta indicada, elige una, varias, o escribe `TODOS`.
   <img width="1100" height="740" alt="image" src="https://github.com/user-attachments/assets/684f93c5-ab5c-46e2-a792-e1b3535f9ed6" />
   
3. Elige la fuente (ArcadeDB o TheGamesDB) y qué recursos descargar (marquesina, logo, decal...) se recomienda **DECAL** para crear las marquesinas. El script guardará los recursos **sin procesar** en una carpeta de caché reutilizable — no genera todavía ningún `.bmp`.
   <img width="1101" height="656" alt="image" src="https://github.com/user-attachments/assets/cfb9c33e-77a9-43ba-91e8-c3a84bcea19a" />

4. **Opción 2 — Generar imágenes:** convierte, 100% sin conexión, lo que ya tienes en caché a `.bmp` de 128×32 con el dithering RGB565 aplicado, y lo deja en tu carpeta `Arcade/<sistema>/` local.
   <img width="1098" height="875" alt="image" src="https://github.com/user-attachments/assets/110fc03e-e99e-4e87-840c-660eed8808ff" />

5. **Opción 3 — Generar / auditar listados:** escanea esa carpeta `Arcade/<sistema>/` y genera (o actualiza) el `<sistema>.txt` que el ESP32 usa para saber, muy rápido, qué romsets tienen marquesina. Si ya existía un listado, te avisa antes de tocarlo de qué archivos sobran o faltan indexar.
   <img width="1104" height="619" alt="image" src="https://github.com/user-attachments/assets/19718bdc-b849-44b1-916b-7ac81835f1c3" />

7. **Copia la carpeta `Arcade` completa a la raíz de la SD de Retro Pixel LED lite**





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
Arcade/snk_ng/mslug.gif
Arcade/snk_ng/mslug_01.gif
Arcade/snk_ng/mslug_02.gif
```

> [!TIP]
> No hace falta que existan los tres — con solo `mslug.gif` ya funciona perfectamente en bucle.

#### El comodín `_default`

Si un juego no tiene ni marquesina propia ni logo de sistema, el panel busca por último un `_default.bmp` (o `_default.gif`, si quieres que también sea animado) en la raíz de `Arcade/`. Es opcional, pero evita que el panel se quede sin nada que mostrar para los juegos que aún no has preparado.

#### ¿De dónde saco los GIFs?

Si ya tienes (o has descargado) una colección de GIFs de arcade con nombres "humanos" en vez de nombres de romset (por ejemplo `ARCADE_NEOGEO_MetalSlugStory.gif` en vez de `mslug.gif`), usa el sigueinte script para renombar los GIFs, descargalo de [aquí](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs).:

1. Ejecuta `Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`.

2. **Opción 1 — Renombrar GIFs:** indica la carpeta donde tienes los GIFs. El script consulta un catálogo público de nombres de MAME ([`MAME.dat`](https://github.com/libretro/libretro-database)) para identificar a qué romset corresponde cada título, además de un diccionario propio para los casos más comunes. Puedes elegir entre coincidencia solo exacta, o exacta + aproximada (resuelve más casos, con algo más de riesgo). Lo que no consiga identificar se mueve a una carpeta `SinResolver\` para que lo revises tú a mano — nunca renombra "a ciegas".
   <img width="1090" height="830" alt="image" src="https://github.com/user-attachments/assets/58ba389f-367c-4114-b6e1-533018f50e77" />

3. **Opción 2 — Copiar GIFs a carpetas de sistema:** una vez renombrados, esta opción compara los GIFs contra los romsets reales de cada sistema en tu carpeta `ROMS/` y los copia automáticamente a `Arcade/<sistema>/`, junto a los `.bmp` que ya tengas ahí.
  <img width="1106" height="1204" alt="image" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

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
    ├── 📄 snk_ng.txt
    ├── 📄 arcade_fbneo.txt
    ├── 📄 arcade_ng.bmp        <- logo del sistema Neo Geo
    ├── 📄 _default.bmp          <- comodin general
    ├── 📄 _default.gif          <- comodin general
    ├── 📂 snk_ng\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif         <- opcional, marquesina animada
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 arcade_fbneo\
        ├── 📄 dino.bmp
        ├── 📄 avsp.bmp
        └── ...
```

## 4. ¡Disfruta de las marquesinas mientras juegas en tu Arcade con ReplayOS!

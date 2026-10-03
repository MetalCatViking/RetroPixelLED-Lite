# 🕹️ Intégration avec RetroBat

Le **Mode Arcade** de la version Lite permet à votre matrice LED de fonctionner également comme une marquee dynamique avec **RetroBat**. Lors du lancement d'un jeu, le panneau affichera automatiquement sa marquee — animée si vous l'avez préparée, ou statique sinon.

> \[!NOTE\]
> **Différence clé avec Batocera/Recalbox :** l'EmulationStation de RetroBat ne dispose pas des événements `system-selected`/`game-selected` (ceux qui permettent de réagir lorsque vous **naviguez** simplement, sans lancer de jeu). Les seuls événements disponibles dans ce dossier de scripts sont `game-start`, `game-end`, `quit`, `reboot`, `shutdown`, `sleep`, `start`, `update-gamelists` et `wake` — par conséquent, dans RetroBat, **il n'existe pas de mode "Menus et Jeux"** comme dans Batocera/Recalbox. Le panneau ne réagit qu'au moment précis où vous lancez un jeu.

Pour compenser cette limitation, `game-start` applique une logique combinée avec trois niveaux de priorité, afin de toujours afficher la meilleure option disponible :

1. **GIF animé du jeu** (s'il existe) → est lu en boucle pendant toute la partie.
2. S'il n'y a pas de GIF (ou s'il ne peut pas être décodé) : **marquee statique du jeu** (`.bmp`).
3. S'il n'y a pas non plus de marquee du jeu : **logo du système** (`.bmp`).
4. Si rien de tout cela n'existe : rien n'est envoyé — le panneau reste dans son état actuel (horloge ou boucle de GIFs normale).

En quittant le jeu (`game-end`), le panneau revient toujours à son état normal, qu'il ait affiché quelque chose ou non.

#### Exploitation des Ressources (Scraping)

Comme sur Batocera et Recalbox, les images que **RetroBat a déjà scrapées** avec son propre scraper (le fichier `gamelist.xml` de chaque système, avec sa balise `<marquee>`) sont réutilisées — il n'est pas nécessaire de chercher quoi que ce soit manuellement jeu par jeu.

## 1. Configuration Critique : IP Fixe pour l'ESP32

Comme sur Batocera et Recalbox, c'est RetroBat qui communique avec l'ESP32 (et non l'inverse), l'ESP32 a donc besoin d'une **IP fixe**.

> \[!TIP\]
> **Attribuer une IP fixe à l'ESP32 :**
> 1. Accédez à la configuration de votre routeur.
> 2. Cherchez la section **DHCP Statique** ou **Bail DHCP permanent par adresse MAC**.
> 3. Associez l'adresse MAC de votre ESP32 à l'IP que vous allez utiliser (ex : `192.168.1.117`).
> 4. Chaque routeur étant différent, en cas de doute, cherchez sur Google : *"Comment attribuer une IP fixe \[modèle de votre routeur\]"*.

## 2. Prérequis : Python et FFmpeg

Contrairement à Batocera/Recalbox (qui intègrent déjà Python/FFmpeg dans le système lui-même), ici tout s'exécute sur votre PC Windows, ces deux dépendances ne sont donc pas installées par défaut.

> \[!NOTE\]
> Vous n'avez pas besoin de les installer manuellement : l'installateur de l'étape suivante les détecte et, s'il en manque, tente de les installer automatiquement avec `winget` (le gestionnaire de paquets de Windows). Si votre version de Windows ne dispose pas de `winget` (anciennes versions), un message vous fournira un lien de téléchargement manuel.

## 3. Installation Automatique dans RetroBat

Comme pour Batocera/Recalbox, un **Script d'Installation PowerShell** s'occupe de tout le déploiement depuis votre PC — à la différence près qu'ici aucun chemin réseau n'est nécessaire : tout se trouve sur le même PC où RetroBat est installé.

### 📦 Que fait cet installateur pour vous ?

* **Configuration IP :** injecte l'adresse IP de votre panneau LED dans les deux scripts de communication (`retrobat_marquesina_start.py` et `retrobat_marquesina_stop.py`).
* **Dépendances :** vérifie la présence de Python et FFmpeg, et tente de les installer via `winget` s'ils sont absents.
* **Organisation des fichiers :** crée un dossier `_engine` dans `emulationstation\.emulationstation\scripts\` et y copie les trois scripts Python (le moteur commun à `game-start` et `game-end`).
* **Hooks d'événements :** génère `marquesina_iniciar.bat` dans `scripts\game-start\` et `marquesina_detener.bat` dans `scripts\game-end\`, pointant déjà vers le bon chemin du moteur venant d'être installé.

### 🛠️ Prérequis

1. Avoir **RetroBat** déjà installé sur le PC (par défaut dans `C:\RetroBat`).
2. Connaître l'**IP fixe de votre panneau LED** Retro Pixel LED (ex. `192.168.1.117`).
3. Télécharger le dossier complet `Instalador Automático` depuis ce dépôt, disponible [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Instalador%20Automatico).

> \[!IMPORTANT\]
> Si vous avez téléchargé le dépôt sous forme de fichier `.zip`, veillez à le **décompresser entièrement** avant d'exécuter l'installateur.

### 💻 Étape par Étape

1. Ouvrez le dossier `Instalador Automatico` sur votre PC. À l'intérieur, à côté de `Batocera\` et `Recalbox\`, vous trouverez le dossier `RetroBat\` contenant les trois scripts Python.
2. **Cliquez** sur `Ejecutar Script Instalador Arcade.bat`.
3. Suivez les instructions dans la fenêtre de console :
   * **Étape 1 :** Saisissez l'adresse IP de votre panneau LED et appuyez sur `Entrée`.
   * **Étape 2 :** Sélectionnez `3) RetroBat (Windows)`.
4. L'installateur vous demandera :
   * Le dossier d'installation de RetroBat (`Entrée` pour utiliser `C:\RetroBat`).
   * Le dossier local où vous stockez les marquees (`Entrée` pour utiliser `C:\RetroPixelLED`).
5. Il vérifiera Python et FFmpeg, copiera les scripts et générera les deux fichiers `.bat` d'événement. Une fois terminé, le message `INSTALACIÓN COMPLETADA!` s'affichera.
6. **Redémarrez RetroBat** (ou au moins fermez et rouvrez EmulationStation) pour que les nouveaux scripts d'événement deviennent actifs.
<img width="970" height="746" alt="image" src="https://github.com/user-attachments/assets/8939cb48-e8aa-4daa-821f-22fa0c35f15a" />

> \[!CAUTION\]
> Si après avoir redémarré le panneau ne réagit pas lors du lancement d'un jeu, il est possible que votre version de RetroBat ait désactivé par défaut le déclenchement des scripts personnalisés. Vérifiez le fichier `emulationstation\.emulationstation\es_settings.cfg` à la recherche d'une option liée aux scripts d'événements (`CustomEventScripts` ou similaire) et activez-la si elle existe.

### 4. 🛠️ Marquees (Bandeaux)

Nous utiliserons le script situé dans le dossier `Arcade/Marquesinas/` du projet [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas). Il se compose de deux fichiers : `Ejecutar Script Marquesinas RetroBat.bat` et `Script Marquesinas RetroBat.ps1`.

1. **Exécutez le script** `Ejecutar Script Marquesinas RetroBat.bat` (Lanceur pour éviter les blocages de Windows).
2. **Configuration des chemins :**
   * **Source :** chemin vers vos ROMs RetroBat (`Entrée` pour utiliser `C:\RetroBat\roms`, chemin local — aucun chemin réseau n'est nécessaire ici, tout est sur le même PC).
   * **Destination :** `Entrée` pour utiliser `C:\RetroPixelLED`.
3. **Sélection du Système :** le script détecte automatiquement quels systèmes ont un fichier `gamelist.xml` déjà scrapé par RetroBat. Vous pouvez en choisir un, plusieurs ou **Tous (0)**.
4. Les marquees seront prêtes directement dans `C:\RetroPixelLED\Arcade\` — comme tout est local, il n'est pas nécessaire de copier quoi que ce soit ailleurs.   
<img width="946" height="574" alt="image" src="https://github.com/user-attachments/assets/7eb498e2-9644-471a-9762-10c08cca52c7" />

### Que fait le script automatiquement ?

* **Redimensionnement :** convertit vos marquees originales en **128×32 pixels**.
* **Format :** force le mode colorimétrique en **BMP 24 bits** (format compatible avec le pilote DMA de l'ESP32), avec le même dither RGB565 utilisé par Batocera.

> \[!CAUTION\]
> À chaque fois que vous ajoutez de nouveaux jeux ou effectuez un "Scrape" dans RetroBat, **vous devez réexécuter ce script** pour mettre à jour les index et les images. Sans cette étape, le panneau ne saura pas que les nouveaux fichiers existent.

### 5. 🎬 Marquees Animées (GIF)

#### Comment cela fonctionne-t-il dans RetroBat ?

Puisqu'il n'y a pas d'événement de "navigation", tout se passe au moment du lancement du jeu (`game-start`) :
- Si un fichier `.gif` portant le même nom que la ROM existe, il est lu en boucle pendant toute la partie.
- S'il n'existe pas (ou ne peut pas être décodé), la marquee `.bmp` du jeu est affichée.
- S'il n'y a pas non plus de `.bmp` pour le jeu, le logo `.bmp` du système est affiché.
- Si rien de tout cela n'existe, le panneau ne change pas — il conserve l'horloge ou la boucle de GIFs en cours.

En quittant le jeu (`game-end`), le panneau revient toujours à son état normal.

#### Nommage des fichiers

Comme dans Batocera, le GIF doit porter **strictement le même nom que la marquee `.bmp`** du même jeu, dans le même dossier :

```
C:\RetroPixelLED\Arcade\neogeo\mslug.bmp   <- déjà présent
C:\RetroPixelLED\Arcade\neogeo\mslug.gif   <- vous l'ajoutez, même nom

```

Vous pouvez également préparer une séquence avec les suffixes `_01`, `_02`, `_03`... Le panneau les lit tous dans l'ordre et recommence au premier en boucle continue :

```
C:\RetroPixelLED\Arcade\neogeo\mslug.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_01.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_02.gif

```

> \[!TIP\]
> Il n'est pas nécessaire que les trois existent — le fichier `mslug.gif` seul fonctionne parfaitement en boucle.

#### Où trouver des GIFs ?

Si vous possédez déjà (ou avez téléchargé) une collection de GIFs arcade nommés avec des titres lisibles plutôt que des noms de romsets (par exemple `ARCADE_NEOGEO_MetalSlugStory.gif` au lieu de `mslug.gif`), utilisez le script suivant pour les renommer. Téléchargez-le [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs) et suivez ces étapes :

1. Exécutez `Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`.

2. **Option 1 — Renommer les GIFs :** indiquez le dossier contenant vos GIFs. Le script consulte un catalogue public de noms MAME ([`MAME.dat`](https://github.com/libretro/libretro-database)) pour identifier le romset correspondant à chaque titre, en plus d'un dictionnaire interne pour les cas les plus courants. Vous pouvez choisir entre une correspondance exacte uniquement, ou exacte + approximative (résout plus de cas, avec un léger risque supplémentaire). Tout ce qui ne peut pas être identifié est déplacé dans un dossier `SinResolver\` pour révision manuelle — il ne renomme jamais "à l'aveugle".
   <img width="1090" height="830" alt="image" src="https://github.com/user-attachments/assets/58ba389f-367c-4114-b6e1-533018f50e77" />

3. **Option 2 — Copier les GIFs dans les dossiers systèmes :** une fois renommés, cette option compare les GIFs aux romsets réels de chaque système dans votre dossier `ROMS/` et les copie automatiquement dans `Arcade/<système>/`, aux côtés des fichiers `.bmp` existants.
    <img width="1106" height="1204" alt="image" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

Vous pouvez également les générer — le panneau exige uniquement que le fichier final soit en **128×32 pixels**. À titre de référence, si votre collection RetroBat contient déjà des vidéos d'aperçu scrapées (`<video>` dans le `gamelist.xml`), vous pouvez les convertir en GIF avec un outil comme [dmd_gif_converter](https://github.com/red77290/dmd_gif_converter). En plus de redimensionner, il inclut un mode de cadrage automatique conçu pour ne pas perdre l'action lors de la réduction d'une grande vidéo à une si petite taille. Il s'agit d'un projet tiers indépendant de ce dépôt — toute autre méthode produisant un `.gif` en 128×32 conviendra parfaitement.

### 6. 🛠️ Logos des Systèmes

Vous pouvez utiliser les logos pré-redimensionnés situés dans le dossier `Arcade/Logos Sistemas/` du projet [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas).

1. **Copier :** Copiez tout son contenu directement dans `C:\RetroPixelLED\Arcade\`, comme indiqué à la section `7. Structure des fichiers sur le PC RetroBat`.

Si vous préférez utiliser d'autres logos, par exemple ceux du thème que vous avez installé, utilisez le script situé dans le dossier `Arcade/Logos Sistemas/` du projet [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas). Il comprend deux fichiers : `Ejecutar Script Logos.bat` et `Script Logos.ps1`.

1. **Exécutez le fichier** `Ejecutar Script Logos.bat` (Lanceur pour éviter les blocages de Windows).
2. **Configuration des chemins :**
   * **Source :** saisissez le chemin où se trouvent vos logos (ex : `\\192.168.1.119\userdata\themes\Animatics-DX-master\art\logos`).
   * **Destination :** saisissez le chemin `C:\Logos`.
3. **Copier :** Si vous avez sélectionné le chemin `C:\Logos`, copiez tout son contenu directement dans `C:\RetroPixelLED\Arcade\`, comme indiqué à la section `7. Structure des fichiers sur le PC RetroBat`.

   <img width="1102" height="573" alt="image" src="https://github.com/user-attachments/assets/7d90cc90-3cad-4991-8498-591081ab2004" />

### Que fait le script automatiquement ?

* **Redimensionnement :** convertit vos marquees originales en **128x32 pixels**.

* **Format :** force le mode colorimétrique en **BMP 24 bits** (format compatible avec le pilote DMA de l'ESP32).

Si vous possédez déjà des logos de système préparés pour Batocera/Recalbox/ReplayOS (`Arcade/<système>.bmp`), vous pouvez les copier directement dans `C:\RetroPixelLED\Arcade\` — la convention de nom et de taille est rigoureusement la même pour les quatre frontends.

## 7. Structure des fichiers sur le PC RetroBat

Contrairement à Batocera/Recalbox (SD/réseau) et ReplayOS (carte SD du panneau), ici tout réside dans un dossier **local** de votre propre PC :

* **`C:\RetroPixelLED\Arcade\<système>.txt`** (index des romsets possédant une marquee pour ce système)
* **`C:\RetroPixelLED\Arcade\<système>\rom_name.bmp`** (marquee statique du jeu, ex : `mslug.bmp`)
* **`C:\RetroPixelLED\Arcade\<système>\rom_name.gif`** (optionnel : marquee animée du jeu)
* **`C:\RetroPixelLED\Arcade\<système>.bmp`** (optionnel : logo du système, en l'absence de marquee propre au jeu)

Le moteur qui alimente tout cela (les trois scripts Python + les deux fichiers `.bat` d'événement) réside séparément au sein de l'installation de RetroBat, et l'installateur se charge de le placer — vous n'avez pas besoin de modifier cette partie.

#### Exemple visuel de l'arborescence :

```
📂 C:\RetroPixelLED\
└── 📂 Arcade\
    ├── 📄 neogeo.txt
    ├── 📄 neogeo.bmp        <- logo du système Neo Geo
    ├── 📂 neogeo\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif     <- optionnel, marquee animée
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 mame\
        ├── 📄 pacman.bmp
        ├── 📄 tetris.bmp
        └── ...

```

## 8. Profitez de vos marquees pendant que vous jouez sur votre Arcade avec RetroBat !

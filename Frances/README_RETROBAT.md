# 🕹️ Intégration avec RetroBat

Le **Mode Arcade** de Retro Pixel LED Lite permet à votre matrice LED de fonctionner comme une marquise dynamique avec **RetroBat**. Au lancement d'un jeu, le panneau affiche automatiquement sa marquise, animée si vous avez préparé un GIF, ou statique dans le cas contraire.

**Réglage du panneau :** dans le menu OSD du panneau, ouvrez **Lecture → Arcade** et sélectionnez **RetroBat (option 4)**. Vous pouvez aussi définir `ARCADE_ENABLE=4` dans `config.ini`. Ce mode doit être activé pour que le panneau accepte les commandes de RetroBat.

> [!NOTE]
> **Différence importante par rapport à Batocera/Recalbox :** l'EmulationStation de RetroBat ne fournit pas les événements `system-selected` et `game-selected`, qui permettent de réagir pendant la navigation dans les menus. Les scripts disponibles utilisent notamment les événements `game-start` et `game-end`. RetroBat ne propose donc pas le mode « Menus et jeux » de Batocera/Recalbox : le panneau change au lancement du jeu, et non pendant la navigation.

Au lancement d'un jeu, le système choisit la meilleure ressource disponible dans cet ordre :

1. **GIF animé du jeu**, s'il existe et peut être décodé ; il est lu en boucle pendant la partie.
2. **Marquise statique du jeu** (`.bmp`) si le GIF n'existe pas ou ne peut pas être décodé.
3. **Logo du système** (`.bmp`) si aucune marquise du jeu n'est disponible.
4. Si aucune ressource n'existe, le panneau conserve son affichage actuel (horloge ou boucle de GIF normale).

À la sortie du jeu (`game-end`), le panneau revient à son fonctionnement normal.

#### Réutiliser les ressources déjà scrapées

Comme avec Batocera et Recalbox, l'intégration réutilise les images que RetroBat a déjà récupérées avec son scraper, notamment les marquises indiquées par la balise `<marquee>` dans le fichier `gamelist.xml` de chaque système. Il n'est donc pas nécessaire de rechercher manuellement chaque image.

## 1. Configuration essentielle : adresse IP fixe de l'ESP32

RetroBat communique avec l'ESP32 ; c'est pourquoi le panneau doit conserver une **adresse IP fixe**.

> [!TIP]
> **Attribuer une adresse IP fixe à l'ESP32 :**
> 1. Ouvrez la configuration de votre routeur.
> 2. Recherchez une option telle que **DHCP statique** ou **Association IP-MAC**.
> 3. Associez l'adresse MAC de l'ESP32 à l'adresse IP souhaitée (par exemple `192.168.1.117`).
> 4. Les menus diffèrent selon les routeurs. En cas de doute, recherchez les instructions correspondant au modèle de votre routeur.

## 2. Prérequis : Python et FFmpeg

Contrairement à Batocera/Recalbox, Python et FFmpeg ne sont pas nécessairement installés par défaut sur votre PC Windows.

> [!NOTE]
> L'installateur ci-dessous vérifie la présence de Python et de FFmpeg. Si nécessaire, il tente de les installer avec `winget` (Gestionnaire de packages Windows). Si `winget` n'est pas disponible, l'installateur affiche un lien pour effectuer l'installation manuellement.

## 3. Installation automatique de RetroBat

Un script PowerShell effectue la configuration depuis le PC sur lequel RetroBat est installé. Tout fonctionne localement sur ce PC ; aucun chemin réseau n'est nécessaire pour les fichiers de marquises.

### 📦 Que fait l'installateur ?

- **Configuration de l'IP :** insère l'adresse IP du panneau dans les scripts de communication `retrobat_marquesina_start.py` et `retrobat_marquesina_stop.py`.
- **Dépendances :** vérifie Python et FFmpeg et tente de les installer via `winget` si nécessaire.
- **Organisation des fichiers :** crée un dossier `_engine` dans `emulationstation\.emulationstation\scripts\` et y copie les trois scripts Python.
- **Événements de jeu :** crée les fichiers `.bat` nécessaires dans les dossiers `scripts\game-start\` et `scripts\game-end\`, configurés pour appeler le moteur installé.

### 🛠️ Prérequis

1. Avoir **RetroBat** installé sur le PC (le chemin par défaut est `C:\RetroBat`).
2. Connaître l'adresse IP fixe du panneau Retro Pixel LED (par exemple `192.168.1.117`).
3. Télécharger le dossier complet [`Instalador Automatico` depuis le dépôt](https://github.com/MetalCatViking/RetroPixelLED-Lite/tree/main/Arcade/Instalador%20Automatico).

> [!IMPORTANT]
> Si vous téléchargez le dépôt sous forme de fichier `.zip`, extrayez-le entièrement avant de lancer l'installateur.

### 💻 Étapes d'installation

1. Ouvrez le dossier `Instalador Automatico`. Il contient les lanceurs, les scripts d'installation et les sous-dossiers `Batocera\`, `Recalbox\` et `RetroBat\`.

2. Lancez **`Ejecutar Script Instalador Arcade EN.bat`**. L'installateur affiche ses instructions en anglais.

3. Suivez les instructions dans la console :
   - **Étape 1 :** saisissez l'adresse IP du panneau et appuyez sur `Entrée`.
   - **Étape 2 :** sélectionnez `3) RetroBat (Windows)`.

4. Lorsque cela vous est demandé, indiquez :
   - le dossier d'installation de RetroBat (appuyez sur `Entrée` pour utiliser `C:\RetroBat`) ;
   - le dossier local où stocker les marquises (appuyez sur `Entrée` pour utiliser `C:\RetroPixelLED\Arcade`).

5. L'installateur vérifie les dépendances, copie les scripts et crée les deux fichiers d'événements. À la fin, il affiche `INSTALLATION COMPLETED!`.

6. **Redémarrez RetroBat**, ou fermez puis rouvrez EmulationStation, pour activer les nouveaux scripts.

<img width="970" height="746" alt="Capture de l'installation RetroBat" src="https://github.com/user-attachments/assets/8939cb48-e8aa-4daa-821f-22fa0c35f15a" />

> [!CAUTION]
> Si le panneau ne réagit pas au lancement d'un jeu après le redémarrage, votre version de RetroBat peut désactiver l'exécution des scripts d'événements. Vérifiez le fichier `emulationstation\.emulationstation\es_settings.cfg` et recherchez un paramètre relatif aux scripts d'événements (par exemple `CustomEventScripts`).

## 4. Préparer les marquises statiques

Le script de préparation se trouve dans le dossier [`Arcade/Marquesinas/`](https://github.com/MetalCatViking/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas).

1. Lancez **`Ejecutar Script Marquesinas_RetroBat EN.bat`**.
2. Configurez les chemins :
   - **Source :** dossier des ROMs RetroBat (appuyez sur `Entrée` pour utiliser `C:\RetroBat\roms`).
   - **Destination :** dossier où générer les fichiers (appuyez sur `Entrée` pour utiliser `C:\RetroPixelLED`).
3. Le script détecte les systèmes qui possèdent déjà un fichier `gamelist.xml`. Choisissez un système par son numéro ou **tous les systèmes (0)**.
4. Les marquises et les index sont générés dans `C:\RetroPixelLED\Arcade\`. Comme le dossier est local, il n'est pas nécessaire de copier les fichiers sur un partage réseau.

<img width="946" height="574" alt="Capture du script de préparation des marquises RetroBat" src="https://github.com/user-attachments/assets/7eb498e2-9644-471a-9762-10c08cca52c7" />

Le script effectue automatiquement :
- **Redimensionnement :** conversion des marquises en **128 × 32 pixels**.
- **Format :** conversion en BMP 24 bits, compatible avec le pilote DMA de l'ESP32, avec tramage RGB565.

> [!CAUTION]
> Après l'ajout de jeux ou un nouveau scraping dans RetroBat, relancez ce script pour mettre à jour les index et les images. Sinon, le panneau ne connaîtra pas les nouveaux fichiers.

## 5. Marquises animées (GIF)

### Fonctionnement

Lors du lancement d'un jeu (`game-start`) :

- si un GIF portant le même nom que la ROM existe, il est lu en boucle pendant la partie ;
- si le GIF est absent ou ne peut pas être décodé, la marquise statique du jeu est affichée ;
- si aucune marquise statique n'existe, le logo du système est affiché ;
- si aucune de ces ressources n'existe, l'affichage actuel du panneau est conservé.

Lorsque vous quittez le jeu (`game-end`), le panneau revient à son affichage normal.

### Nommage des fichiers

Le GIF doit porter **exactement le même nom que la marquise `.bmp`** du jeu et se trouver dans le même dossier :

```text
C:\RetroPixelLED\Arcade\neogeo\mslug.bmp   <- marquise statique
C:\RetroPixelLED\Arcade\neogeo\mslug.gif   <- GIF animé correspondant
```

Vous pouvez également préparer une séquence de GIFs pour un même jeu en ajoutant les suffixes `_01`, `_02`, `_03`, etc. Le panneau les lit dans l'ordre puis recommence au début :

```text
C:\RetroPixelLED\Arcade\neogeo\mslug.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_01.gif
C:\RetroPixelLED\Arcade\neogeo\mslug_02.gif
```

Un seul fichier `mslug.gif` suffit ; les suffixes sont facultatifs.

### Trouver et copier des GIFs

Si votre collection utilise des noms descriptifs plutôt que des noms de ROM (par exemple `ARCADE_NEOGEO_MetalSlugStory.gif` au lieu de `mslug.gif`), utilisez l'outil de renommage dans [`Arcade/GIFs/`](https://github.com/MetalCatViking/RetroPixelLED-Lite/tree/main/Arcade/GIFs).

1. Lancez **`Run RetroPixelLED GIF Renamer EN.bat`**.
2. **Option 1 — Renommer les GIFs :** indiquez le dossier source. Le script consulte un catalogue public de noms MAME ([`MAME.dat`](https://github.com/libretro/libretro-database)) et un dictionnaire complémentaire. Vous pouvez choisir une correspondance exacte ou une correspondance exacte et approximative. Les fichiers non identifiés sont déplacés dans `SinResolver\` pour vérification manuelle ; le script ne les renomme pas au hasard.
3. **Option 2 — Copier les GIFs dans les dossiers des systèmes :** indiquez le dossier des ROMs (`C:\RetroBat\roms` par défaut), puis choisissez la destination **4) RetroBat (local Arcade folder)**. Le dossier Arcade local par défaut est `C:\RetroPixelLED\Arcade`.

<img width="1106" height="1204" alt="Capture de l'outil de renommage et de copie des GIFs" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

Vous pouvez également créer vos propres GIFs : le panneau nécessite uniquement une image finale de **128 × 32 pixels**. Si votre collection RetroBat contient des vidéos d'aperçu dans `gamelist.xml`, un outil tiers comme [dmd_gif_converter](https://github.com/red77290/dmd_gif_converter) peut les convertir en GIF et les recadrer. Tout autre outil produisant un GIF de 128 × 32 pixels convient également.

## 6. Logos des systèmes

Vous pouvez utiliser les logos déjà redimensionnés du dossier [`Arcade/Logos Sistemas/Logos/`](https://github.com/MetalCatViking/RetroPixelLED-Lite/tree/main/Arcade/Logos%20Sistemas/Logos). Copiez leur contenu dans `C:\RetroPixelLED\Arcade\`.

Pour traiter vos propres logos, lancez **`Run System Logo Converter EN.bat`** dans le dossier `Arcade/Logos Sistemas/` :

1. **Source :** indiquez le dossier contenant les logos de votre thème.
2. **Destination :** indiquez `C:\Logos`.
3. Copiez ensuite les logos générés dans `C:\RetroPixelLED\Arcade\`.

Le convertisseur redimensionne les images en **128 × 32 pixels** et les enregistre en BMP 24 bits.

## 7. Structure des fichiers sur le PC RetroBat

Tous les fichiers utilisés par RetroBat se trouvent dans le dossier local `C:\RetroPixelLED\` :

- **`C:\RetroPixelLED\Arcade\<system>.txt`** : index des ROMs et de leurs marquises pour un système ;
- **`C:\RetroPixelLED\Arcade\<system>\nom_rom.bmp`** : marquise statique d'un jeu ;
- **`C:\RetroPixelLED\Arcade\<system>\nom_rom.gif`** : marquise animée facultative ;
- **`C:\RetroPixelLED\Arcade\<system>.bmp`** : logo facultatif du système, utilisé si aucune marquise de jeu n'est disponible.

Le moteur et les scripts d'événements sont installés séparément dans le dossier de scripts d'EmulationStation par l'installateur.

#### Exemple de structure

```text
📂 C:\RetroPixelLED\
└── 📂 Arcade\
    ├── 📄 neogeo.txt
    ├── 📄 neogeo.bmp        <- logo du système Neo Geo
    ├── 📂 neogeo\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif     <- marquise animée facultative
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 mame\
        ├── 📄 pacman.bmp
        ├── 📄 tetris.bmp
        └── ...
```

## 8. Profitez des marquises pendant vos parties RetroBat !

# 🕹️ Intégration avec ReplayOS

Le **Mode Arcade** de la version Lite permet à votre matrice LED de fonctionner comme un marquee dynamique avec **ReplayOS**. Le panneau détectera le système et le jeu auxquels vous jouez, vous l'affichera automatiquement et, si le jeu dispose d'un marquee animé préparé, il le lira en boucle pendant que vous jouez.

> [!NOTE]
> **Différence clé avec Batocera/Recalbox :** ReplayOS ne possède pas de système de scripts/événements comme ces frontends, donc ici **rien n'est installé sur l'appareil ReplayOS**. À la place, c'est l'ESP32 lui-même qui interroge régulièrement l'**API REST** de ReplayOS pour savoir quel système et quel jeu sont actuellement actifs, et recherche le marquee correspondant sur sa propre carte SD. Toute la préparation du contenu (images, GIFs, listes) se fait sur votre PC à l'aide des outils PowerShell de cette section, et le résultat est copié sur la carte SD du panneau.

#### Exploitation des Ressources (Scraping)
Tout comme dans Batocera, il n'est pas nécessaire de chercher les marquees à la main jeu par jeu : les outils de cette section téléchargent les images depuis **ArcadeDB** (ou **TheGamesDB**) et les convertissent automatiquement au format requis par le panneau.

## 1. Configuration Critique : API REST et Jeton (Token) de ReplayOS

Pour que le mode **🕹️ Arcade** fonctionne, l'ESP32 doit pouvoir interroger ReplayOS sur le jeu en cours. Cela nécessite trois choses :

1. **Activer le contrôle réseau dans ReplayOS :** dans `REPLAY OPTIONS > SYSTEM`, activez l'option **`NET CONTROL`** (`system_net_control`). Cela ouvre le serveur d'API sur le port `55356`.
2. **Obtenir le Jeton (Net Control Code) :** allez dans `REPLAY OPTIONS > INFORMATION > NET CONTROL CODE`. Il s'agit d'un code numérique à 6 chiffres — vous en aurez besoin pour configurer l'ESP32.
3. **IP fixe pour votre ReplayOS :** l'ESP32 interroge toujours la même adresse IP, celle-ci doit donc être fixe.

> [!TIP]
> **Attribuer une IP fixe à votre ReplayOS :**
> 1. Accédez à la configuration de votre routeur.
> 2. Cherchez la section **DHCP Statique** ou **Assignation d'IP par MAC**.
> 3. Liez l'adresse MAC de votre Raspberry Pi/mini PC exécutant ReplayOS à une IP fixe (ex. : `192.168.1.120`).
> 4. Chaque routeur étant différent, en cas de doute, cherchez sur Google : *"Comment attribuer une IP fixe [modèle de votre routeur]"*.

4. **Configurer l'ESP32 :** Nous pouvons le configurer depuis 2 endroits : l'application (PWA) ou le fichier config.ini (*l'application est recommandée*).
- **Application (PWA) :** Entrez dans la configuration, dans la section *ARCADE*, sélectionnez `ReplayOS`, et dans la section *REPLAYOS*, indiquez l'IP et le Jeton.
  
  <img width="428" height="945" alt="image" src="https://github.com/user-attachments/assets/1724a3a3-f292-4b11-8e54-b547c4ece0fa" />
  <img width="412" height="938" alt="image" src="https://github.com/user-attachments/assets/e649cf1b-1a98-423e-b3c8-5386d39fd0f1" />

- **Config.ini :** Ouvrez le fichier config.ini. Dans la section *[LOGIC]*, indiquez `ARCADE_ENABLE=3` et dans la section *[REPLAY_OS]*, indiquez l'IP attribuée à ReplayOS dans `IP=xxx.xxx.xxx.xxx` et le jeton dans `TOKEN=xxxxxx`.
  <img width="683" height="155" alt="image" src="https://github.com/user-attachments/assets/ec850c72-c111-46ab-bb0b-1c6bd9b24073" /> <img width="510" height="141" alt="image" src="https://github.com/user-attachments/assets/18c8d28c-2b52-48f0-b384-d9e66196467d" />

## 2. Outils PowerShell

Contrairement à Batocera et Recalbox, il n'y a pas d'installateur qui déploie quoi que ce soit sur ReplayOS lui-même. À la place, il y a **deux scripts** qui préparent tout le contenu sur votre PC, prêt à être copié sur la carte SD du panneau :

* **`Script_Marquesinas_ReplayOS.ps1`** — scrappe les ressources depuis ArcadeDB/TheGamesDB, génère les images `.bmp` et crée/audite les listes `.txt` dont l'ESP32 a besoin pour localiser rapidement quels romsets ont un marquee.
* **`Script_RetroPixelLED_GIF_Renamer.ps1`** — si vous possédez déjà une collection de GIFs animés arcade (par exemple, issue d'un pack tiers) avec des noms "humains" au lieu des noms de romset MAME, ce script les renomme automatiquement et les place dans le dossier du système correspondant.

### 🛠️ Prérequis

1. Avoir accès, depuis le PC, à votre dossier de **ROMS** et à la **carte SD** du panneau (insérée dans le PC ou accessible comme lecteur).
2. Télécharger les scripts pour les marquees **`Ejecutar Script Marquesinas_ReplayOS.bat`** et **`Script_Marquesinas_ReplayOS.ps1`**, disponibles [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/Marquesinas).
3. Télécharger les scripts pour renommer les GIFs **`Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`** et **`Script_RetroPixelLED_GIF_Renamer.ps1`**, disponibles [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs).
> [!IMPORTANT]
> Si vous avez téléchargé le dépôt sous forme de fichier `.zip`, veillez à le **décompresser entièrement** avant d'exécuter les scripts.

### 💻 Étape par Étape : Scraping, Images et Listes

1. Exécutez `Ejecutar Script Marquesinas_ReplayOS.bat`.
2. **Option 1 — Scrapper le(s) système(s) :** saisissez le chemin de votre dossier `ROMS` et le chemin où vous souhaitez enregistrer les ressources téléchargées. Le script affichera les sous-dossiers de système trouvés dans le chemin indiqué ; choisissez-en un, plusieurs, ou tapez `TODOS`.
   <img width="1100" height="740" alt="image" src="https://github.com/user-attachments/assets/684f93c5-ab5c-46e2-a792-e1b3535f9ed6" />
   
3. Choisissez la source (ArcadeDB ou TheGamesDB) et les ressources à télécharger (marquee, logo, decal...). Il est recommandé d'utiliser **DECAL** pour créer les marquees. Le script enregistrera les ressources **brutes** dans un dossier de cache réutilisable — il ne génère aucun fichier `.bmp` pour le moment.
   <img width="1101" height="656" alt="image" src="https://github.com/user-attachments/assets/cfb9c33e-77a9-43ba-91e8-c3a84bcea19a" />

4. **Option 2 — Générer les images :** convertit, 100 % hors ligne, ce qui est en cache au format `.bmp` 128×32 avec le trameur RGB565 appliqué, et le dépose dans votre dossier local `Arcade/<système>/`.
   <img width="1098" height="875" alt="image" src="https://github.com/user-attachments/assets/110fc03e-e99e-4e87-840c-660eed8808ff" />

5. **Option 3 — Générer / auditer les listes :** analyse le dossier `Arcade/<système>/` et génère (ou met à jour) le fichier `<système>.txt` que l'ESP32 utilise pour savoir très rapidement quels romsets disposent d'un marquee. Si une liste existait déjà, il vous avertit avant d'y toucher des fichiers en trop ou manquants.
   <img width="1104" height="619" alt="image" src="https://github.com/user-attachments/assets/19718bdc-b849-44b1-916b-7ac81835f1c3" />

7. **Copiez l'intégralité du dossier `Arcade` à la racine de la carte SD de Retro Pixel LED Lite.**

### 🎬 Marquees Animés (GIF)

#### Comment cela fonctionne-t-il ?

- Le panneau affiche le marquee statique (`.bmp`) du jeu lorsqu'il détecte que vous jouez, comme d'habitude.
- Si un fichier `.gif` portant le même nom existe dans le même dossier, le panneau le lit en boucle pendant toute la durée de la partie.
- S'il n'existe pas, il n'y a aucun problème — il conserve le marquee statique sans produire d'erreur.

#### Nommage des fichiers

Le GIF doit porter exactement le même nom que le marquee `.bmp` du même jeu :

```
Arcade/snk_ngo/mslug.bmp   <- vous l'aviez déjà
Arcade/snk_ngo/mslug.gif   <- vous l'ajoutez, même nom
```

Vous pouvez également préparer une séquence de plusieurs GIFs pour le même jeu avec les suffixes `_01`, `_02`, `_03`... Le panneau les lira tous dans l'ordre et recommencera au premier, en boucle continue :

```
Arcade/snk_ng/mslug.gif
Arcade/snk_ng/mslug_01.gif
Arcade/snk_ng/mslug_02.gif
```

> [!TIP]
> Il n'est pas nécessaire que les trois existent — un seul fichier `mslug.gif` fonctionne parfaitement en boucle.

#### Le joker `_default`

Si un jeu ne possède ni marquee propre ni logo de système, le panneau cherche en dernier recours un fichier `_default.bmp` (or `_default.gif`, si vous souhaitez qu'il soit animé) à la racine de `Arcade/`. C'est optionnel, mais cela évite que le panneau ne reste sans affichage pour les jeux non encore préparés.

#### Où trouver des GIFs ?

Si vous avez déjà (ou avez téléchargé) une collection de GIFs arcade avec des noms "humains" au lieu de noms de romsets (par exemple `ARCADE_NEOGEO_MetalSlugStory.gif` au lieu de `mslug.gif`), utilisez le script suivant pour les renommer, téléchargeable [ici](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Arcade/GIFs) :

1. Exécutez `Ejecutar Script_RetroPixelLED_GIF_Renamer.bat`.

2. **Option 1 — Renommer les GIFs :** indiquez le dossier où se trouvent vos GIFs. Le script consulte un catalogue public de noms MAME ([`MAME.dat`](https://github.com/libretro/libretro-database)) pour identifier le romset correspondant à chaque titre, en plus d'un dictionnaire interne pour les cas les plus courants. Vous pouvez choisir entre une correspondance exacte uniquement, ou exacte + approximative (résout plus de cas, avec un peu plus de risques). Ce qui ne peut pas être identifié est déplacé dans un dossier `SinResolver\` pour que vous puissiez le vérifier à la main — le script ne renomme jamais "à l'aveugle".
   <img width="1090" height="830" alt="image" src="https://github.com/user-attachments/assets/58ba389f-367c-4114-b6e1-533018f50e77" />

3. **Option 2 — Copier les GIFs dans les dossiers de système :** une fois renommés, cette option compare les GIFs aux romsets réels de chaque système dans votre dossier `ROMS/` et les copie automatiquement dans `Arcade/<système>/`, aux côtés des fichiers `.bmp` déjà présents.
   <img width="1106" height="1204" alt="image" src="https://github.com/user-attachments/assets/ee3e51a6-2dd7-4297-8a2a-a4486171f60d" />

> [!NOTE]
> Cette seconde option sait également copier vers les chemins de Batocera ou Recalbox (par réseau), si vous préparez des GIFs pour plusieurs frontends en même temps depuis le même PC.

## 3. Structure des fichiers sur la carte SD de Retro Pixel LED Lite

Pour que l'intégration fonctionne, le dossier `Arcade` doit être placé à la **racine** de la carte SD du panneau :

* **`Arcade/<système>.txt`** (liste des romsets disposant d'un marquee pour ce système)
* **`Arcade/<système>/rom_name.bmp`** (marquee statique du jeu, ex. : `mslug.bmp`)
* **`Arcade/<système>/rom_name.gif`** (optionnel : marquee animé du même jeu)
* **`Arcade/<système>.bmp`** (optionnel : logo du système, s'il n'y a pas de marquee propre au jeu)
* **`Arcade/_default.bmp`** (optionnel : joker général s'il n'y a ni marquee ni logo de système)
* **`Arcade/_default.gif`** (optionnel : joker général s'il n'y a ni marquee ni logo de système)

#### Exemple visuel de dossiers :
```
📂 F:\ (SD de RetroPixelLED)
└── 📂 Arcade\
    ├── 📄 snk_ng.txt
    ├── 📄 arcade_fbneo.txt
    ├── 📄 arcade_ng.bmp        <- logo du système Neo Geo
    ├── 📄 _default.bmp          <- joker général
    ├── 📄 _default.gif          <- joker général
    ├── 📂 snk_ng\
    │   ├── 📄 mslug.bmp
    │   ├── 📄 mslug.gif         <- optionnel, marquee animé
    │   ├── 📄 kof98.bmp
    │   └── ...
    └── 📂 arcade_fbneo\
        ├── 📄 dino.bmp
        ├── 📄 avsp.bmp
        └── ...
```

## 4. Profitez de vos marquees tout en jouant sur votre borne de jeu avec ReplayOS !

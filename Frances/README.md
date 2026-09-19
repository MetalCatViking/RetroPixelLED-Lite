# ✨ Retro Pixel LED Lite

<p align="center">
  <img alt="Version" src="https://img.shields.io/badge/version-3.1.3-blue">
  <img alt="Plateforme" src="https://img.shields.io/badge/plateforme-ESP32-informational">
  <img alt="Licence" src="https://img.shields.io/badge/licence-MIT-green">
  <img alt="Statut" src="https://img.shields.io/badge/statut-actif-success">
</p>

<p align="center">
  <a href="https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/README.md">🇪🇸 Español</a> ·
  <a href="https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Frances/README.md">🇫🇷 Français</a> ·
  <a href="https://t.me/RetroPixelLed">✈️ Groupe Telegram</a>
</p>

<p align="center">
  <a href="https://paypal.me/fjgordillo"><img alt="Faire un don avec PayPal" src="https://img.shields.io/badge/☕_Offrez--moi_un_café-PayPal-00457C?style=for-the-badge&logo=paypal&logoColor=white"></a>
</p>

> 💛 Si Retro Pixel LED apporte une touche rétro sympathique à votre intérieur, vous pouvez m'offrir un café en utilisant le bouton ci-dessus. Chaque contribution m'aide à développer de nouvelles fonctionnalités !

---

## 💡 Description du projet

**Retro Pixel LED Lite** est la version haute performance de Retro Pixel LED, conçue pour ceux qui recherchent une stabilité absolue, une vitesse instantanée et un système sans entretien. Contrairement à la version standard, le firmware **LITE** se débarrasse du serveur web embarqué et des connexions permanentes pour dédie 100 % de la puissance de l'ESP32 à une seule tâche : le rendu fluide des GIFs.

Si la branche 2.x.x a apporté le menu OSD, la version **v3.0.0** a marqué un tournant vers l'indépendance matérielle : le panneau LED est devenu un appareil autonome qui n'a plus besoin d'être connecté à un ordinateur pour sa configuration ou sa maintenance. Le fichier `config.ini` et les playlists peuvent être édités directement via l'explorateur Windows ou un client FTP, transformant la carte SD en un lecteur réseau sans fil. Elle intègre également le support natif des télécommandes infrarouges : naviguez dans le menu OSD, ajustez la luminosité et allumez/éteignez le panneau depuis votre canapé.

**Depuis la v3.1.0, vous pouvez contrôler le panneau via une application web (PWA)**, et depuis la **v3.1.2**, directement depuis **Home Assistant**. 🏠

---

## 📑 Table des matières

1. [🆕 Nouveautés de la version actuelle](#-nouveautés-de-la-version-v313-lite)
2. [🚀 Guide rapide](#-guide-rapide-premiers-pas)
3. [🎛️ Fonctionnalités principales](#️-fonctionnalités-principales)
   - [🖥️ Menu OSD](#️-menu-osd-navigation-intelligente)
   - [📱 Application PWA de contrôle à distance](#-pwa--application-de-contrôle-à-distance)
   - [🏠 Home Assistant](#-home-assistant)
   - [🕹️ Mode Arcade](#️-mode-arcade-batocera-recalbox--replayos)
   - [🕒 Horloge et Météo](#-horloge-et-météo)
   - [⏰ Minuteur](#-minuteur)
   - [🌐 Multi-langue](#-multi-langue)
   - [📂 Serveur FTP](#-serveur-ftp)
   - [🔄 Mise à jour OTA](#-mise-à-jour-ota)
4. [⚙️ Installation et configuration](#️-installation-et-configuration)
   - [1. Programmer l'ESP32](#1--programmer-lesp32-web-installer)
   - [2. Préparer la carte SD](#2--préparation-de-la-carte-sd)
   - [3. Le fichier `config.ini`](#3--configuration-via-configini)
   - [4. Fuseau horaire (TZ)](#4--configuration-du-fuseau-horaire-tz)
   - [5. Clé API Météo](#5-%EF%B8%8F-comment-obtenir-votre-cl%C3%A9-api-m%C3%A9t%C3%A9o)
5. [📖 Générateur de playlists (Windows)](#-générateur-de-playlists-windows)
6. [🕹️ Intégration Arcade](#️-intégration-arcade-batocera-recalbox-ou-replayos)
7. [🏠 Intégration Home Assistant](#-intégration-home-assistant-guide-complet)
8. [🧠 Architecture interne / Core Lite](#-architecture-interne--core-lite)
9. [📜 Historique détaillé des modifications](#-historique-détaillé-des-modifications-v300--v313)
10. [🛒 Liste du matériel](#-liste-du-matériel)
11. [🔌 Câblage (Pinout)](#-câblage-pinout)
12. [🛠️ Feuille de route (Roadmap)](#%EF%B8%8F-feuille-de-route-roadmap)
13. [⚖️ Licence et remerciements](#️-licence-et-remerciements)

---

## 🆕 Nouveautés de la version v3.1.3 Lite

- **🕹️ Support du système RePlayOS :** lecture automatique des GIFs et marquises rétro lors du changement de jeu grâce à l'intégration du frontend RePlayOS.
- **⏰ Nouveaux styles visuels pour l'horloge :** options de personnalisation supplémentaires pour l'affichage de l'heure sur le panneau.
- **📡 Cartographie télécommande IR via la PWA :** assignez et configurez les boutons de votre télécommande infrarouge directement depuis l'interface web.

Pour le détail des versions précédentes (marquises GIF en Arcade, reconnexion WiFi, affichage de l'IP dans le menu OSD...), consultez l'[Historique des modifications](#-historique-détaillé-des-modifications-v300--v313).

---

## 🚀 Guide rapide (premiers pas)

Si vous installez Retro Pixel LED Lite pour la première fois, voici la méthode la plus rapide :

1. **Flashez le firmware** avec l'[installateur web](#1--programmer-lesp32-web-installer) — il vous suffit d'utiliser Chrome ou Edge, aucune installation requise sur votre PC.
2. **Préparez la carte MicroSD** au format FAT32 en y copiant le [contenu du dossier `Contenido SD`](#2--préparation-de-la-carte-sd).
3. **Éditez `config.ini`** avec vos identifiants WiFi et vos préférences ([référence complète](#3--configuration-via-configini)).
4. **Allumez le panneau.** Il synchronisera l'heure, chargera vos GIFs et sera immédiatement opérationnel. ✨
5. *(Optionnel)* Installez la [PWA](#-pwa--application-de-contrôle-à-distance) pour le contrôler depuis votre téléphone, ou [intégrez-le à Home Assistant](#-intégration-home-assistant-guide-complet).
6. *(Optionnel)* Si vous utilisez Batocera, Recalbox ou ReplayOS, suivez le [guide d'intégration Arcade](#️-intégration-arcade-batocera-recalbox-ou-replayos) pour afficher les marquises dynamiques.

Le reste de ce document constitue une référence détaillée pour chaque fonctionnalité. 🙂

---

## 🎛️ Fonctionnalités principales

Cette section résume **le rôle** de chaque composant du système. Pour la procédure d'installation étape par étape, rendez-vous dans la section [⚙️ Installation et configuration](#️-installation-et-configuration).

### 🖥️ Menu OSD (navigation intelligente)

Le système se contrôle via un **bouton unique** (ou la télécommande IR), avec une logique de pression qui s'adapte en fonction du menu :

- **Pression courte :**
  - **Dans les menus :** déplacer le curseur / naviguer vers le bas.
  - **En mode veille :** réveille immédiatement le panneau.
- **Pression longue :**
  - **Action générale :** entrer dans les sous-menus ou valider la sélection.
  - **Dans la config du minuteur :** soustrait **-5 minutes** à la valeur actuelle.
- **Pression très longue (> 4 sec) :**
  - **Désactivation manuelle (Override) :** force l'extinction (mode veille), bloquant le minuteur jusqu'au cycle suivant.
- **Maintien continu :**
  - **Dans la config du minuteur :** augmente automatiquement de **+5 minutes** en boucle tant que le bouton est maintenu.

```text
🏠 MENU PRINCIPAL
├── 📂 Playlists
│   ├── 📄 Favoris
│   ├── 📄 Arcade
│   ├── 📄 ...
│   └── 🔙 Retour
├── 📂 Lecture
│   ├── 🖼️ Mode : [GIFs / Horloge]
│   ├── 🔀 Aléatoire : [OUI / NON]
│   ├── 🕹️ Arcade : [OFF / Batocera / Recalbox / ReplayOS]
│   ├── 💬 Texte : [OUI / NON]
│   └── 🔙 Retour
├── ☀️ Luminosité
│   └── Niveau : [5% - 100%]
├── 📶 WiFi : [ON / OFF]
│   ├── 🔄 Activer : [OUI / NON]
│   ├── 🔎 Afficher IP : [OUI / NON]
│   ├── 🏷️ IP : [192.168.1.117]
│   ├── 📱 Contrôle APP : [OUI / NON]
│   └── 🔙 Retour
├── 🕒 Horloge : [ON / OFF]
│   ├── 🔄 Activer : [OUI / NON]
│   ├── 🖼️ Tous les : [1...20] GIFs
│   ├── ⏳ Durée : [5...30] sec
│   ├── 🎨 Style Horloge : [Matrix, Solid, Rainbow, Pulse, Gradient]
│   ├── 🎨 Couleur : [Blanc, Rouge, Vert, Bleu, Jaune, Cyan, Magenta, Orange, Rose]
│   ├── 🔄 Transition : [OUI / NON]
│   └── 🔙 Retour
├── 🌡️ Météo : [ON / OFF]
│   ├── 🔄 Activer : [OUI / NON]
│   └── 🔙 Retour
├── 🕒 Minuteur : [ON / OFF]
│   ├── 🔄 Activar : [OUI / NON]
│   ├── ⏳ ON : [00:00 à 24:00]
│   ├── ⏳ OFF : [00:00 à 24:00]
│   └── 🔙 Retour
├── ⚙️ Paramètres avancés
│   ├── ⚡ Vitesse I2S : [8, 10, 16, 20MHz]
│   ├── 🔄 Rafraîchissement : [30, 60, 90, 120Hz]
│   ├── 🖼️ Buffer : [OUI / NON]
│   ├── 👻 AntiGhost : [1, 2, 3, 4]
│   ├── 🎮 Mappage Télécommande IR : [On, Off, Menu, Valid, Haut, Bas, Lum+, Lum-]
│   ├── ⚠️ Réinitialiser
│   └── 🔙 Retour
├── 🚀 Mise à jour
│   ├── 🔄 Chercher OTA
│   ├── 🔤 Télécharger langues
│   └── 🔙 Retour
├── 📂 Explorateur SD
│   ├── 🔄 Démarrer FTP
│   └── 🔙 Retour
├── 🌐 Langue
│   ├── [ES] Español
│   ├── [EN] English
│   ├── [FR] Français
│   ├── ...
│   └── 🔙 Retour
├── 💾 Sauvegarder
└── 🔙 Quitter
```

---

### 📱 PWA — Application de contrôle à distance

**[👉 Installer ou tester Retro Pixel LED Control](https://fjgordillo86.github.io/RetroPixelLED-Lite/control/)**

Application web moderne, installable sur n'importe quel appareil (téléphone, tablette, PC) connecté au même réseau local que le panneau LED. Elle ne nécessite aucun serveur externe et fonctionne hors ligne une fois installée. 📴

https://github.com/user-attachments/assets/f5231448-7862-4476-901e-ac25ac7f4248

L'interface se divise en **5 sections** :

**1️⃣ Page principale (Accueil)**
- **☀️ Contrôle de la luminosité :** curseur 0-100% avec application instantanée.
- **🎛️ Sélecteur de mode :** bascule en temps réel entre GIF, Horloge ou Texte.
  - **Mode GIF :** sélection de playlist + lecture aléatoire.
  - **Mode Horloge :** 5 styles (Matrix, Solid, Rainbow, Pulse, Gradient) et 9 couleurs.
  - **Mode Texte :** aperçu en direct pendant la saisie, choix de la couleur, de la police et de la vitesse de défilement.
- **🔌 État de la connexion :** indicateur visuel (vert/rouge) du WiFi local.

**2️⃣ Minuteur ⏰**
- Activation / désactivation en un clic.
- Plages horaires d'allumage et d'extinction (format 24h).
- Bouton d'allumage/extinction manuel immédiat.
- État actuel : allumé (✓ vert) ou en veille (● gris).

**3️⃣ Mode Texte défilant**
- Aperçu en direct sur une matrice 26×7 pendant la saisie.
- Sélecteur de couleur : palette de 9 couleurs + code HEX personnalisé.
- **Sélecteur de police :** `Bold`, `SemiBold`, `Regular`, `Light` — *(nouveau dans la v3.1.2)*.
- Vitesse de défilement : curseur de 5 à 200 ms par pas.
- Boutons "▶ Envoyer" et "■ Stopper".

**4️⃣ Mises à jour 🔄**
- **Mise à jour OTA du firmware :** vérification et installation automatique depuis GitHub.
- **Téléchargement des langues :** récupération des fichiers `.json` depuis GitHub vers le dossier `/idioma` de la SD.

**5️⃣ Configuration 🛠**

Édition à distance du fichier `config.ini`, organisée en 7 modules : WiFi, Matériel, Arcade, Texte défilant, Horloge, Météo et Langue.

Redémarrage automatique après enregistrement si la modification le nécessite.

#### ⚙️ Installation de l'application PWA

1. Ouvrez <https://fjgordillo86.github.io/RetroPixelLED-Lite/control/> depuis votre appareil.
2. Cliquez sur l'icône de connexion (⚙) et entrez l'adresse IP locale de votre panneau (ex : `192.168.1.117`).
3. *(Optionnel)* Installez l'application : Chrome/Edge le proposera automatiquement ; sinon, menu (⋮) → "Installer l'application". Sur Safari iOS, Partager (↗) → "Sur l'écran d'accueil".
4. Terminé — l'application est prête à être utilisée. 🎉

#### 📝 Prérequis

- Le panneau et votre appareil doivent être connectés au même réseau WiFi local.
- `CONFI_APP_ENABLE=1` dans le fichier `config.ini`.
- `TEXT_ENABLE=1` pour autoriser l'envoi de messages texte.
- Connexion internet sur le panneau pour les mises à jour OTA et le téléchargement des langues.

---

### 🏠 Home Assistant

Contrôle complet depuis votre tableau de bord Home Assistant via l'**API REST locale** — sans cloud ni dépendance à internet. Fonctionnalités :
- 🟢 **Allumer / Éteindre** le panneau avec un commutateur (`switch`).
- 📊 **Consulter l'état actuel** (mode actif, playlist en cours, etc.).
- 🔄 **Changer de mode** (Horloge / GIF) et de **playlist** instantanément.
- 💬 **Envoyer du texte défilant** en personnalisant la couleur, la vitesse et la **police**.

Consultez le [Guide complet d'intégration Home Assistant](#-intégration-home-assistant-guide-complet) plus bas.

---

### 🕹️ Mode Arcade (Batocera, Recalbox & ReplayOS)

Transforme votre panneau en une marquise dynamique réagissant aux jeux lancés, via deux méthodes : scripts locaux (**Batocera / Recalbox**) ou surveillance réseau (**ReplayOS**).

Affiche automatiquement :
1. **Marquise du jeu :** image `.bmp` 24 bits ou **GIF animé** (y compris des séquences de plusieurs GIFs joués en boucle).
2. **Logo du système :** image `.bmp` lors de la navigation dans les menus.

S'active depuis `Menu → Lecture → Arcade`. Instructions complètes disponibles dans la [section d'intégration Arcade](#️-intégration-arcade-batocera-recalbox-ou-replayos).

---

### 🕒 Horloge et Météo

- **Horloge :** 5 styles (Matrix, Solid, Rainbow, Pulse, Gradient) et 9 couleurs, avec effet de transition de particules. Elle interrompt la galerie de GIFs tous les *x* GIFs pour s'afficher pendant *x* secondes, puis reprend la lecture exacte du GIF interrompu.
- **Météo :** utilise une clé API gratuite OpenWeatherMap pour afficher la température, l'icône météo et un message personnalisé (`WEATHER_MSG`). Voir [comment obtenir votre clé API](#5--comment-obtenir-votre-clé-api-météo).

---

### ⏰ Minuteur

Allumage et extinction programmables selon une plage horaire, avec possibilité de bascule manuelle via le bouton physique ou la PWA.

---

### 🌐 Multi-langue

Système de **dictionnaires dynamiques** : les traductions ne restent pas chargées en RAM en permanence. Elles sont lues depuis la carte SD uniquement lors de l'accès au menu OSD, libérant la mémoire heap pour le moteur GIF.

- **Emplacement :** dossier `/idioma/` sur la SD. Le nom du fichier définit la langue dans le menu : `/idioma/FR.json` → "FR".
- **Structure JSON :** blocs `MENU`, `SUBMENU_XXX`, `ESTADOS`, `CONFIG_INI`.
- **Règles importantes :**
  - 🚫 Évitez les caractères spéciaux non supportés.
  - 📏 Maximum 21 caractères par libellé pour assurer le centrage automatique sur 128px.
  - 🔡 Incluez les deux-points et l'espace si nécessaire (ex: `"mode": "Mode: "`).
  - 💾 Enregistrez en UTF-8 sans BOM.
- **Mise à jour distante :** téléchargez les dernières traductions directement depuis GitHub via le menu OSD (`Mise à jour → Télécharger langues`) ou la PWA.

---

### 📂 Serveur FTP

Serveur de fichiers sans fil pour la maintenance sans retirer la carte MicroSD.

> [!IMPORTANT]
> Recommandé pour éditer **`config.ini`**, les fichiers de langues (`.json`) et les **playlists** (`.txt`). Transférer de vastes collections de GIFs via FTP n'est pas recommandé en raison des limites de vitesse de transfert.

**Activation :** `Menu OSD → Explorateur SD → Démarrer FTP`. Le panneau stoppe la lecture des GIFs et affiche son IP.

**Configuration recommandée (FileZilla) :**

| Paramètre | Valeur |
| :--- | :--- |
| Protocole | FTP basique (non sécurisé) |
| Hôte | Adresse IP du panneau |
| Identifiant / Mot de passe | `admin` / `admin` |
| Port | `21` |
| Connexions simultanées | 1 (limite stricte) |
| Limite de vitesse | 20 KiB/s |

---

### 🔄 Mise à jour OTA

Mise à jour sans fil sans raccorder le panneau au PC :
1. Connectez le panneau au réseau WiFi dans `config.ini`.
2. Allez dans `Menu OSD → Mise à jour → Chercher OTA` (ou via la PWA).
3. Le système télécharge le firmware depuis GitHub et redémarre. 🔃

> [!WARNING]
> Ne coupez pas l'alimentation pendant le processus de mise à jour.

---

## ⚙️ Installation et configuration

### 1. 🚀 Programmer l'ESP32 (Web Installer)

Flashez le firmware directement depuis votre navigateur :

**[👉 Ouvrir l'installateur web Retro Pixel LED Lite](https://fjgordillo86.github.io/RetroPixelLED-Lite/)**

1. Utilisez un navigateur compatible (**Google Chrome** ou **Microsoft Edge**).
2. Connectez votre ESP32 en USB.
3. Cliquez sur **"Install"** et sélectionnez le port COM correspondant.
4. **Important :** cochez la case **"Erase device"** pour effacer complètement la mémoire et éviter les problèmes de fragmentation.

> 💡 **L'ESP32 n'est pas détecté ?** Installez les pilotes USB de votre carte :
> - **Puce CP2102 :** [pilotes Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
> - **Puce CH340/CH341 :** [pilotes SparkFun](https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all)

### 2. 📂 Préparation de la carte SD

Formatez votre carte MicroSD en **FAT32** et copiez-y le contenu du dossier [Contenido SD](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Contenido%20SD) à la racine :

```text
/ (Racine de la SD)
├── gifs/                        <-- Vos dossiers contenant les GIFs (Arcade, Consoles, etc.)
├── idioma/                      <-- Fichiers .json contenant les traductions.
│   ├── ES.json
│   ├── EN.json
│   └── FR.json
├── playlists/                   <-- Listes de lecture générées par le script.
│   ├── Arcade.txt
│   ├── Computers.txt
│   ├── Consoles.txt
│   └── Tous.txt
├── config.ini                   <-- Fichier de configuration principal.
└── Generador de Playlists.bat   <-- Script de génération des playlists.
```

> [!IMPORTANT]
> Si vous ajoutez, supprimez ou déplacez des fichiers dans `/gifs/`, exécutez à nouveau `Generador de Playlists.bat` pour mettre à jour l'indexation.

### 3. 📝 Configuration via `config.ini`

Le fichier `config.ini` se trouve dans le dossier [Contenido SD](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Contenido%20SD) — copiez-le à la racine de la SD et adaptez les paramètres :

```ini
# ============================================================
# 🕹️ RETRO PIXEL LED LITE v3.1.3 - FICHIER DE CONFIGURATION
# ============================================================
# Remarque : Ne laissez pas d'espaces autour du symbole '='.
# Exemple correct : BRIGHTNESS=40

[WIFI_NTP]
# Configuration réseau WiFi
WIFI_ENABLE=1
SSID=Nom_De_Votre_Reseau
PASS=Mot_De_Passe_WiFi
# Configuration du fuseau horaire (Chaine POSIX)
TZ=CET-1CEST,M3.5.0,M10.5.0/3

[HARDWARE]
# Nombre de panneaux LED
PANEL_CHAIN=2
# Ordre des couleurs : RGB, RBG ou GBR
COLOR_ORDER=RGB
# Luminosité (0 à 255)
BRIGHTNESS=38
# Vitesse I2S : 0=8MHz, 1=10MHz, 2=16MHz, 3=20MHz (Turbo)
I2S_SPEED=2
# Rafraichissement minimum (Hz) : 30 à 120
REFRESH_MIN=120
# Double Buffer : 0=OFF, 1=ON (Élimine le scintillement)
DOUBLE_BUFF=0
# Anti-Ghosting : 1 à 4
LATCH_BLANK=1

[LOGIC]
# Mode d'affichage : 0=GIFs, 1=Horloge Seule
PLAY_MODE=0
# Activer l'interface PWA : 0=OFF, 1=ON (Nécessite le WiFi)
CONFI_APP_ENABLE=1
# Sélection du système Arcade : 0=OFF, 1=Batocera, 2=Recalbox, 3=ReplayOS
ARCADE_ENABLE=0
# Activer le texte défilant : 0=OFF, 1=ON (Nécessite le WiFi)
TEXT_ENABLE=1
# Activer l'horloge : 0=OFF, 1=ON (Nécessite le WiFi)
CLOCK_ENABLE=1
# Ordre de lecture : 0=Séquentiel, 1=Aléatoire
RANDOM_MODE=1
# Intervalle : Afficher l'horloge tous les X GIFs
AUTO_CLOCK_INT=6
# Durée d'affichage de l'horloge (secondes)
CLOCK_DURATION=10
# Styles : 0=Matrix, 1=Solid, 2=Rainbow, 3=Pulse, 4=Gradient
CLOCK_STYLE=2
# Effet de transition de particules : 0=OFF, 1=ON
TRANSITION_ENABLE=1
# Couleur de l'horloge (0=Blanc, 1=Rouge, 2=Vert, 3=Bleu, 4=Jaune, 5=Cyan, 6=Magenta, 7=Orange, 8=Rose)
CLOCK_COLOR=4

[WEATHER]
# Activer la météo : 0=OFF, 1=ON (Nécessite le WiFi)
WEATHER_ENABLE=1
# Votre ville (Sans espaces, utilisez '+' : Paris,FR ou Lyon,FR)
CITY=Paris,FR
# Votre clé API gratuite OpenWeatherMap
API_KEY=xxxxxxxxxxxxxxxxxxxxxxx
# Intervalle de mise à jour en MINUTES
WEATHER_INT=60
# Texte personnalisé au-dessus de l'horloge
WEATHER_MSG=Game Room

[LANGUAGE]
# Code Langue (Nom du fichier dans /idioma sans .json : ES, EN, FR...)
LANGUAGE=FR

[IR_REMOTE]
# Codes HEX de la télécommande (Enregistrés automatiquement via le menu OSD)
BTN_ON=F20DFF00
BTN_OFF=E01FFF00
BTN_BRILLO_UP=F609FF00
BTN_BRILLO_DOWN=E21DFF00
BTN_MENU=EA15FF00
BTN_OK=ED12FF00
BTN_SUBIR=E41BFF00
BTN_BAJAR=B34CFF00

[REPLAY_OS]
# Adresse IP attribuée à ReplayOS
IP=192.168.1.101
# Token ReplayOS : SYSTEM > INFORMATION > NET CONTROL CODE
TOKEN=xxxxxx

[END]
```

### 4. 🌍 Configuration du fuseau horaire (TZ)

Pour que l'**horloge** et le **minuteur** fonctionnent correctement, la variable `TZ` doit respecter la norme POSIX.

- **France / Belgique / Suisse / Espagne :** `TZ=CET-1CEST,M3.5.0,M10.5.0/3`
- **Royaume-Uni / Portugal :** `TZ=WET0WEST,M3.5.0/1,M10.5.0`
- **Canada (Est) / Québec :** `TZ=EST5EDT,M3.2.0,M11.1.0`

👉 Consultez la [Base de données POSIX ESP32](https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv) pour obtenir la chaîne exacte correspondant à votre région.

### 5. ☁️ Comment obtenir votre clé API Météo

1. Créez un compte gratuit sur [OpenWeatherMap.org](https://openweathermap.org/).
2. Accédez à votre profil → **"My API Keys"**.
3. Générez une nouvelle clé.
4. **Attention :** l'activation d'une nouvelle clé peut prendre entre 30 minutes et 2 heures. ⏳
5. Copiez votre clé dans la variable `API_KEY=` de votre fichier `config.ini`.

---

## 📖 Générateur de playlists (Windows)

Le script `Generador de Playlist v1.0.1.bat` crée des sélections personnalisées de GIFs. Il se trouve dans le dossier [Contenido SD](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Contenido%20SD).

1. Placer le fichier `.bat` à la racine de la SD à côté du dossier `gifs`.
2. Double-cliquez pour exécuter l'outil.
3. Saisissez les numéros de dossiers séparés par des virgules (ex: `3,4,10`) ou tapez `TODO` pour inclure l'ensemble.
4. Entrez le nom de votre playlist (ex: `MesFavoris`).
5. Le fichier `playlists/MesFavoris.txt` est généré automatiquement.

<img width="514" height="565" alt="Script PlayList" src="https://github.com/user-attachments/assets/3c600615-5539-4430-af7b-26cd219fc7fe" />

---

## 🕹️ Intégration Arcade (Batocera, Recalbox ou ReplayOS)

Activez l'option dans `Menu → Lecture → Arcade` pour afficher les marquises de vos jeux :

```text
🏠 MENU PRINCIPAL
├── 📂 Lecture
│   ├── 🖼️ Mode : [GIFs / Horloge]
│   ├── 🔀 Aléatoire : [OUI / NON]
│   ├── 🕹️ Arcade : [OFF / Batocera / Recalbox / ReplayOS]   <-- SÉLECTIONNEZ ICI
│   └── 🔙 Retour
```

> [!IMPORTANT]
> Pour installer les scripts requis sur votre système d'émulation, suivez la documentation dédiée :
>
> **[👉 Guide d'installation pour Batocera](https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Frances/README_BATOCERA.md)**
>
> **[👉 Guide d'installation pour Recalbox](https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Frances/README_RECALBOX.md)**
>
> **[👉 Guide d'installation pour ReplayOS](https://github.com/fjgordillo86/RetroPixelLED-Lite/blob/main/Frances/README_REPLAYOS.md)**

---

## 🏠 Intégration Home Assistant (Guide complet)

Intégrez **Retro Pixel LED Lite** dans **Home Assistant** via son API REST locale.

Fonctionnalités :
- 🟢 Commutateur d'alimentation (`switch`).
- 📊 Monitoring (mode actif, playlist chargée, IP).
- 🔄 Sélecteur de modes et playlists.
- 💬 Envoi de texte défilant avec sélection des couleurs, vitesses et **polices**.

### 📦 Installation dans Home Assistant

Si vous utilisez des "packages", copiez `retropixel.yaml` depuis le dossier **[Home Assistant](https://github.com/fjgordillo86/RetroPixelLED-Lite/tree/main/Home%20Asisstant)** vers `/config/packages/`. Sinon, collez son contenu dans votre `configuration.yaml`.

> ⚠️ **Important :** Remplacez l'adresse IP `192.168.31.210` par celle de votre ESP32, et adaptez le nom des playlists.

Une carte d'interface Lovelace prête à l'emploi (`entities.yaml`) est également fournie.

<img width="822" height="1005" alt="Captura HA" src="https://github.com/user-attachments/assets/9294b479-f428-4c68-9fcc-c871ad2e88e4" />

---

## 🧠 Architecture interne / Core Lite

Détails techniques :

- **📡 Cartographie dynamique des touches IR :** configuration simplifiée des boutons enregistrée dans `config.ini`.
- **📂 Gestion FTP sans fil :** mise à jour des playlists et paramètres à distance.
- **Système de protection RAM :** gestion dynamique de la mémoire heap pour prévenir les réinitialisations inopinées lors de l'allocation des buffers.
- **Recherche binaire (Arcade) :** indexation rapide pour rechercher les marquises parmi des milliers de fichiers sur la carte SD en quelques millisecondes.
- **Gestion dynamique de la mémoire :** bascule automatique entre le *Double Buffer* (pour les GIFs à fréquence d'images élevée) et le *Single Buffer* (pour les images fixes ou de haute résolution).
- **API HTTP en temps réel :** points d'accès REST légers pour la synchronisation avec Batocera, Recalbox, ReplayOS et Home Assistant.
- **Centrage intelligent du texte :** calcul automatique de la largeur en pixels pour centrer le texte défilant (`offset + 64px`).
- **Mode WiFi économe :** la connexion réseau s'active brièvement pour la synchronisation horaire et météo avant de repasser hors ligne, garantissant **0 ms de latence** sur le rendu.
- **Bannière d'information dynamique :** repositionnement de l'horloge vers le bas (`startY=9`) lorsque la météo est activée.
- **Système d'icônes Jour/Nuit :** icônes adaptées aux conditions météorologiques et à l'heure locale.
- **Lecture intelligente :** interruption ponctuelle de la boucle GIF pour afficher l'horloge avant de reprendre le GIF exactement là où il s'était arrêté.
- **Fallback Hors-Ligne :** maintien de l'heure via le RTC interne en cas de perte du signal WiFi.

---

## 📜 Historique détaillé des modifications (v3.0.0 → v3.1.3)

| Fonctionnalité | Détails techniques | Avantage |
| :--- | :--- | :--- |
| **🕹️ Support RePlayOS** | Intégration de l'API RePlayOS pour la gestion des marquises et logos. | **Expérience Arcade dynamique.** Mise à jour automatique des images lors des changements de jeux. |
| **⏰ Styles d'horloge** | Ajout de nouveaux modes de rendu et thèmes visuels. | **Personnalisation accrue** selon vos préférences. |
| **📡 Cartographie IR via PWA** | Capture et configuration des touches de la télécommande depuis l'interface web. | Configuration simplifiée de la télécommande. |
| **🏠 Intégration Home Assistant** | Points d'accès REST (`GET /status`, `POST /control`, `/playlist`, `/texto`, `/timer/toggle`) et fichier YAML dédié. | **Domotique intégrée.** Contrôle de l'allumage, des modes et du texte depuis HA. |
| **🔤 Polices de caractères** | 4 polices au choix (`Bold`, `SemiBold`, `Regular`, `Light`) pour le texte défilant via API, PWA et HA. | **Style visuel personnalisé** pour vos messages. |
| **🎛️ Interface PWA** | Application web progressive avec 5 modules de contrôle et éditeur de `config.ini`. | **Contrôle à distance complet** sans dépendance réseau externe. |
| **☀️ Ajustement de la luminosité** | Curseur 0-100% avec prise en compte immédiate. | Adaptation rapide à la lumière ambiante. |
| **🔤 Support UTF-8** | Convertisseur UTF-8 vers Latin-1 pour gérer l'ensemble des caractères accentués. | Affichage correct des caractères internationaux. |
| **🎨 Changement de mode à chaud** | Bascule rapide entre les modes (GIF / Horloge / Texte) via la PWA sans redémarrage. | Transitions instantanées. |
| **🎞️ Playlists dynamiques** | Changement de la liste de lecture active depuis l'application web. | Gestion facilitée des collections de GIFs. |
| **⏰ Minuteur programmable** | Plages horaires d'allumage et d'extinction configurables avec reprise manuelle. | Gestion automatisée de l'alimentation. |
| **🔄 Mises à jour sans fil (OTA & Langues)** | Téléchargement du firmware et des langues depuis GitHub sans retirer la carte SD. | Maintenance du système à distance. |
| **⚙️ Configuration distante** | Éditeur complet du fichier `config.ini` depuis la PWA. | Paramétrage fluide sans manipulation de fichiers. |
| **💬 Envoi de texte défilant** | Endpoints HTTP POST + PWA pour diffuser des messages personnalisés. | Diffusion de messages en temps réel. |
| **💥 Effet de particules** | Animation de particules pour l'affichage et la disparition de l'horloge. | Transitions visuelles fluides. |
| **🎨 Personnalisation des couleurs OSD** | Sélecteur de couleurs interactif sauvegardé dans la mémoire de l'appareil. | Changement de style visuel sans éditer de fichier. |
| **⚡ Rendu horloge optimisé** | Passage en *Single Buffer* pour le rendu de l'horloge. | Élimine les scintillements d'affichage. |
| **🧠 Optimisation de la mémoire** | Migration des `String` vers des tableaux `char[]` et utilisation optimisée des macros `PSTR()` / `F()`. | Évite la fragmentation de la RAM et préserve les ressources du DMA. |
| **🛡️ Système Anti-Panic** | Vérification automatique de l'allocation mémoire à l'initialisation de l'affichage. | Prévient les plantages (`StoreProhibited`) liés à l'usage du WiFi. |
| **🖱️ Logique d'appui prolongé** | Validation des appuis longs pour éviter les fausses manipulations. | Navigation plus sûre dans l'OSD. |
| **📂 Serveur FTP intégré** | Gestion des fichiers à distance sur la carte SD. | Édition des playlists et de la configuration sans lecteur de carte. |
| **📡 Navigation par télécommande IR** | Prise en charge des télécommandes infrarouges pour la navigation OSD. | Contrôle à distance simple. |
| **🎨 Ordre des couleurs configurable** | Option `colorOrder` (RGB/RBG/GBR) ajustable dans `config.ini`. | Compatibilité étendue avec les différents panneaux HUB75. |

---

## 🛒 Liste du matériel

Matériel recommandé et testé :

- **Microcontrôleur :** [ESP32 DevKit V1 (30 broches) - AliExpress](https://es.aliexpress.com/item/1005005704190069.html)
- **Panneau LED Matrix (HUB75) :** [Panneau P2.5 / P4 RGB Matrix - AliExpress](https://es.aliexpress.com/item/1005008479388445.html)
- **Lecteur de carte Micro SD :** [Module Adaptateur Micro SD (SPI) - AliExpress](https://es.aliexpress.com/item/1005005591145849.html)
- **Shield PCB ESP32 vers Matrix :** [Carte DMDos V3 - Mortaca](https://www.mortaca.com/) *(Optionnel, évite les soudures et intègre un port SD)*
- **Récepteur Infrarouge :** [Module récepteur IR universel - AliExpress](https://es.aliexpress.com/item/1005005343424296.html)
- **Bouton poussoir :** [Bouton poussoir momentané DS-316 - AliExpress](https://es.aliexpress.com/item/4000888761296.html)
- **Alimentation :** Alimentation 5V DC (2A minimum recommandé pour un panneau 64x32).

---

## 🔌 Câblage (Pinout)

Si vous utilisez la carte **DMDos V3**, le câblage est pré-routé — vous pouvez passer à la section suivante.

#### 📂 Lecteur Micro SD (Bus SPI)
| Broche SD | Broche ESP32 | Fonction |
| :--- | :--- | :--- |
| **CS** | GPIO 5 | Sélection du composant (Chip Select) |
| **CLK** | GPIO 18 | Horloge bus SPI (Clock) |
| **MOSI** | GPIO 23 | Sortie données (Master Out) |
| **MISO** | GPIO 19 | Entrée données (Master In) |
| **VCC** | 3.3V | Alimentation |
| **GND** | GND | Masse |

#### 🖼️ Panneau LED RGB HUB75
| Broche Panneau | Broche ESP32 | Fonction |
| :--- | :--- | :--- |
| **R1** | GPIO 25 | Données Rouge (Haut) |
| **G1** | GPIO 26 | Données Vert (Haut) |
| **B1** | GPIO 27 | Données Bleu (Haut) |
| **R2** | GPIO 14 | Données Rouge (Bas) |
| **G2** | GPIO 12 | Données Vert (Bas) |
| **B2** | GPIO 13 | Données Bleu (Bas) |
| **A** | GPIO 33 | Ligne d'adresse A |
| **B** | GPIO 32 | Ligne d'adresse B |
| **C** | GPIO 22 | Ligne d'adresse C |
| **D** | GPIO 17 | Ligne d'adresse D |
| **E** | GND | Masse |
| **CLK** | GPIO 16 | Horloge de rafraîchissement |
| **LAT** | GPIO 4 | Verrouillage (Latch) |
| **OE** | GPIO 15 | Activation de la sortie (Luminosité) |

#### 🕹️ Contrôles (Bouton et Infrarouge)
| Composant | Broche ESP32 | Fonction |
| :--- | :--- | :--- |
| **Bouton (Signal)** | GPIO 21 | **Multifonction :** Pression courte (Naviguer) / Pression longue (Valider / Veille). |
| **Bouton (Masse)** | GND | Référence de masse. |
| **Récepteur IR (Data)**| GPIO 34 | Entrée du signal télécommande (Protocole NEC). |
| **Récepteur IR (VCC)** | 3.3V | Alimentation du module. |
| **Récepteur IR (GND)** | GND | Référence de masse. |

<img width="769" height="716" alt="image" src="https://github.com/user-attachments/assets/11fef006-59f3-405f-b00a-a32c9bba7bc5" />

---

### 📂 Configuration du serveur FTP (Explorateur SD)

Permet d'accéder sans fil aux fichiers stockés sur la carte SD depuis votre réseau local.

> [!IMPORTANT]
> **Utilisation recommandée :** modification de `config.ini`, des dictionnaires de langues (`.json`), des playlists (`.txt`) et de petits fichiers. Il n'est pas recommandé de transférer de grandes collections de GIFs par ce biais.

**Démarrer le serveur FTP :**
1. Rendez-vous dans le `Menu OSD → Explorateur SD`.
2. Sélectionnez **Démarrer FTP**.
3. L'affichage des GIFs s'interrompt et l'**adresse IP** du panneau s'affiche (ex: `192.168.1.109`).

**Configuration sous FileZilla :**
- **Protocole :** FTP - Protocole de Transfert de Fichiers
- **Hôte :** Adresse IP affichée sur le panneau
- **Type d'authentification :** Normale
- **Identifiant / Mot de passe :** `admin` / `admin`
- **Port :** `21`
- **Nombre de connexions simultanées :** 1

<img width="545" height="227" alt="image" src="https://github.com/user-attachments/assets/1b537615-3e39-48ba-9eb0-48b03931c5f9" />
<img width="544" height="193" alt="image" src="https://github.com/user-attachments/assets/ba4c85bc-920a-48c9-83d8-99b96ecbc57f" />

**Dans les options de FileZilla → Transferts :**
- Nombre maximal de transferts simultanés : 1.
- Limite de vitesse : 20 KiB/s en émission et réception.

<img width="841" height="522" alt="image" src="https://github.com/user-attachments/assets/e90d3e84-9c93-45c0-b942-8b601db40041" />

**Remarques importantes :**
- La lecture des GIFs est mise en pause tant que le serveur FTP est actif.
- Pour quitter le mode FTP, appuyez sur le bouton physique ou la touche "OK" de la télécommande.
- Évitez de couper l'alimentation pendant le transfert de fichiers pour éviter de corrompre la carte SD.

---

## 🛠️ Feuille de route (Roadmap)

### ⚡ Performances et fonctionnalités de base

*(À définir)*

### 🎨 Améliorations visuelles et connectivité

*(À définir)*

---

## ⚖️ Licence et remerciements

Ce projet est distribué sous licence libre **MIT**.

Un grand merci aux auteurs des bibliothèques utilisées :
- **Bitbank2** pour la bibliothèque `AnimatedGIF`.
- **Mrfaptastic** pour le moteur Matrix DMA ESP32.
- La **Communauté Telegram DMDos**, pour leurs idées et retours d'expérience.
- **RpiTe@m** pour le pack de 600 GIFs et la collection de 11 000 GIFs disponible [ici](https://www.neo-arcadia.com/forum/viewtopic.php?t=67065).
- **shan-aya** pour la traduction française et l'utilitaire [DMD_GIF_converter](https://github.com/shan-aya/DMD_GIF_converter).
- **joseAveleira** pour l'effet de particules de l'horloge. [GitHub](https://github.com/joseAveleira/RelojPixel/tree/main)

---

<p align="center">Projet développé avec passion et beaucoup de GIFs rétro. Si ce projet vous plaît, n'hésitez pas à lui ajouter une ⭐ sur GitHub !</p>

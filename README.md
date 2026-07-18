# GUImp

Un éditeur d'images en C, dans l'esprit de GIMP, construit au-dessus d'une bibliothèque d'interface graphique développée entièrement de zéro. Pas de GTK ni de Qt : SDL2 sert uniquement à afficher les pixels, et une bibliothèque maison gère tout le reste.

![Interface de GUImp](Screenshot.png)

**[Voir la démo en vidéo sur YouTube](https://www.youtube.com/watch?v=oCEkPwQBrj0)**

## Présentation

Le projet est en réalité composé de deux parties :

1. `libui`, une bibliothèque d'interface graphique complète, écrite de zéro au-dessus de SDL2 : fenêtres, widgets, événements, thèmes, glisser-déposer.
2. GUImp, un éditeur d'images léger, construit uniquement sur `libui`.

La contrainte du sujet est stricte : l'éditeur ne doit jamais appeler SDL, le système ou une autre bibliothèque graphique directement. Chaque pixel, chaque clic et chaque menu passe par `libui`. L'intérêt du projet ne se limite donc pas à l'éditeur, mais surtout à la bibliothèque qui le fait tourner.

## La bibliothèque `libui`

- Widgets : boutons, labels, images, champs de saisie, zones de texte, cases à cocher, boutons radio, curseurs, listes déroulantes, barres de progression, et conteneurs imbriqués librement.
- Fenêtres : fenêtres génériques et boîtes de dialogue modales prêtes à l'emploi (`OK`, `OK / Annuler`).
- Événements et callbacks : associer son propre comportement aux clics, survols, touches clavier, changements de curseur, validation de saisie, fermeture de fenêtre et dépôt de fichier.
- Prefabs : blocs réutilisables comme les sélecteurs de fichier et de police, les en-têtes et les menus standards.
- Divers : fenêtres et éléments défilables, raccourcis clavier par élément, glisser-déposer.

### Des widgets construits comme des objets

Il n'y a pas de mot-clé `class` en C, alors `libui` construit ses objets à la main. Chaque widget, quel que soit son type, est la même structure qui porte son comportement à travers des pointeurs de fonctions :

```c
typedef struct s_widget
{
    char                name[128];
    struct s_core       *core;
    t_widget            *owning_window;
    struct s_widget     **childs;          // widgets imbriqués
    struct s_widget     *parent;
    e_widget_type       type;
    e_widget_state      state;
    SDL_Color           colors[NUM_STATE];
    bool                is_draggable;
    bool                is_resizable;
    SDL_Point           scroll;
    /* ... géométrie, texture, drapeaux, curseur ... */

    // Comportement de ce widget, câblé comme une vtable :
    void  (*event)(struct s_widget *widget);
    void  (*update)(struct s_widget *widget);
    void  (*render)(struct s_widget *widget);
    void  (*destroy)(struct s_widget *widget);
    void  (*build)(struct s_widget *widget);
}   t_widget;
```

Un bouton, un curseur, une fenêtre ou le canevas sont tous ce même type : ce qui les distingue, ce sont les fonctions branchées sur `build` / `render` / `event` / `update` / `destroy`. Ajouter un nouveau type de widget revient à écrire ces fonctions et à les attacher, sans dupliquer de code. C'est du polymorphisme et de l'encapsulation, exprimés avec des pointeurs de fonctions plutôt qu'avec une fonctionnalité du langage.

### Un thème décrit en CSS

Plutôt que de coder les couleurs en dur, `libui` lit une feuille de style. Voici le thème réellement livré avec GUImp :

```css
window {
    background-color: #1a1a2e;
}

button {
    background-color: #16213e;
    hover-color: #116cdb;
    click-color: #533483;
    outline: 10;
}

slider {
    color: #ffffff;
    fill-color: #f5e509;
    outline: 2;
}
```

Le parseur (`css_parser.c`) lit le fichier, et `css_apply.c` associe chaque sélecteur (`window`, `button`, `box`, `slider`, `text`, `canvas`) aux widgets correspondants. Les couleurs acceptent les formats `#RRGGBB` et `#AARRGGBB`.

## L'éditeur GUImp

Construit uniquement avec `libui`, l'éditeur propose :

- Outils de dessin : pinceau, gomme, lignes droites, et un menu de formes (lignes, rectangles, carrés, cercles) en mode plein ou creux.
- Navigation : outils zoom et main, épaisseur et rayon de pinceau réglables.
- Couleur : roue RGB, curseurs RGB, réglage de luminosité, et pipette pour prélever une couleur directement sur l'image.
- Remplissage : pot de peinture.
- Texte : insertion de texte avec choix de la couleur, de la police et de la taille.
- Stickers : dépôt d'images prédéfinies sur le canevas.
- Calques et presse-papiers : nouveaux calques, couper, copier, coller, effacer.
- Fichiers : import et export en PNG et JPEG.
- Confort : le curseur change selon l'outil sélectionné, et la touche ESC ferme tout proprement.

## Compilation et lancement

Dépendances (Debian / Ubuntu) :

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

Compilation et exécution :

```bash
make && ./guimp
```

## Organisation du projet

```
guimp/
├── main.c            # logique de l'éditeur et assemblage de l'interface
├── draws.c           # fonctions de dessin et de rendu
├── build.c           # construction des fenêtres et de la mise en page
├── css_parser.c      # parseur de la feuille de style
├── css_apply.c       # application des styles aux widgets
├── utils.c
├── about.c
├── guimp.h
├── style.css         # thème de l'interface, modifiable
├── assets/           # curseurs, icônes, stickers
└── libui/            # la bibliothèque d'interface maison
```

## Contexte

Projet réalisé à l'École 42, dont l'objectif était de concevoir une bibliothèque d'interface réutilisable, puis de le prouver en construisant une vraie application par-dessus.

L'ensemble de `libui` et de GUImp a été écrit entièrement à la main, sans assistance d'IA.

Auteur : [bvaujour](https://github.com/bvaujour)

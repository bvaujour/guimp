<h1 align="center">GUImp</h1>

<p align="center">
  <b>A GIMP-style image editor written in C, running on a graphical toolkit built entirely from scratch.</b><br>
  No GTK. No Qt. Just SDL2 for pixels, and a homemade UI library (with its own CSS theming engine) for everything else.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white">
  <img src="https://img.shields.io/badge/graphics-SDL2-1D4ED8">
  <img src="https://img.shields.io/badge/UI-custom%20toolkit-533483">
  <img src="https://img.shields.io/badge/theming-CSS%20engine-116cdb">
  <img src="https://img.shields.io/badge/42-School-black">
</p>

<p align="center">
  <a href="https://www.youtube.com/watch?v=oCEkPwQBrj0">
    <img src="Screenshot.png" width="720" alt="GUImp interface — click to watch the demo">
  </a>
  <br>
  <i>Click the screenshot to watch the demo on YouTube.</i>
</p>

---

## What is this?

GUImp is really **two projects in one**:

1. **`libui`** — a complete graphical user interface library, written from zero on top of SDL2. Windows, widgets, events, theming, drag and drop: the whole framework, hand built.
2. **GUImp** — a lightweight image editor, in the spirit of GIMP, built **exclusively** on `libui`.

The hard rule of the project: the editor is **never allowed to call SDL, the OS, or any other graphics library directly.** Every pixel, every click and every menu goes through `libui`. So the interesting part is not just the editor, it is the toolkit underneath it.

---

## Highlights

- 🧱 **A GUI toolkit from scratch** (`libui`): windows, modal dialogs, buttons, menus, labels, text fields and text areas, checkboxes, radio buttons, sliders, drop down lists, progress bars, and nested widgets.
- 🎨 **A homemade CSS theming engine.** Widget colors and styles are described in a plain `style.css` file, parsed and applied at runtime. Change the look of the whole app without touching a line of C.
- 🧩 **Object oriented design in pure C.** Each widget carries its own `build` / `update` / `render` / `event` / `destroy` function pointers, wired to an event and callback system (`onclick`, `onslider`, `onfiledropped`, and more).
- 🖱️ **Real interaction.** Mouse, keyboard and focus events, per tool cursors, scrolling, and drag and drop onto both widgets and windows.
- 🖼️ **A genuine image editor** on top of it all: brush, shapes, fill, color picker, text, stickers, layers and PNG / JPEG import and export.

---

## The toolkit: `libui`

`libui` is the layer that makes GUImp possible. It provides:

- **Widgets:** buttons, labels, images, editable inputs, text areas, checkboxes, radio buttons, sliders, drop down lists, progress bars, and containers that nest freely.
- **Windows:** generic windows plus ready made modal dialogs (`OK`, `OK / Cancel`).
- **Events and callbacks:** bind your own behavior to clicks, releases, hover, key presses, slider changes, input validation, window close and file drop.
- **Prefabs:** reusable building blocks such as file and font selection dialogs, headers and standard menus.
- **Extras:** scrollable windows and elements, per element hotkeys, and drag and drop.

### Widgets are objects (function pointers as methods)

There is no `class` keyword in C, so `libui` builds objects by hand. Every widget, whatever its type, is the **same struct** carrying its own behavior through function pointers:

```c
typedef struct s_widget
{
    char                name[128];
    struct s_core       *core;
    t_widget            *owning_window;
    struct s_widget     **childs;          // nested widgets
    struct s_widget     *parent;
    e_widget_type       type;
    e_widget_state      state;
    SDL_Color           colors[NUM_STATE];
    bool                is_draggable;
    bool                is_resizable;
    SDL_Point           scroll;
    /* ... geometry, texture, flags, cursor ... */

    // Behavior of this widget, wired like a vtable:
    void  (*event)(struct s_widget *widget);
    void  (*update)(struct s_widget *widget);
    void  (*render)(struct s_widget *widget);
    void  (*destroy)(struct s_widget *widget);
    void  (*build)(struct s_widget *widget);
}   t_widget;
```

A button, a slider, a window or the canvas are all this one type: what makes them different is the set of functions plugged into `build` / `render` / `event` / `update` / `destroy`. Adding a new widget means writing those handlers and attaching them, no shared code to fork. This is polymorphism and encapsulation, expressed with function pointers instead of a language feature.

### Theming with CSS

Instead of hard coding colors, `libui` reads a stylesheet. This is the actual theme shipped with GUImp:

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

The parser (`css_parser.c`) reads the file, the applier (`css_apply.c`) maps each selector (`window`, `button`, `box`, `slider`, `text`, `canvas`) onto the matching widgets. Colors accept both `#RRGGBB` and `#AARRGGBB`.

---

## The editor: GUImp

Built with `libui` only, the editor offers:

- **Drawing tools:** brush, eraser, straight lines, and a shape menu (lines, rectangles, squares, circles) in filled or hollow modes.
- **Navigation:** zoom and hand tools, adjustable brush thickness and radius.
- **Color:** RGB wheel, RGB sliders, brightness control, and an eyedropper to pick colors straight from the image.
- **Fill:** paint bucket.
- **Text:** insert text with a choice of color, font and font size.
- **Stickers:** drop predefined images onto the canvas.
- **Layers and clipboard:** new layers, cut, copy, paste and clear.
- **Files:** import and export in **PNG** and **JPEG**.
- **Feel:** the cursor changes with the selected tool; `ESC` closes everything cleanly.

---

## Build and run

**Dependencies** (Debian / Ubuntu):

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

**Build and launch:**

```bash
make && ./guimp
```

---

## Project layout

```
guimp/
├── main.c            # editor logic and UI assembly
├── draws.c           # drawing / rendering helpers
├── build.c           # window and layout construction
├── css_parser.c      # CSS stylesheet parser
├── css_apply.c       # applies parsed styles to widgets
├── utils.c
├── about.c
├── guimp.h
├── style.css         # the editable interface theme
├── assets/           # cursors, icons, stickers
└── libui/            # the custom GUI toolkit
```

---

## Context

Built as a project at [42 School](https://42.fr), where the goal was to design a reusable UI library and then prove it by building a real application on top of it.

**Author:** [bvaujour](https://github.com/bvaujour)

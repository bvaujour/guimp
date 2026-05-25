/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guimp.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xacharle <xacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:25:14 by injah             #+#    #+#             */
/*   Updated: 2026/05/04 19:24:46 by xacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUIMP_H
# define GUIMP_H

# include "libui/libui.h"
# include "libui/libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>

# define MAX_STICKERS	1000

# define MAX_SPACING	400
# define MAX_THICKNESS	50
# define MAX_WIDTH		1024
# define MAX_HEIGHT		1024
# define MAX_RADIUS		512
# define MAX_LAYERS		20
# define MAX_CANVAS		10
# define MIN_SPACING	1
# define MIN_THICKNESS	1
# define MIN_WIDTH		1
# define MIN_HEIGHT		1
# define MIN_RADIUS		1

# define TITLES_FONT_SIZE		20
# define BUTTONS_FONT_SIZE		16
# define TOOL_WINDOW_WIDTH		400

enum	e_color
{
	RED,
	GREEN,
	BLUE,
	ALPHA,
	BRIGHTNESS,
	NUM_COLOR
};

enum	e_tool
{
	SELECT,
	PENCIL,
	RECT,
	LINE,
	CIRCLE,
	BRUSH,
	BUCKET,
	ERASER,
	TEXT_TOOL,
	NUM_TOOL
};

enum	e_header
{
	FILE_BUTTON,
	EDIT_BUTTON,
	HELP_BUTTON,
	NUM_HEADER
};

enum	e_file_menu
{
	NEW_BUTTON,
	NEW_LAYER_BUTTON,
	IMPORT_FILE_BUTTON,
	EXPORT_FILE_BUTTON,
	EXIT_BUTTON,
	NUM_FILE_MENU
};

enum	e_edit_menu
{
	COPY_BUTTON,
	PASTE_BUTTON,
	CUT_BUTTON,
	CLEAR_BUTTON,
	PARAMETER_BUTTON,
	FONT_BUTTON,
	NUM_EDIT_MENU
};

enum	e_help_menu
{
	ABOUT_BUTTON,
	NUM_HELP_MENU
};

enum	e_onoff
{
	FILL_MODE,
	DRAW_MODE,
	NUM_ONOFF
};


typedef struct	s_vector2
{
	int	x;
	int	y;
}				t_vector2;

typedef	struct	s_rgba
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}				t_rgba;

typedef struct	s_parameter
{
	int				width;
	int				height;
	int				thickness;
	int				spacing;
	int 			radius;
	t_prefab_slider	width_slider;
	t_prefab_slider	height_slider;
	t_prefab_slider	thickness_slider;
	t_prefab_slider	spacing_slider;
	t_prefab_slider	radius_slider;
	t_prefab_button onoff_buttons[NUM_ONOFF];
}				t_parameter;

typedef struct	s_modal_export
{
	t_widget		*window;
	t_prefab_button	png_jpg_button;
	t_prefab_input	input;
}				t_modal_export;

typedef struct	s_modal_new
{
	t_widget		*window;
	t_prefab_input	width;
	t_prefab_input	height;
}				t_modal_new;

typedef struct	s_area
{
	int	x1;
	int	y1;
	int	width;
	int	height;
}				t_area;

typedef struct	s_drawing
{
	int		last_x;
	int		last_y;
	bool	is_drawing;
}				t_drawing;

typedef struct	s_color_widgets
{
	t_widget		*sliders_box;
	t_widget		*wheel;
	t_prefab_box	container;
	t_prefab_slider	sliders[NUM_COLOR];
	t_widget		*pipette_box;
	t_widget		*pipette;
	bool			pipette_active;
}				t_color_widgets;

typedef struct	s_header_widgets
{
	t_prefab_header	header;
	t_prefab_button	file_menu_buttons[NUM_FILE_MENU];
	t_prefab_button	edit_menu_buttons[NUM_EDIT_MENU];
	t_prefab_button	help_menu_buttons[NUM_HELP_MENU];

}				t_header_widgets;

typedef struct	s_canvas_widgets
{
	t_widget	*container;
	t_widget	*upgrade_layer;
	t_widget	*downgrade_layer;
	t_widget	*add_layer;
	t_widget	*clear_layer;
	t_widget	*close_layer;
	t_widget	*render_box;
	t_widget	*canvas;
	t_widget	*layers[MAX_LAYERS];
	t_widget	*layers_buttons_box;
	t_widget	*layers_buttons[MAX_LAYERS];
	int			active_layer;
	int			nb_layer;
	t_widget	*zoom_infos_text;
	char		zoom_infos_str[50];
	t_vector2	resolution;
}				t_canvas_widgets;

typedef struct	s_stickers_widgets
{
	t_prefab_box	container;
	t_widget		*textures[MAX_STICKERS];
	t_widget		*sticker_buttons[MAX_STICKERS];	
}				t_stickers_widgets;

typedef struct	s_windows_widgets
{
	t_widget		*tool;
	t_widget		*render;
	t_widget		*about;
	t_widget		*info;
	t_modal_export	export;
	t_modal_export	import;
}				t_windows_widgets;

typedef struct	s_draw_text
{
	int					font_size;
	t_prefab_input		font_size_input;
	t_prefab_input		input;
	t_prefab_font_list	font_list;
	char				*font;
	int					active_font;
}				t_draw_text;

typedef struct	s_data
{
	t_core				*core;
	int					screen_width;
	int					screen_height;

	t_drawing			draw_infos;

	t_widget			*tool_box;
	t_prefab_box		tool_settings_container;
	t_widget			*settings_boxs[NUM_TOOL];

	t_prefab_box		tool_button_container;
	t_widget			*tool_buttons[NUM_TOOL];
	
	t_color_widgets		wcolor;
	t_header_widgets	wheader;

	t_canvas_widgets	wcanvas[MAX_CANVAS];
	t_widget			*canvas_buttons_box;
	t_widget			*canvas_buttons[MAX_CANVAS];
	int					active_canvas;
	int					nb_canvas;

	t_stickers_widgets	wstickers;
	t_windows_widgets	wwindows;
	
	t_parameter			param[NUM_TOOL];
	float				brightness;
	t_rgba				color;
	int					active_sticker;
	int					nb_stickers;
	enum e_tool			active_tool;
	bool				ctrl_pressed;
	bool				alt_pressed;
	bool				shift_pressed;
	int					selected_font;
	t_modal_new			modal_new;
	t_draw_text			draw_text;
	bool				header_opened;
	t_prefab_font_list	edit_font_list;
}				t_data;


# define STYLE_WINDOW_BG		(1 << 0)
# define STYLE_BUTTON_NORMAL	(1 << 1)
# define STYLE_BUTTON_HOVER		(1 << 2)
# define STYLE_BUTTON_CLICK		(1 << 3)
# define STYLE_CANVAS_BG		(1 << 4)
# define STYLE_SLIDER_FILL		(1 << 5)
# define STYLE_COLOR_R			(1 << 6)
# define STYLE_COLOR_G			(1 << 7)
# define STYLE_COLOR_B			(1 << 8)
# define STYLE_COLOR_A			(1 << 9)
# define STYLE_BRIGHTNESS		(1 << 10)
# define STYLE_BUTTON_OUTLINE	(1 << 11)
# define STYLE_BOX_OUTLINE		(1 << 12)
# define STYLE_SLIDER_OUTLINE	(1 << 13)
# define STYLE_TEXT_COLOR		(1 << 14)

typedef struct	s_style
{
	unsigned int	window_bg;
	unsigned int	button_normal;
	unsigned int	button_hover;
	unsigned int	button_click;
	unsigned int	canvas_bg;
	unsigned int	slider_fill;
	t_rgba			draw_color;
	float			brightness;
	int				button_outline;
	int				box_outline;
	int				slider_outline;
	unsigned int	text_color;
	int				flags;
}				t_style;

t_rgba			unpack_color(unsigned int color);
unsigned int	pack_color(t_rgba color, float brightness);



void			draw_on_layer(t_data *data, t_widget *canvas, int x, int y);



int				distance(int x1, int y1, int x2, int y2);
int				lerp(int a, int b, int step, int max);


void			on_widget_clicked(t_widget *widget, int button, int x, int y, void *param);
void			set_active_tool(t_data *data, enum e_tool tool);
void			get_color_from_wheel(t_data *data, int x, int y);
void			add_sticker(t_data *data, const char *path);
void			toggle_header_menus(t_data *data, int menu);

void			base_init(t_data *data);
void			style_init(t_data *data, t_style *style);
int				parse_css_file(const char *path, t_style *style);
void			apply_css_style(t_data *data, t_style *style);

void			add_layer(t_data *data, const char *path, int index);
void			add_empty_layer(t_data *data);
void			set_active_layer(t_canvas_widgets *wcanvas, int index);
void			set_active_sticker(t_data *data, int index);
bool			is_canvas(t_widget *widget, t_data *data);
void			new_canvas(t_data *data, int resolution_x, int resolution_y);

void			init_font_picker(t_data *data);
void			handle_font_button_click(t_widget *widget, t_data *data);
void			build_canvas_infos(t_canvas_widgets *wcanvas);
bool			is_header(t_data *data, t_widget *widget);
void			close_header(t_data *data);
void			add_copy_layer(t_data *data, int x, int y);
void			set_active_canvas(t_data *data, int index);
void			remove_layer(t_canvas_widgets *wcanvas, int index);
void			open_modal_new(t_data *data);
void			open_about_modal(t_data *data);
#endif
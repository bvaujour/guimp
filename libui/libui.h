/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libui.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xacharle <xacharle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:53:57 by bvaujour          #+#    #+#             */
/*   Updated: 2026/05/04 17:09:25 by xacharle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LIBUI_H
# define LIBUI_H

# include <stdio.h>
# include <stdbool.h>
# include <math.h>
# include "libui_scancodes.h"

typedef struct	s_core		t_core;
typedef struct	s_widget	t_widget;

# define UI_MAX_HEADER	10

typedef struct	s_rect
{
	int				x;
	int				y;
	int				width;
	int				height;
}				t_rect;

typedef struct	s_margin
{
	int				top;
	int				right;
	int				bottom;
	int				left;
}				t_margin;

typedef enum	text_mode
{
	DEFAULT_TEXT,
	CENTERED,
	SINGLE_LINE,
	FIT,
}			e_text_mode;

typedef enum	scroll_mode
{
	DEFAULT_SCROLL,
	FREE_SCROLL,
	NO_SCROLL,
}			e_scroll_mode;

typedef enum	window_type
{
	DEFAULT_WINDOW,
	MODAL_ACCEPT_WINDOW,
	MODAL_INFO_WINDOW,
}			e_window_type;

typedef enum	box_mode
{
	DEFAULT,
	STACK_CHILDS,
	WRAP_CHILDS,
	EVEN_CHILDS_VERTICAL,
	EVEN_CHILDS_HORIZONTAL,
	LIST_CHILDS_VERTICAL,
	LIST_CHILDS_HORIZONTAL,
	CENTER_CHILDS,
}			e_box_mode;

typedef enum	e_fonts
{
	ROBOTO,
	TOMORROW,
	AMARANTE,
	FROZENSOLID,
	BOKYTIME,
	BITCOUTINK,
	NUM_FONT
}			e_font;

typedef struct	s_prefab_text
{
	t_widget	*box;
	t_widget	*text;
}				t_prefab_text;

typedef struct	s_prefab_button
{
	t_widget	*button;
	t_widget	*text;
}				t_prefab_button;

typedef struct	s_prefab_slider
{
	t_widget		*box;
	t_prefab_text	name_text;
	t_widget		*slider;
	t_prefab_text	value_text;
}				t_prefab_slider;


typedef struct	s_prefab_input
{
	t_widget		*box;
	t_widget		*input;
}				t_prefab_input;

typedef struct	s_prefab_header
{
	t_widget		*container;
	t_prefab_button	buttons[UI_MAX_HEADER];
	t_widget		*box[UI_MAX_HEADER];
}				t_prefab_header;

typedef struct	s_prefab_box
{
	t_widget		*box;
	t_prefab_text	title;
	t_widget		*inner_box;
	t_widget		*button1;
	t_widget		*button2;
	int				initial_height;
	bool			is_minimized;
}				t_prefab_box;

typedef struct	s_prefab_font_list
{
	t_widget		*box;
	t_prefab_button	buttons[NUM_FONT];
	char			paths[NUM_FONT][256];
	char			names[NUM_FONT][256];
}				t_prefab_font_list;

//CORE

t_core			*ui_init();
void			ui_run(t_core *core);
void			ui_quit(t_core *core);
void			ui_set_font(t_core *core, const char *path, int size);

//BUTTON
t_widget		*ui_create_button(t_widget *parent, t_rect	relative_rect);
t_widget		*ui_create_onoff_button(t_widget *parent, t_rect relative_rect);

//WIDGET

void			ui_set_widget_position(t_widget *widget, int x, int y);
void			ui_set_widget_size(t_widget *widget, int width, int height);
void			ui_set_widget_position_and_size(t_widget *widget, int x, int y, int width, int height);
int				ui_set_widget_texture(t_widget *widget, const char *path);


void			ui_set_widget_colors(t_widget *widget, unsigned int normal, unsigned int hovered, unsigned int clicked);
void			ui_set_widget_color(t_widget *widget, unsigned int color);
void			ui_set_widget_normal_color(t_widget *widget, unsigned int color);
void			ui_set_widget_hovered_color(t_widget *widget, unsigned int color);
void			ui_set_widget_clicked_color(t_widget *widget, unsigned int color);


void			ui_set_widget_visibility(t_widget *widget, int new_visibility);
int				ui_get_widget_visibility(t_widget *widget);
void			ui_toggle_widget_visibility(t_widget *widget);
void			ui_get_screen_size(int *screen_width, int *screen_height);
void			ui_set_widget_cursor_from_image(t_widget *widget, const char *path, int width, int height, unsigned int color_mod);

void			ui_add_widget_size(t_widget *widget, int delta_w, int delta_h);
void			ui_set_widget_dragable(t_widget *widget, bool new_draggable);
void			ui_set_widget_drag_zone(t_widget *widget, bool full_width, bool full_height, t_rect drag_zone);

void			ui_set_widget_resizable(t_widget *widget, bool new_resizable);
bool			ui_get_widget_resizable(t_widget *widget);
void			ui_set_widget_cursor_as_arrow(t_widget *widget);


//WINDOW

t_widget 		*ui_create_window(t_core *core, t_rect screen_rect, e_window_type type, const char *name);

//BOX

t_widget	*ui_create_box(t_widget *parent, t_rect	relative_rect, e_box_mode mode, t_margin margin);
//SLIDER

t_widget		*ui_create_slider(t_widget *parent, t_rect relative_rect);



t_widget		*ui_create_layer(t_widget *parent);
void 			ui_layer_snapshot(t_widget *layer);
void 			ui_layer_restore_previous_snapshot(t_widget *layer);
void 			ui_layer_restore_next_snapshot(t_widget *layer);

void			ui_bind_onkeypress(t_core *core, void (*f)(int key, void *param), void *param);
void			ui_bind_onkeyrelease(t_core *core, void (*f)(int key, void *param), void *param);
void			ui_bind_onwidgetclicked(t_core *core, void (*f)(t_widget *widget, int mouse_button, int x, int y, void *param), void *param);
void			ui_bind_onwidgetreleased(t_core *core, void (*f)(t_widget *widget, int mouse_button, int x, int y, void *param), void *param);
void			ui_bind_onwidgethovered(t_core *core, void (*f)(t_widget *widget, int x, int y, void *param), void *param);
void			ui_bind_onwidgetunhovered(t_core *core, void (*f)(t_widget *widget, void *param), void *param);
void			ui_bind_onsliderslided(t_core *core, void (*f)(t_widget *slider, float value, void *param), void *param);
void			ui_bind_oninputvalidated(t_core *core, void (*f)(t_widget *input, const char *str, void *param), void *param);
void			ui_bind_onwindowclosed(t_core *core, void (*f)(t_widget *window, bool accepted, void *param), void *param);
void			ui_bind_onwidgetwheeled(t_core *core, void (*f)(t_widget *widget, int value, void *param), void *param);
void			ui_bind_oninputchanged(t_core *core, void (*f)(t_widget *input, char *str, void *param), void *param);
t_widget		*ui_create_text(t_widget *parent, int x, int y, const char *str, e_text_mode mode, int font_size);



void			ui_draw_rect(t_widget *layer, t_rect rect, int thickness, unsigned int color);
void			ui_draw_point(t_widget *layer, int x, int y, unsigned int color);
void			ui_draw_line(t_widget *layer, int x1, int y1, int x2, int y2, unsigned int color);
void			ui_draw_line_thick(t_widget *layer, int x1, int y1, int x2, int y2, int thickness, unsigned int color);

void			ui_draw_filled_rect(t_widget *layer, t_rect rect, unsigned int color);
void			ui_draw_circle(t_widget *layer, int centreX, int centreY, int radius, unsigned int color);
void			ui_draw_filled_circle(t_widget *layer, int centreX, int centreY, int radius, unsigned int color);
void 			ui_draw_sticker(t_widget *layer, t_widget *sticker, t_rect dest_rect, unsigned int color_mod);
void			ui_bucket_layer(t_widget *layer, int start_x, int start_y, unsigned color);
void			ui_clear_layer(t_widget *layer, unsigned int color);
void 			ui_erase_layer(t_widget *layer, int x, int y, int width, int height, unsigned char force);

t_widget		*ui_create_input(t_widget *parent, t_rect relative_rect, int font_size);

char			*ui_input_get_text(t_widget *input);
void			ui_set_text_str(t_widget *text, char *str);
void			ui_set_text_font_color(t_widget *text, unsigned int font_color);


void			ui_add_widget_position(t_widget *widget, int delta_x, int delta_y);
void			ui_set_box_mode(t_widget *box, e_box_mode mode);

t_widget		*ui_create_image(t_widget *parent, t_rect relative_rect, const char *image_path);
unsigned int	ui_image_get_pixel(t_widget *image, int x, int y);
void			ui_set_slider_value(t_widget *slider, float value);
void			ui_slider_set_fill_color(t_widget *slider, unsigned int color);
void			ui_set_text_str_by_int(t_widget *text, int nbr);
void			ui_layer_open_image(t_widget *layer, const char *path);
void			ui_bind_onfiledropped(t_core *core, void (*f)(t_widget *widget, const char *path, void *param), void *param);
void			ui_set_widget_outline(t_widget *widget, int outline_thickness);
const char		*ui_image_get_image_path(t_widget *image);
t_prefab_slider	ui_create_prefab_slider(t_widget *parent, t_rect relative_rect, char *name, float default_value, float max_value);
	
t_prefab_button	ui_create_prefab_button(t_widget *parent, t_rect relative_rect, const char *str, e_text_mode text_mode);
t_prefab_button	ui_create_prefab_onoff_button(t_widget *parent, t_rect relative_rect, const char *str, e_text_mode text_mode);
t_prefab_text	ui_create_prefab_text(t_widget *parent, t_rect relative_rect, const char *str, unsigned int font_color);
t_prefab_box	ui_create_prefab_box(t_widget *parent, t_rect relative_rect, e_box_mode mode, const char *title, t_rect title_rect);
void			ui_set_text_str_by_float(t_widget *text, float nbr, int precision);
void			ui_get_widget_position(t_widget *widget, int *x, int *y);
void			ui_get_widget_size(t_widget *widget, int *width, int *height);
void			ui_set_widget_cursor_as_rect(t_widget *widget, int width, int height, int thickness, unsigned int color_mod, int hotspot_x, int hotspot_y);
void 			ui_layer_export(t_widget *layer, const char *path);
void			ui_close_window(t_core *core, t_widget *window);
t_widget		*ui_create_layer_from_image(t_widget *parent, const char *path);
void			ui_set_widget_hit_testable(t_widget *widget, bool new_hit_testable);
void			ui_canvas_export_layers_as_jpg(t_widget *canvas, const char *path, int quality);
void			ui_canvas_export_layers_as_png(t_widget *canvas, const char *path);
t_widget		*ui_create_canvas(t_widget *parent, int resolution_w, int resolution_h);
void			ui_widget_clone_texture(t_widget *widget, t_widget *widget_to_clone);

void			ui_widget_downgrade(t_widget *widget);
void			ui_widget_upgrade(t_widget *widget);
void			ui_canvas_zoom(t_widget *canvas, int value);
void			ui_set_widget_scroll_mode(t_widget *widget, e_scroll_mode mode);
void 			ui_copy_layer(t_widget *layer, t_rect selection);

void 			ui_paste_on_layer(t_widget *layer);

void			ui_start_selection(t_widget *window, t_widget *container);
void			ui_start_selection_circle(t_widget *window, t_widget *container);
void			ui_start_selection_line(t_widget *window, t_widget *container);
void			ui_stop_selection(t_widget *window);
void			ui_stop_hide_selection(t_widget *window);
t_rect			ui_get_selection(t_widget *window);
void			ui_draw_text(t_widget *canvas, int x, int y, const char *text, unsigned int color);
float			ui_layer_get_zoom(t_widget *layer);
bool			ui_button_get_onoff_state(t_widget *button);
void			ui_draw_circle_moins_opti_mais_plus_beau(t_widget *layer, int center_x, int center_y, int radius, int thickness, unsigned int color);
void			ui_toggle_minimize_prefab_box(t_prefab_box *prefab);
void			ui_set_custom_cursor(t_widget *window, const char *path, int width, int height);
void			ui_set_custom_cursor_size(t_widget *window, int width, int height);
void			ui_set_use_custom_cursor(t_widget *window, bool newUse);
void			ui_set_custom_cursor_color(t_widget *window, unsigned int packed_color);
void			ui_image_set_texture(t_widget *image, const char *image_path);
float			ui_canvas_get_zoom(t_widget *canvas);
void 			ui_erase_layer_sticker(t_widget *layer, t_widget *sticker, t_rect dest_rect, unsigned char force);

t_widget		*ui_create_layer_from_copy_selection(t_widget *parent, int x, int y);
void			ui_clear_layer_from_selection(t_widget *layer);
t_widget *ui_create_prefab_modal_info_window(t_core *core, int width, int height, e_box_mode inner_box_mode, const char *info, int font_size);
void			ui_widget_remove_child(t_widget *parent, t_widget *child);
void			ui_set_use_override_cursor(t_core *core, bool new_use);

void			ui_set_override_cursor(t_core *core, const char *path, int hot_x, int hot_y);
unsigned int	ui_get_pixel(t_widget *widget, int x, int y);
t_prefab_header	ui_create_prefab_header(t_widget *parent, int pos_x, int pos_y, const char **headers, int num_header);
void			ui_input_set_text(t_widget *input, const char *str);
void			ui_layer_write(t_widget *layer, char *str, char *font_path, int font_size, unsigned int color);
t_prefab_input	ui_create_prefab_input(t_widget *parent, t_rect rect, const char *label);
t_prefab_font_list	ui_create_prefab_font_list(t_widget *parent, t_rect rect);
t_rect			ui_get_widget_absolute_rect(t_widget *widget);
#endif
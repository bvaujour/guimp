/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libui_int.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 04:12:23 by injah             #+#    #+#             */
/*   Updated: 2026/05/05 15:18:49 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LIBUI_INT_H
# define LIBUI_INT_H

# include "libui.h"
# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
# include <SDL2/SDL_image.h>
# include "libft.h"


# define	UI_MAX_TEXT_SIZE				512
# define	UI_MIN_RESIZE_W					20
# define	UI_MIN_RESIZE_H					10
# define	UI_RESIZE_ZONE					5
# define	UI_MAX_TEXT_SIZE				512
# define	UI_MAX_SNAPSHOTS				30

# define	UI_ERROR						-1
# define	UI_SUCCESS						0
# define	UI_SCROLL_SPEED					10

# define	UI_MAX_WINDOWS					5
# define	UI_MAX_WINDOW_CHILDS			1000
# define	UI_MAX_LAYER_CHILDS				0
# define	UI_MAX_CANVAS_CHILDS			20
# define	UI_MAX_IMAGE_CHILDS				5
# define	UI_MAX_INPUT_CHILDS				0
# define	UI_MAX_TEXT_CHILDS				0
# define	UI_MAX_BUTTON_CHILDS			1
# define	UI_MAX_BOX_CHILDS				1000
# define	UI_MAX_SLIDER_CHILDS			3

typedef		enum e_widget_type
{
	WINDOW,
	LAYER,
	CANVAS,
	BUTTON,
	ONOFF_BUTTON,
	INPUT,
	TEXT,
	IMAGE,
	DRAWABLE,
	SLIDER,
	BOX,
}				e_widget_type;

typedef struct	s_img
{
	unsigned int	*pixels;
	int				pitch;
	int				width;
	int				height;
}				t_img;

typedef enum	e_widget_state
{
	NORMAL,
	HOVERED,
	CLICKED,
	NUM_STATE
}				e_widget_state;

typedef struct	s_prefab_modal
{
	t_widget		*window;
	t_prefab_button	button_ok;
	t_prefab_button	button_cancel;
}				t_prefab_modal;

typedef struct	s_mouse_infos
{
	SDL_Point	position;
	SDL_Point	motion;
	bool		buttons[10];
	SDL_Cursor	*current_cursor;
	SDL_Cursor	*system_cursors[SDL_NUM_SYSTEM_CURSORS];
}				t_mouse_infos;

typedef struct	s_custom_cursor
{
	SDL_Texture	*texture;
	SDL_Rect	rect;
	SDL_Point	hot_spot;
	char		path[UI_MAX_TEXT_SIZE];
	SDL_Color	color;
}				t_custom_cursor;

typedef struct	s_widget
{
	char				name[128];
	struct s_core		*core;
	t_widget			*owning_window;
	SDL_Renderer		*renderer;
	struct s_widget		**childs;
	int					max_child;
	int					nb_child;
	struct s_widget		*parent;
	void				*data;
	void				*pixels;
	e_widget_type		type;
	SDL_Point			position;
	SDL_Rect			clip;
	int					width;
	int					height;
	SDL_Texture			*texture;
	bool				is_clone;
	bool				is_on;
	e_widget_state		state;
	SDL_Color			colors[NUM_STATE];
	bool				is_visible;
	bool				is_draggable;
	SDL_Rect			drag_zone;
	bool				drag_full_width;
	bool				drag_full_height;
	bool				is_resizable;
	SDL_Cursor			*cursor;
	bool				hit_testable;
	int					outline;
	SDL_Point			scroll;
	e_scroll_mode		scroll_mode;
	void				(*event)(struct s_widget *widget);
	void				(*update)(struct s_widget *widget);
	void				(*render)(struct s_widget *widget);
	void				(*destroy)(struct s_widget *widget);
	void				(*build)(struct s_widget *widget);
}				t_widget;

typedef struct	s_layer_data
{
	SDL_Surface		*surface;
	SDL_Texture		*snapshots[UI_MAX_SNAPSHOTS];
	int				snapshot_index;
	int				snapshots_length;
	float			zoom;
	int				texture_width;
	int				texture_height;
	unsigned int	*clipboard_pixels;
    int				clipboard_width;
    int				clipboard_height;
}				t_layer_data;

typedef struct	s_canvas_data
{
	float		zoom;
	int			texture_width;
	int			texture_height;
}				t_canvas_data;

typedef struct	s_window_data
{
	SDL_Window		*window;
	unsigned int	id;
	e_window_type	type;
	t_widget		*on_widget;
	SDL_Point		selection_start;
	SDL_Rect		selection;
	SDL_Surface 	*surface;
	bool			is_selectionning;
	bool			is_selectionning_circle;
	bool			is_selectionning_line;
	bool			show_selection;
	bool			show_selection_circle;
	bool			show_selection_line;
	bool			use_custom_cursor;
	t_custom_cursor	custom_cursor;
	bool			is_durty;
}				t_window_data;

typedef struct	s_input_data
{
	char		str[UI_MAX_TEXT_SIZE];
	int			str_index;
	SDL_Color	str_color;
	SDL_Texture	*str_texture;
	SDL_Rect	str_rect;
	int			font_size;
	int			cursor_pos;
}				t_input_data;

typedef struct	s_text_data
{
	e_text_mode		mode;
	char			str[UI_MAX_TEXT_SIZE];
	bool			str_is_dirty;
	int				font_size;
}				t_text_data;

typedef struct	s_box_data
{
	e_box_mode	mode;
	t_margin	margin;
}				t_box_data;

typedef struct	s_image_data
{
	SDL_Surface	*surface;
	char		image_path[UI_MAX_TEXT_SIZE];
}				t_image_data;

typedef struct	s_slider_data
{
	SDL_Color		fill_color;
	float			value;
	SDL_Rect		inner;
}				t_slider_data;

typedef struct	s_button_data
{
	int dummy;
}				t_button_data;

typedef struct	s_copy
{
	// SDL_Surface	*surface;
	SDL_Texture	*texture;
}				t_copy;

typedef struct	s_core
{
	t_widget				**windows;
	t_widget				*focused_window;
	t_prefab_modal			modal;
	TTF_Font				*font;
	char					*font_file;

	void					(*onkeypress)(int, void *);
	void					*onkeypress_param;

	void					(*onkeyrelease)(int, void *);
	void					*onkeyrelease_param;

	void					(*onwidgetclicked)(t_widget *widget, int mouse_button, int x, int y, void *param);
	void					*onwidgetclicked_param;

	void					(*onwidgetreleased)(t_widget *widget, int mouse_button, int x, int y, void *param);
	void					*onwidgetreleased_param;
	
	void					(*onwidgetwheeled)(t_widget *widget, int value, void *param);
	void					*onwidgetwheeled_param;

	void					(*onwidgethovered)(t_widget *widget, int x, int y, void *param);
	void					*onwidgethovered_param;

	void					(*onwidgetunhovered)(t_widget *widget, void *param);
	void					*onwidgetunhovered_param;

	void					(*onsilderslided)(t_widget *slider, float value, void *param);
	void					*onsilderslided_param;

	void					(*oninputvalidated)(t_widget *input, const char *str, void *param);
	void					*oninputvalidated_param;

	void					(*onfiledropped)(t_widget *widget, const char *path, void *param);
	void					*onfiledropped_param;

	void					(*onwindowclosed)(t_widget *window, bool accepted, void *param);
	void					*onwindowclosed_param;

	void					(*oninputchanged)(t_widget *input, char *str, void *param);
	void					*oninputchanged_param;

	t_mouse_infos			mouse;
	const Uint8 			*keys;
	SDL_Event				event;
	

	t_widget 				*focused_widget;
	t_widget 				*last_hovered_widget;
	t_widget 				*hovered_widget;
	t_widget 				*clicked_widget;

	t_widget 				*scrollable_widget;
	t_widget				*draggable_widget;
	t_widget				*resizable_widget;
	bool					can_resize_w;
	bool					can_resize_h;
	bool					can_resize_x;
	bool					can_resize_y;
	bool					can_drag;
	bool					is_running;
	t_copy					copy_info;
	bool					use_override_cursor;
	SDL_Cursor				*override_cursor;
}				t_core;

//ENGINE
void		ui_destroy(t_core *core);
void 		ui_destroy_widget(t_widget *widget);

void		ui_set_cursor(t_core *core, SDL_Cursor *cursor);

int			ui_core_add_window(t_core *core, t_widget *window);

void		ui_build_widget(t_widget *widget);

//WIDGET
int			ui_add_child(t_widget *widget, t_widget *child);
t_widget 	**ui_new_widget_tab(int tab_len);
int			ui_add_child(t_widget *parent, t_widget *child);


t_widget 	*ui_new_widget(t_widget *parent, SDL_Rect rect, e_widget_type type, int max_child);
void		ui_widget_outline(t_widget *widget, int thickness);

void		ui_widget_call_onclicked(t_core *core, t_widget *widget, int x, int y, int mouse_button);
void		ui_widget_change_state(t_widget *widget, e_widget_state new_state);
SDL_Rect	ui_get_clip_rect(t_widget *widget);
SDL_Point	ui_get_absolute_position(t_widget *widget);
//UTILS
TTF_Font	*ui_open_font_match_size(const char *font_path, const char *text, int width, int height);
TTF_Font	*ui_open_font_match_height(const char *font_path, const char *text, int height);
SDL_Color	ui_unpack_color(unsigned int color);



SDL_Texture	*ui_new_texture(SDL_Renderer *renderer, int width, int height, SDL_Color color_mod);

void		ui_manage_wheel(t_widget *widget);
SDL_Rect 	ui_calculate_inner(t_widget *widget);
bool		ui_widget_can_scroll(t_widget *widget);
t_img		ui_layer_get_img(t_widget *layer);
void		ui_layer_set_img(t_widget *layer, t_img img);
void		ui_widget_basic_render(t_widget *widget);
void		ui_widget_call_onreleased(t_core *core, t_widget *widget, int x, int y, int mouse_button);

void		ui_widget_call_onhovered(t_core *core, t_widget *widget);

void		ui_widget_call_onunhovered(t_core *core, t_widget *widget);
SDL_Texture	*ui_create_str_texture(SDL_Renderer *renderer, const char *str, TTF_Font *font, int font_size, int wrap_length);
void		ui_window_set_is_dirty(t_widget *window, bool new_dirty);

bool		ui_window_get_is_dirty(t_widget *window);
bool		ui_is_in_familly_of(t_widget *widget, t_widget *other);
void		ui_close_modal(t_core *core);
void		ui_window_selection_rect(t_widget *window);
void		ui_window_selection_circle(t_widget *window);
void		ui_window_selection_line(t_widget *window);
void		ui_window_show_selection(t_widget *window);
void		ui_call_close_window(t_core *core, bool accepted, t_widget *window);
#endif

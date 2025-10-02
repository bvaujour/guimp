/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libui.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:53:57 by bvaujour          #+#    #+#             */
/*   Updated: 2025/10/02 07:45:20 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LIBUI_H
# define LIBUI_H

# include <stdio.h>
# include <stdbool.h>
# include "SDL/include/SDL2/SDL.h"
# include "SDL_ttf/include/SDL2/SDL_ttf.h"
# include "SDL_image/include/SDL2/SDL_image.h"

# define	UI_BLUE							(SDL_Color){0, 150, 200, 255}
# define	UI_LIGHT_GREY					(SDL_Color){150, 150, 150, 255}
# define	UI_DARK_GREY					(SDL_Color){70, 70, 70, 255}
# define	UI_BLACK 						(SDL_Color){0, 0, 0, 255}
# define	UI_WHITE 						(SDL_Color){255, 255, 255, 255}

# define	UI_THEME1_BOX_COLOR				(SDL_Color){60, 60, 60, 255}
# define	UI_THEME1_WINDOW_COLOR			(SDL_Color){90, 90, 90, 255}
# define	UI_THEME1_BUTTON_DEFAULT_COLOR	(SDL_Color){150, 150, 150, 255}
# define	UI_THEME1_BUTTON_HOVERED_COLOR	(SDL_Color){130, 130, 130, 255}
# define	UI_THEME1_BUTTON_CLICKED_COLOR	(SDL_Color){100, 100, 100, 255}
# define	UI_THEME1_FONT					"libui/fonts/Roboto/Roboto_Condensed-Black.ttf"

# define	SCROLL_SPEED		5
# define	MAX_WIDGET			30
# define	MAX_BOX				20
# define	MAX_CONTEXT				10

typedef enum	e_widget_type
{
	BUTTON,
	SLIDER,
	BOX
}				t_widget_type;

typedef enum	e_button_state
{
	DEFAULT,
	HOVERED,
	CLICKED
}				t_button_state;

typedef enum	e_direction
{
	HORIZONTAL,
	VERTICAL,
}				e_direction;

typedef	struct	s_offset
{
	int	left;
	int	top;
	int	right;
	int	bottom;
}				t_offset;

typedef struct	s_config
{
	TTF_Font		*font;
	SDL_Color		font_color;
	SDL_Color		window_color;
	SDL_Color		box_color;
	SDL_Color		button_color[3];
}				t_config;



typedef struct	s_button_data
{
	SDL_Surface			*surfaces[3];
	t_button_state		state;
	void				(*on_click)(void *param);
	void				*param;
}				t_button_data;

struct s_core;
struct s_box;
struct s_context;
struct s_widget;

typedef struct	s_widget
{
	SDL_Surface			*surface;
	SDL_Rect			rect;
	SDL_Rect			relative;
	t_widget_type		type;
	t_button_data		button_data;
	void				(*update)(struct s_widget *widget);
	void				(*destroy)(struct s_widget *widget);
	bool				is_durty;
	struct s_box		*box;
	struct s_context	*context;
	struct s_core		*core;
}				t_widget;

typedef struct	s_box
{
	SDL_Texture			*texture;
	SDL_Rect			rect;
	t_widget			widgets[MAX_WIDGET];
	int					nb_widget;
	e_direction			flex_direction;
	int					flex;
	struct s_context	*context;
	struct s_core		*core;
	bool				is_durty;
	SDL_Point			scroll;
	SDL_Point			max_scroll;
	SDL_Rect			total_widget_rect;
}				t_box;

typedef struct	s_context
{
    SDL_Window		*window;
    SDL_Renderer	*renderer;
	SDL_Rect		rect;
	t_box			boxs[MAX_BOX];
	int				nb_box;
	e_direction		flex_direction;
	int				flex;
	bool			is_visible;
	bool			is_durty;
	struct s_core	*core;
}				t_context;

typedef struct	s_core
{
	t_context		contexts[MAX_CONTEXT];
	// bool			hotkeys[SDL_NUM_SCANCODES];
	int				nb_contexts;
	e_direction		flex_direction;
	SDL_Event		event;
	bool			is_running;
	int				screen_w;
	int				screen_h;
	t_config		config;
	SDL_Point		mouse;
	Uint32			focused_window_id;
	SDL_Point		wheel;
	bool			scrolled;
	bool			left_click;
	bool			lshift;
}				t_core;



void			ui_get_screen_size(int *width, int *height);
void 			ui_get_sdl_version();

SDL_Surface		*ui_copy_surface(SDL_Surface *surface);
void			ui_fill_surf(SDL_Surface *surface, SDL_Color color);
SDL_Surface		*ui_new_surf(int width, int height, SDL_Color *color);
void			ui_blit_centered(SDL_Surface *src, SDL_Surface *dst);
void			ui_blit(SDL_Surface *src, SDL_Surface *dst, int x, int y);


void			ui_destroy(t_core *core);
int				ui_init(t_core *core);

void			ui_run(t_core *core);
void			ui_update(t_core *core);

void			ui_render(t_core *core);

t_context		*ui_create_basic_window(t_core *core, const char *title, SDL_Rect rect);
t_context		*ui_create_rendering_window(t_core *core, const char *title, SDL_Rect rect);
t_box			*ui_create_box(t_context *context, e_direction direction, SDL_Rect rect);
t_widget		*ui_add_button(t_box *box, char *label, int font_size, t_offset padding);

void			ui_update_button(t_widget *widget);
void			ui_destroy_button(t_widget *button);
void			ui_bind_button(t_widget *button, void(*f)(), void *param);

void			ui_box_consume_scroll(t_box *box);

void			ui_build_box(t_box *box);



#endif
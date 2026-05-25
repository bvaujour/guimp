/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_widget_user.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 03:24:56 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

void	ui_set_widget_position(t_widget *widget, int x, int y)
{
	if (widget->position.x != x || widget->position.y != y)
	{
		widget->position.x = x;
		widget->position.y = y;
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

t_rect	ui_get_widget_absolute_rect(t_widget *widget)
{
	SDL_Point	absolute;

	if (widget == NULL)
		return ((t_rect){0, 0, 0, 0});
	absolute = ui_get_absolute_position(widget);
	return ((t_rect){absolute.x, absolute.y, widget->width, widget->height});
}


void	ui_get_widget_position(t_widget *widget, int *x, int *y)
{
	if (!widget)
		return ;
	if (x)
		*x = widget->position.x;
	if (y)
		*y = widget->position.y;
}

void	ui_set_widget_hit_testable(t_widget *widget, bool new_hit_testable)
{
	widget->hit_testable = new_hit_testable;
}

void	ui_set_widget_size(t_widget *widget, int width, int height)
{
	if (widget->width != width || widget->height != height)
	{
		widget->width = width;
		widget->height = height;
		if (widget->parent && widget->parent->build)
			ui_build_widget(widget->parent);
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

void	ui_get_widget_size(t_widget *widget, int *width, int *height)
{
	if (!widget)
		return ;
	if (width)
		*width = widget->width;
	if (height)
		*height = widget->height;
}

void	ui_add_widget_position(t_widget *widget, int delta_x, int delta_y)
{
	if (delta_x || delta_y)
	{
		widget->position.x += delta_x;
		widget->position.y += delta_y;
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

void	ui_add_widget_size(t_widget *widget, int delta_w, int delta_h)
{
	if (delta_w || delta_h)
	{
		widget->width += delta_w;
		widget->height += delta_h;
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

void	ui_set_widget_position_and_size(t_widget *widget, int x, int y, int width, int height)
{
	ui_set_widget_position(widget, x, y);
	ui_set_widget_size(widget, width, height);
}

void	ui_set_widget_dragable(t_widget *widget, bool new_draggable)
{
	widget->is_draggable = new_draggable;
}

void	ui_set_widget_drag_zone(t_widget *widget, bool full_width, bool full_height, t_rect drag_zone)
{
	widget->drag_zone.x = drag_zone.x;
	widget->drag_zone.y = drag_zone.y;
	widget->drag_zone.w = drag_zone.width;
	widget->drag_zone.h = drag_zone.height;
	widget->drag_full_width = full_width;
	widget->drag_full_height = full_height;
}

void	ui_set_widget_resizable(t_widget *widget, bool new_resizable)
{
	widget->is_resizable = new_resizable;
}

bool	ui_get_widget_resizable(t_widget *widget)
{
	return (widget->is_resizable);
}

void	ui_set_widget_scroll_mode(t_widget *widget, e_scroll_mode mode)
{
	widget->scroll_mode = mode;
}

void	ui_set_widget_colors(t_widget *widget, unsigned int normal, unsigned int hovered, unsigned int clicked)
{
	if (widget == NULL)
	{
		ft_printf("Error: ui_set_widget_colors used on null widget\n");
		return ;
	}
	widget->colors[NORMAL] = ui_unpack_color(normal);
	widget->colors[HOVERED] = ui_unpack_color(hovered);
	widget->colors[CLICKED] = ui_unpack_color(clicked);
	SDL_SetTextureColorMod(widget->texture, widget->colors[widget->state].r, widget->colors[widget->state].g, widget->colors[widget->state].b);
	SDL_SetTextureAlphaMod(widget->texture, widget->colors[widget->state].a);
	ui_window_set_is_dirty(widget->owning_window, true);
}

void	ui_set_widget_color(t_widget *widget, unsigned int color)
{
	SDL_Color	last_color;
	SDL_Color	new_color;
	if (widget == NULL)
	{
		ft_printf("Error: ui_set_widget_colors used on null widget\n");
		return ;
	}
	new_color = ui_unpack_color(color);
	if (SDL_GetTextureColorMod(widget->texture, &last_color.r, &last_color.g, &last_color.b) == 0 && (last_color.r != new_color.r || last_color.g != new_color.g || last_color.b != new_color.b))
	{
		SDL_SetTextureColorMod(widget->texture, new_color.r, new_color.g, new_color.b);
		ui_window_set_is_dirty(widget->owning_window, true);
	}
	if (SDL_GetTextureAlphaMod(widget->texture, &last_color.a) == 0 && (last_color.a != new_color.a))
	{
		SDL_SetTextureAlphaMod(widget->texture, new_color.a);
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

void	ui_set_widget_normal_color(t_widget *widget, unsigned int color)
{
	if (widget == NULL)
		return ;
	widget->colors[NORMAL] = ui_unpack_color(color);
	if (widget->state == NORMAL)
	{
		SDL_SetTextureColorMod(widget->texture, widget->colors[NORMAL].r, widget->colors[NORMAL].g, widget->colors[NORMAL].b);
		SDL_SetTextureAlphaMod(widget->texture, widget->colors[NORMAL].a);
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

void	ui_set_widget_hovered_color(t_widget *widget, unsigned int color)
{
	if (widget == NULL)
		return ;
	widget->colors[HOVERED] = ui_unpack_color(color);
	if (widget->state == HOVERED)
	{
		SDL_SetTextureColorMod(widget->texture, widget->colors[HOVERED].r, widget->colors[HOVERED].g, widget->colors[HOVERED].b);
		SDL_SetTextureAlphaMod(widget->texture, widget->colors[HOVERED].a);
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

void	ui_set_widget_clicked_color(t_widget *widget, unsigned int color)
{
	if (widget == NULL)
		return ;
	widget->colors[CLICKED] = ui_unpack_color(color);
	if (widget->state == CLICKED)
	{
		SDL_SetTextureColorMod(widget->texture, widget->colors[CLICKED].r, widget->colors[CLICKED].g, widget->colors[CLICKED].b);
		SDL_SetTextureAlphaMod(widget->texture, widget->colors[CLICKED].a);
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

int	ui_set_widget_texture(t_widget *widget, const char *path)
{
	SDL_Surface	*surface;
	SDL_Color	color;

	if (!widget)
		return (-1);
	surface = IMG_Load(path);
	if (!surface)
	{
		ft_printf("ui_set_widget_texture: failed open image.\n");
		return (-1);
	}
	if (widget->texture && widget->is_clone == false)
		SDL_DestroyTexture(widget->texture);
	widget->texture = SDL_CreateTextureFromSurface(widget->renderer, surface);
	widget->is_clone = false;
	color = widget->colors[widget->state];
	SDL_SetTextureColorMod(widget->texture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(widget->texture, color.a);
	SDL_FreeSurface(surface);
	return (0);
}

void	ui_set_widget_visibility(t_widget *widget, int new_visibility)
{
	if (new_visibility != widget->is_visible)
	{
		widget->is_visible = new_visibility;
		ui_window_set_is_dirty(widget->owning_window, true);
	}
}

int	ui_get_widget_visibility(t_widget *widget)
{
	return (widget->is_visible);
}

void	ui_toggle_widget_visibility(t_widget *widget)
{
	widget->is_visible = !widget->is_visible;
	ui_window_set_is_dirty(widget->owning_window, true);
}

void	ui_get_screen_size(int *screen_width, int *screen_height)
{
	SDL_Rect		rect;

	SDL_GetDisplayUsableBounds(0, &rect);
	if (screen_width)
		*screen_width = rect.w;
	if (screen_height)
		*screen_height = rect.h;
}

void	ui_set_widget_cursor_from_image(t_widget *widget, const char *path, int width, int height, unsigned int color_mod)
{
	SDL_Surface	*surface;
	SDL_Surface	*scaled;
	SDL_Color	color;

	surface = IMG_Load(path);
	if (surface)
	{
		if (widget->cursor)
			SDL_FreeCursor(widget->cursor);
		scaled = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);
		color = ui_unpack_color(color_mod);
		SDL_SetSurfaceColorMod(surface, color.r, color.g, color.b);
		SDL_SetSurfaceAlphaMod(surface, color.a);
		SDL_BlitScaled(surface, NULL, scaled, NULL);
		widget->cursor =  SDL_CreateColorCursor(scaled, width / 2, height / 2);
		SDL_FreeSurface(surface);
		SDL_FreeSurface(scaled);
	}
}

void	ui_set_widget_cursor_as_rect(t_widget *widget, int width, int height, int thickness, unsigned int color_mod, int hotspot_x, int hotspot_y)
{
	SDL_Surface	*surface;

    if (widget->cursor)
        SDL_FreeCursor(widget->cursor);
    surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
    SDL_FillRect(surface, NULL, color_mod);
    SDL_FillRect(surface, &(SDL_Rect){thickness, thickness, width - 2 * thickness, height - 2 * thickness}, 0x00000000);
	widget->cursor = SDL_CreateColorCursor(surface, hotspot_x, hotspot_y);
    SDL_FreeSurface(surface);
}

void	ui_set_widget_cursor_as_arrow(t_widget *widget)
{
	if (widget->cursor)
		SDL_FreeCursor(widget->cursor);
	widget->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW); 
}



void	ui_set_widget_outline(t_widget *widget, int outline_thickness)
{
	widget->outline = outline_thickness;
	ui_window_set_is_dirty(widget->owning_window, true);
}

void	ui_widget_downgrade(t_widget *widget)
{
	int			i;
	int			pos;
	t_widget	*tmp;

	if (widget->parent == NULL)
		return ;
	i = 0;
	pos = 0;
	while (widget->parent->childs[i])
	{
		if (widget == widget->parent->childs[i])
		{
			pos = i;
			break ;
		}
		i++;
	}
	if (pos == 0)
		return ;
	tmp = widget->parent->childs[pos - 1];
	widget->parent->childs[pos - 1] = widget;
	widget->parent->childs[pos] = tmp;
	if (widget->parent->build)
		widget->parent->build(widget->parent);
	ui_window_set_is_dirty(widget->owning_window, true);
}

void	ui_widget_upgrade(t_widget *widget)
{
	int			i;
	int			pos;
	t_widget	*tmp;

	if (widget->parent == NULL)
		return ;
	i = 0;
	pos = 0;
	while (widget->parent->childs[i])
	{
		if (widget == widget->parent->childs[i])
		{
			pos = i;
			break ;
		}
		i++;
	}
	if (pos == widget->parent->nb_child - 1)
		return ;
	tmp = widget->parent->childs[pos + 1];
	widget->parent->childs[pos + 1] = widget;
	widget->parent->childs[pos] = tmp;
	if (widget->parent->build)
		widget->parent->build(widget->parent);
	ui_window_set_is_dirty(widget->owning_window, true);
}

void	ui_widget_clone_texture(t_widget *widget, t_widget *widget_to_clone)
{
	if (widget->texture && widget->is_clone == false)
		SDL_DestroyTexture(widget->texture);
	widget->texture = widget_to_clone->texture;
	widget->is_clone = true;
}

void	ui_widget_set_selection_begin(t_widget *window)
{
	t_window_data	*data;
	if (window->type != WINDOW)
		return ;
	data = window->data;
	data->selection_start = window->core->mouse.position;
}

void	ui_widget_remove_child(t_widget *parent, t_widget *child)
{
	int	i;

	if (parent->nb_child == 0)
		return ;
	i = 0;
	while (i < parent->nb_child)
	{
		if (parent->childs[i] == child)
			break ;
		i++;
	}
	if (i == parent->nb_child)
		return;
	while (i < parent->nb_child - 1)
	{
		parent->childs[i] = parent->childs[i + 1];
		i++;
	}
	parent->childs[i] = NULL;
	parent->nb_child--;
	ui_destroy_widget(child);
	ui_build_widget(parent);
	ui_window_set_is_dirty(parent->owning_window, true);
}

unsigned int	ui_get_pixel(t_widget *widget, int x, int y)
{
	ft_printf("get pixel on layer x: %d y: %d of %s\n", x, y, widget->name);
	if (widget->type == LAYER)
	{
		t_img			img;

		img = ui_layer_get_img(widget);
		ft_printf("get pixel on layer x: %d y: %d\n", x, y);
		return (img.pixels[y * img.width + x]);
	}
	else if (widget->type == IMAGE)
	{
		t_image_data 	*data;
		unsigned int	*pixels;
		data = (t_image_data *)widget->data;
		SDL_LockSurface(data->surface);
		x = x * ((float)data->surface->w / widget->width);
		y = y * ((float)data->surface->h / widget->height);
		if (x < 0 || y < 0 || x >= data->surface->w || y >= data->surface->h)
			return 0;
		pixels = data->surface->pixels;
		SDL_UnlockSurface(data->surface);
		return (pixels[y * data->surface->w + x]);
	}
	return (0);
}

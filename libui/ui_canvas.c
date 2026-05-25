/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_canvas.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:48:33 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:28:18 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

void	ui_canvas_zoom(t_widget *canvas, int value)
{
	SDL_Point		absolute;
	SDL_Point		relative_mouse_position;
	SDL_Point		new_mouse_position;
	SDL_Point		delta_mouse_position;
	SDL_FPoint		rel;
	int				previous_width;
	int				previous_height;
	t_canvas_data	*cdata;
	t_layer_data	*ldata;
	int				i;
	
	cdata = (t_canvas_data *)canvas->data;
	absolute = ui_get_absolute_position(canvas);
	relative_mouse_position.x = (canvas->core->mouse.position.x - absolute.x) / cdata->zoom;
	relative_mouse_position.y = (canvas->core->mouse.position.y - absolute.y) / cdata->zoom;
	cdata->zoom += 0.1f * value;
	if (cdata->zoom < 0.1f)
		cdata->zoom = 0.1f;
	previous_width = canvas->width;
	previous_height = canvas->height;
	ui_set_widget_size(canvas, cdata->texture_width * cdata->zoom, cdata->texture_height * cdata->zoom);
	new_mouse_position.x = relative_mouse_position.x * cdata->zoom;
	new_mouse_position.y = relative_mouse_position.y * cdata->zoom;
	delta_mouse_position.x = canvas->core->mouse.position.x - (absolute.x + new_mouse_position.x);
	delta_mouse_position.y = canvas->core->mouse.position.y - (absolute.y + new_mouse_position.y);
	ui_add_widget_position(canvas, delta_mouse_position.x, delta_mouse_position.y);
	i = 0;
	while (canvas->childs[i])
	{
		ldata = canvas->childs[i]->data;
		ldata->zoom = cdata->zoom;
		rel.x = (float)canvas->childs[i]->position.x / previous_width;
		rel.y = (float)canvas->childs[i]->position.y / previous_height;
		ui_set_widget_position(canvas->childs[i], roundf(rel.x * canvas->width), roundf(rel.y * canvas->height));
		ui_set_widget_size(canvas->childs[i], ldata->texture_width * ldata->zoom, ldata->texture_height * ldata->zoom);
		i++;
	}
}


void		ui_canvas_render(t_widget *canvas)
{
	SDL_Point	absolute;
	int			x;
	int			y;
	int			width;
	int			height;

	absolute = ui_get_absolute_position(canvas);
	SDL_QueryTexture(canvas->texture, NULL, NULL, &width, &height);
	y = 0;
	while (y < canvas->height)
	{
		x = 0;
		while (x < canvas->width)
		{
			SDL_RenderCopy(canvas->renderer, canvas->texture, NULL, &(SDL_Rect){absolute.x + x, absolute.y + y, width, height});
			x += width;
		}
		y += height;
	}
	int	i;

	i = 0;
	SDL_RenderSetClipRect(canvas->renderer, &canvas->parent->clip);
	while (canvas->childs[i])
	{
		ui_widget_outline(canvas->childs[i], canvas->childs[i]->outline);
		i++;
	}
}

void	ui_canvas_event(t_widget *canvas)
{
	SDL_Point	absolute;

	if (canvas->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		absolute = ui_get_absolute_position(canvas);
		ui_widget_call_onclicked(canvas->core, canvas, canvas->core->mouse.position.x - absolute.x, canvas->core->mouse.position.y - absolute.y, canvas->core->event.button.button);
	}
	else if (canvas->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(canvas);
		ui_widget_call_onreleased(canvas->core, canvas, canvas->core->mouse.position.x - absolute.x, canvas->core->mouse.position.y - absolute.y, canvas->core->event.button.button);
	}
}

t_widget	*ui_create_canvas(t_widget *parent, int resolution_w, int resolution_h)
{
	t_widget	*canvas;
	t_canvas_data	*data;

	if (parent == NULL)
		return (NULL);
	canvas = ui_new_widget(parent, (SDL_Rect){0, 0, resolution_w, resolution_h}, CANVAS, UI_MAX_CANVAS_CHILDS);
	if (!canvas)
		return (ui_destroy_widget(canvas), NULL);
	canvas->data = malloc(sizeof(t_canvas_data));
	if (!canvas->data)
		return (ui_destroy_widget(canvas), NULL);
	data = (t_canvas_data *)canvas->data;
	*data = (t_canvas_data){0};
	canvas->outline = 1;
	canvas->scroll_mode = NO_SCROLL;
	data->zoom = 1.0f;
	data->texture_width = resolution_w;
	data->texture_height = resolution_h;
	while (canvas->width > parent->width || canvas->height > parent->height)
	{
		ui_canvas_zoom(canvas, -1);
	}
	canvas->position.x = parent->width / 2 - canvas->width / 2;
	canvas->position.y = parent->height / 2 - canvas->height / 2;
	ft_strcpy(canvas->name, "canvas");
	canvas->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	canvas->render = ui_canvas_render;
	canvas->event = ui_canvas_event;
	canvas->texture = IMG_LoadTexture(canvas->renderer, "libui/assets/backgrounds/TBG.jpg");
	ui_set_widget_colors(canvas, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	if (ui_add_child(parent, canvas) != UI_SUCCESS)
		return (ui_destroy_widget(canvas), NULL);
	return (canvas);
}


float	ui_canvas_get_zoom(t_widget *canvas)
{
	t_canvas_data	*data;
	if (canvas->type != CANVAS)
		return (0.0f);
	data = canvas->data;
	return (data->zoom);
}
void	ui_canvas_export_layers_as_jpg(t_widget *canvas, const char *path, int quality)
{
	int	i;
	SDL_Surface		*surface;
	SDL_Texture		*texture;
	t_canvas_data	*cdata;
	t_layer_data	*ldata;
	if (canvas->nb_child == 0)
		return ;
	cdata = (t_canvas_data *)canvas->data;
	texture = SDL_CreateTexture(canvas->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, cdata->texture_width, cdata->texture_height);
	SDL_SetRenderTarget(canvas->renderer, texture);
	SDL_SetRenderDrawColor(canvas->renderer, 0, 0, 0, 0);
	SDL_RenderClear(canvas->renderer);
	SDL_SetRenderTarget(canvas->renderer, texture);
	i = 0;
	while (canvas->childs[i])
	{
		if (canvas->childs[i]->type == LAYER)
		{
			ldata = (t_layer_data *)canvas->childs[i]->data;
			SDL_RenderCopy(canvas->childs[i]->renderer, canvas->childs[i]->texture, NULL, &(SDL_Rect){canvas->childs[i]->position.x / cdata->zoom, canvas->childs[i]->position.y / cdata->zoom, ldata->texture_width, ldata->texture_height});
		}
		i++;
	}
	surface = SDL_CreateRGBSurfaceWithFormat(0, cdata->texture_width, cdata->texture_height, 32, SDL_PIXELFORMAT_ARGB8888);
	SDL_RenderReadPixels(canvas->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
	IMG_SaveJPG(surface, path, quality);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_SetRenderTarget(canvas->renderer, NULL);
}

void	ui_canvas_export_layers_as_png(t_widget *canvas, const char *path)
{
	int	i;
	SDL_Surface		*surface;
	SDL_Texture		*texture;
	if (canvas->nb_child == 0)
		return ;
	texture = SDL_CreateTexture(canvas->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, canvas->width, canvas->height);
	SDL_SetRenderTarget(canvas->renderer, texture);
	SDL_SetRenderDrawColor(canvas->renderer, 0, 0, 0, 0);
	SDL_RenderClear(canvas->renderer);
	i = 0;
	while (canvas->childs[i])
	{
		if (canvas->childs[i]->type == LAYER)
		{
			SDL_RenderCopy(canvas->childs[i]->renderer, canvas->childs[i]->texture, NULL, &(SDL_Rect){canvas->childs[i]->position.x, canvas->childs[i]->position.y, canvas->childs[i]->width, canvas->childs[i]->height});
		}
		i++;
	}
	surface = SDL_CreateRGBSurfaceWithFormat(0, canvas->width, canvas->height, 32, SDL_PIXELFORMAT_ARGB8888);
	SDL_RenderReadPixels(canvas->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
	IMG_SavePNG(surface, path);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_SetRenderTarget(canvas->renderer, NULL);
}
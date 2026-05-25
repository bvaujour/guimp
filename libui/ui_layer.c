/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 10:15:42 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:03:14 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

void	ui_layer_open_image(t_widget *layer, const char *path)
{
	SDL_Surface		*surface;
	SDL_Texture		*texture;

	surface = IMG_Load(path);
	if (surface)
	{
		texture = SDL_CreateTextureFromSurface(layer->renderer, surface);
		if (texture)
		{
			SDL_SetRenderTarget(layer->renderer, layer->texture);
			SDL_RenderCopy(layer->renderer, texture, NULL, NULL);
			SDL_SetRenderTarget(layer->renderer, NULL);
			SDL_DestroyTexture(texture);
			ui_window_set_is_dirty(layer->owning_window, true);
		}
		SDL_FreeSurface(surface);
	}
}

void		ui_layer_event(t_widget *layer)
{
	SDL_Point		absolute;
	t_layer_data	*data;

	if (layer->core->event.type == SDL_MOUSEMOTION || layer->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		absolute = ui_get_absolute_position(layer);
		data = (t_layer_data *)layer->data;
		if (layer->core->mouse.buttons[SDL_BUTTON_LEFT])
			ui_widget_call_onclicked(layer->core, layer, (int)floorf(layer->core->mouse.position.x - absolute.x) / data->zoom, (int)floorf(layer->core->mouse.position.y - absolute.y) / data->zoom, SDL_BUTTON_LEFT);
		else if (layer->core->mouse.buttons[SDL_BUTTON_RIGHT])
			ui_widget_call_onclicked(layer->core, layer, (int)floorf(layer->core->mouse.position.x - absolute.x) / data->zoom, (int)floorf(layer->core->mouse.position.y - absolute.y) / data->zoom, SDL_BUTTON_RIGHT);
	}
	else if (layer->core->event.type == SDL_MOUSEBUTTONUP)
	{
		data = (t_layer_data *)layer->data;
		absolute = ui_get_absolute_position(layer);
		ui_widget_call_onreleased(layer->core, layer, (int)floorf(layer->core->mouse.position.x - absolute.x) / data->zoom, (int)floorf(layer->core->mouse.position.y - absolute.y) / data->zoom, layer->core->event.button.button);
	}
}


void		ui_layer_destroy(t_widget *layer)
{
	t_layer_data	*data;
	int	i;
	
	i = 0;
	data = (t_layer_data *)layer->data;
	SDL_FreeSurface(data->surface);
	while (i < UI_MAX_SNAPSHOTS)
	{
		SDL_DestroyTexture(data->snapshots[i]);
		i++;
	}
}

t_widget	*ui_create_layer(t_widget *parent)
{
	t_widget		*layer;
	t_layer_data	*data;
	t_canvas_data	*cdata;
	if (parent == NULL || parent->type != CANVAS)
		return (NULL);
	layer = ui_new_widget(parent, (SDL_Rect){0, 0, parent->width, parent->height}, LAYER, UI_MAX_LAYER_CHILDS);
	if (!layer)
		return (ui_destroy_widget(layer), NULL);
	layer->data = malloc(sizeof(t_layer_data));
	if (!layer->data)
		return (ui_destroy_widget(layer), NULL);
	data = (t_layer_data *)layer->data;
	*data = (t_layer_data){0};
	cdata = parent->data;
	ft_strcpy(layer->name, "layer");
	data->zoom = cdata->zoom;
	data->texture_width = cdata->texture_width;
	data->texture_height = cdata->texture_height;
	data->surface = SDL_CreateRGBSurfaceWithFormat(0, data->texture_width, data->texture_height, 32, SDL_PIXELFORMAT_ARGB8888);
	SDL_FillRect(data->surface, NULL, SDL_MapRGBA(data->surface->format, 255, 255, 255, 255));
	ui_set_widget_colors(layer, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	layer->outline = 1;
	layer->render = ui_widget_basic_render;
	layer->destroy = ui_layer_destroy;
	layer->event = ui_layer_event;
	layer->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW); 
	layer->texture = SDL_CreateTexture(layer->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, data->texture_width, data->texture_height);
	SDL_SetTextureBlendMode(layer->texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_BLEND);
	if (ui_add_child(parent, layer) != UI_SUCCESS)
		return (ui_destroy_widget(layer), NULL);
	ui_canvas_zoom(parent, 0);
	data->snapshot_index = -1;
	ui_clear_layer(layer, 0);
	ui_layer_snapshot(layer);
	return (layer);
}

t_widget	*ui_create_layer_from_image(t_widget *parent, const char *path)
{
	t_widget		*layer;
	t_layer_data	*data;
	SDL_Surface		*surface;
	SDL_Texture		*texture;

	if (parent == NULL)
		return (NULL);

	surface = IMG_Load(path);
	if (surface == NULL)
	{
		return (NULL);
	}
	layer = ui_new_widget(parent, (SDL_Rect){parent->width / 2 - surface->w / 2, parent->height / 2 - surface->h / 2, surface->w, surface->h}, LAYER, UI_MAX_LAYER_CHILDS);
	if (!layer)
		return (ui_destroy_widget(layer), NULL);
	layer->data = malloc(sizeof(t_layer_data));
	if (!layer->data)
		return (ui_destroy_widget(layer), NULL);
	data = (t_layer_data *)layer->data;
	*data = (t_layer_data){0};
	ft_strcpy(layer->name, "layer");
	data->zoom = 1.0f;
	data->surface = surface;
	ui_set_widget_colors(layer, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	layer->outline = 1;
	layer->render = ui_widget_basic_render;
	layer->destroy = ui_layer_destroy;
	layer->event = ui_layer_event;
	layer->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW); 
	layer->texture = SDL_CreateTexture(layer->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, layer->width, layer->height);
	data->texture_width = layer->width;
	data->texture_height = layer->height;
	SDL_SetTextureBlendMode(layer->texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_BLEND);
	texture = SDL_CreateTextureFromSurface(layer->renderer, surface);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderCopy(layer->renderer, texture, NULL, NULL);
	SDL_SetRenderTarget(layer->renderer, NULL);
	SDL_DestroyTexture(texture);
	if (ui_add_child(parent, layer) != UI_SUCCESS)
		return (ui_destroy_widget(layer), NULL);
	ui_canvas_zoom(parent, 0);
	data->snapshot_index = -1;
	ui_layer_snapshot(layer);
	return (layer);
}

t_widget	*ui_create_layer_from_copy_selection(t_widget *parent, int x, int y)
{
	t_widget		*layer;
	t_layer_data	*data;
	int				width;
	int				height;
	if (parent == NULL)
		return (NULL);
	if (parent->core->copy_info.texture == NULL)
		return (NULL);
	SDL_QueryTexture(parent->core->copy_info.texture, NULL, NULL, &width, &height);
	layer = ui_new_widget(parent, (SDL_Rect){x, y, width, height}, LAYER, UI_MAX_LAYER_CHILDS);
	if (!layer)
		return (ui_destroy_widget(layer), NULL);
	layer->data = malloc(sizeof(t_layer_data));
	if (!layer->data)
		return (ui_destroy_widget(layer), NULL);
	data = (t_layer_data *)layer->data;
	*data = (t_layer_data){0};
	ft_strcpy(layer->name, "layer");
	data->zoom = 1.0f;
	data->surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_ARGB8888);
	ui_set_widget_colors(layer, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	layer->outline = 1;
	layer->render = ui_widget_basic_render;
	layer->destroy = ui_layer_destroy;
	layer->event = ui_layer_event;
	layer->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW); 
	layer->texture = SDL_CreateTexture(layer->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, layer->width, layer->height);
	data->texture_width = layer->width;
	data->texture_height = layer->height;
	SDL_SetTextureBlendMode(layer->texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderCopy(layer->renderer, parent->core->copy_info.texture, NULL, NULL);
	SDL_SetRenderTarget(layer->renderer, NULL);
	if (ui_add_child(parent, layer) != UI_SUCCESS)
		return (ui_destroy_widget(layer), NULL);
	ui_canvas_zoom(parent, 0);
	data->snapshot_index = -1;
	ui_layer_snapshot(layer);
	return (layer);
}
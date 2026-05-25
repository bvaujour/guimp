/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_layer_user.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 18:55:09 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

float	ui_layer_get_zoom(t_widget *layer)
{
	t_layer_data	*data;

	data = (t_layer_data *)layer->data;
	return (data->zoom);
}

void ui_copy_layer(t_widget *layer, t_rect selection)
{
	t_layer_data	*data;
	SDL_Surface		*surface;
	SDL_Point		absolute;
	data = (t_layer_data *)layer->data;
	absolute = ui_get_absolute_position(layer);
	selection.x -= absolute.x;
	selection.y -= absolute.y;
	selection.x = (int)floorf(selection.x / data->zoom);
	selection.y = (int)floorf(selection.y / data->zoom);
	ft_printf("zoom: %f\n", data->zoom);
	selection.width =(int)ceilf(selection.width / data->zoom);
	selection.height = (int)ceilf(selection.height / data->zoom);
	ft_printf("copy : x:%d y:%d w:%d h:%d\n", selection.x, selection.y, selection.width, selection.height);

	surface = SDL_CreateRGBSurfaceWithFormat(0, selection.width, selection.height, 32, SDL_PIXELFORMAT_ARGB8888);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderReadPixels(layer->renderer, &(SDL_Rect){selection.x, selection.y, selection.width, selection.height}, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
	SDL_SetRenderTarget(layer->renderer, NULL);
	if (layer->core->copy_info.texture)
		SDL_DestroyTexture(layer->core->copy_info.texture);
	layer->core->copy_info.texture = SDL_CreateTextureFromSurface(layer->renderer, surface);
	SDL_FreeSurface(surface);
}

void ui_paste_on_layer(t_widget *layer)
{
	t_layer_data	*data;
	SDL_Point		absolute;
	int				x;
	int				y;
	int				height;
	int				width;

	if (layer->core->hovered_widget != layer)
		return ;
	if (layer->core->copy_info.texture == NULL)
		return ;
	data = (t_layer_data *)layer->data;
	absolute = ui_get_absolute_position(layer);
	x = layer->core->mouse.position.x - absolute.x;
	y = layer->core->mouse.position.y - absolute.y;
	x = (int)floorf(x / data->zoom);
	y = (int)floorf(y / data->zoom);
	SDL_QueryTexture(layer->core->copy_info.texture, NULL, NULL, &width, &height);
	ft_printf("paste : x:%d y:%d w:%d h:%d", x, y, width, height);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderCopy(layer->renderer, layer->core->copy_info.texture, NULL, &(SDL_Rect){x, y, width, height});
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}


t_img	ui_layer_get_img(t_widget *layer)
{
	t_img			img;
	t_layer_data	*data;
	
	data = (t_layer_data *)layer->data;
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderReadPixels(layer->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, data->surface->pixels, data->surface->pitch);
	SDL_SetRenderTarget(layer->renderer, NULL);
	img.width = data->surface->clip_rect.w;
	img.height = data->surface->clip_rect.h;
	img.pixels = data->surface->pixels;
	img.pitch = data->surface->pitch;
	return (img);
}

void	ui_layer_set_img(t_widget *layer, t_img img)
{
	SDL_UpdateTexture(layer->texture, NULL, img.pixels, img.pitch);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void	ui_draw_rect(t_widget *layer, t_rect rect, int thickness, unsigned int color)
{
	SDL_Color		c;
	int				i;

	if (layer->type != LAYER)
		return ;
	if (rect.width <= 0)
		rect.width = 1;
	if (rect.height <= 0)
		rect.height = 1;
	c = ui_unpack_color(color);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	if (thickness > rect.width / 2 || thickness > rect.height / 2)
		SDL_RenderFillRect(layer->renderer, &(SDL_Rect){rect.x, rect.y, rect.width, rect.height});
	else
	{
		i = 0;
		while (i < thickness)
		{
			SDL_RenderDrawRect(layer->renderer, &(SDL_Rect){rect.x + i, rect.y + i, rect.width - 2 * i, rect.height - 2 * i});
			i++;
		}
	}
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void ui_draw_filled_rect(t_widget *layer, t_rect rect, unsigned int color)
{
	SDL_Color	c;

	if (rect.width <= 0)
		rect.width = 1;
	if (rect.height <= 0)
		rect.height = 1;
	c = ui_unpack_color(color);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(layer->renderer, &(SDL_Rect){rect.x, rect.y, rect.width, rect.height});
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}


void	ui_draw_point(t_widget *layer, int x, int y, unsigned int color)
{
	SDL_Color	c;

	c = ui_unpack_color(color);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderDrawPoint(layer->renderer, x, y);
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void	ui_draw_line(t_widget *layer, int x1, int y1, int x2, int y2, unsigned int color)
{
	SDL_Color	c;

	c = ui_unpack_color(color);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderDrawLine(layer->renderer, x1, y1, x2, y2);
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void	ui_draw_line_thick(t_widget *layer, int x1, int y1, int x2, int y2, int thickness, unsigned int color)
{
	SDL_Color	c = ui_unpack_color(color);

	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);

	for (int i = -thickness / 2; i <= thickness / 2; i++)
	{
		ft_printf("drawing line x1:%d x2:%d y1:%d y2:%d\n", x1 + i, x2 + i, y1, y2);
		SDL_RenderDrawLine(layer->renderer, x1 + i, y1, x2 + i, y2);
		ft_printf("drawing line x1:%d x2:%d y1:%d y2:%d\n", x1, x2, y1 + i, y2 + i);
		SDL_RenderDrawLine(layer->renderer, x1, y1 + i, x2, y2 + i);
	}
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);

}


void	ui_draw_circle(t_widget *layer, int center_x, int center_y, int radius, unsigned int color)
{
	int x = (radius - 1);
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = tx - (radius * 2);
	SDL_Color	c;

	if (radius == 0)
		radius = 1;
	c = ui_unpack_color(color);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	SDL_SetRenderTarget(layer->renderer, layer->texture);

	while (x >= y)
	{
		SDL_RenderDrawPoint(layer->renderer, center_x + x, center_y - y);
		SDL_RenderDrawPoint(layer->renderer, center_x + x, center_y + y);
		SDL_RenderDrawPoint(layer->renderer, center_x - x, center_y - y);
		SDL_RenderDrawPoint(layer->renderer, center_x - x, center_y + y);
		SDL_RenderDrawPoint(layer->renderer, center_x + y, center_y - x);
		SDL_RenderDrawPoint(layer->renderer, center_x + y, center_y + x);
		SDL_RenderDrawPoint(layer->renderer, center_x - y, center_y - x);
		SDL_RenderDrawPoint(layer->renderer, center_x - y, center_y + x);
		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}
		if (error > 0)
		{
			--x;
			tx += 2;
			error += tx - (radius * 2);
		}
	}
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void	ui_draw_circle_moins_opti_mais_plus_beau(t_widget *layer, int center_x, int center_y, int radius, int thickness, unsigned int color)
{
	int			x;
	int			y;
	int			dist;
	int			r_outer;
	int			r_inner;
	SDL_Color	c;

	c = ui_unpack_color(color);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	SDL_SetRenderTarget(layer->renderer, layer->texture);

	if (thickness > radius)
		thickness = radius;
	r_outer = radius * radius;
	r_inner = (radius - thickness) * (radius - thickness);

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			dist = x * x + y * y;
			if (dist <= r_outer && dist >= r_inner)
				SDL_RenderDrawPoint(layer->renderer, center_x + x, center_y + y);
			x++;
		}
		y++;
	}
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void	ui_draw_filled_circle(t_widget *layer, int center_x, int center_y, int radius, unsigned int color)
{
	int	x;
	int y;
	int sqr_radius;
	int	sqr_dy;
	SDL_Color	c;

	if (radius == 0)
		radius = 1;
	c = ui_unpack_color(color);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	// SDL_SetRenderTarget(layer->renderer, layer->texture);
	sqr_radius = radius * radius;
	y = center_y - radius;
	while (y < center_y + radius)
	{
		x = center_x - radius;
		sqr_dy = (center_y - y) * (center_y - y);
		while (x < center_x + radius)
		{
			if (sqr_dy + (center_x - x) * (center_x - x) <= sqr_radius)
				SDL_RenderDrawPoint(layer->renderer, x, y);
			x++;
		}
		y++;
	}
	// SDL_SetRenderTarget(layer->renderer, NULL);
	// ui_window_set_is_dirty(layer->owning_window, true);
}


void	ui_bucket_layer(t_widget *layer, int start_x, int start_y, unsigned color)
{
	unsigned int	target_color;
	SDL_Point		*stack;
	int				top;
	int				stack_size;
	SDL_Point		current;
	t_img			img;

	img = ui_layer_get_img(layer);
	if (start_x < 0 || start_x >= img.width || start_y < 0 || start_y >= img.height)
		return;
	target_color = img.pixels[start_y * img.width + start_x];
	if (target_color == color)
		return;
	stack_size = (img.width * img.height) * 3;
	stack = malloc(stack_size * sizeof(SDL_Point));
	top = 0;
	stack[top++] = (SDL_Point){start_x, start_y};
	while (top > 0)
	{
		current = stack[--top];
		if (current.x < 0 || current.x >= img.width || current.y < 0 || current.y >= img.height)
			continue ;
		if (img.pixels[current.y * img.width + current.x] != target_color || img.pixels[current.y * img.width + current.x] == color)
			continue ;
		img.pixels[current.y * img.width + current.x] = color;
		if (top < stack_size)
			stack[top++] = (SDL_Point){current.x + 1, current.y};
		if (top < stack_size)
			stack[top++] = (SDL_Point){current.x - 1, current.y};
		if (top < stack_size)
			stack[top++] = (SDL_Point){current.x, current.y + 1};
		if (top < stack_size)
			stack[top++] = (SDL_Point){current.x, current.y - 1};
	}
	free(stack);
	ui_layer_set_img(layer, img);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void	ui_clear_layer(t_widget *layer, unsigned int color)
{
	SDL_Color	c;

	c = ui_unpack_color(color);
	SDL_SetRenderDrawColor(layer->renderer, c.r, c.g, c.b, c.a);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderClear(layer->renderer);
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}


void	ui_clear_layer_from_selection(t_widget *layer)
{
	SDL_Point		absolute;
	t_rect 			selection;
	t_layer_data	*data;

	selection = ui_get_selection(layer->owning_window);
	absolute = ui_get_absolute_position(layer);
	data = (t_layer_data *)layer->data;
	selection.x -= absolute.x;
	selection.y -= absolute.y;
	selection.x = (int)floorf(selection.x / data->zoom);
	selection.y = (int)floorf(selection.y / data->zoom);
	selection.width =(int)ceilf(selection.width / data->zoom);
	selection.height = (int)ceilf(selection.height / data->zoom);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_NONE);
	ui_draw_filled_rect(layer, selection, 0x00000000);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_BLEND);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void ui_erase_layer(t_widget *layer, int x, int y, int width, int height, unsigned char force)
{
	SDL_BlendMode erase_mode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_SetRenderDrawBlendMode(layer->renderer, erase_mode);
	SDL_SetRenderDrawColor(layer->renderer, 0, 0, 0, force);
	SDL_RenderFillRect(layer->renderer, &(SDL_Rect){x, y, width, height});
	SDL_SetRenderTarget(layer->renderer, NULL);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_BLEND);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void ui_erase_layer_sticker(t_widget *layer, t_widget *sticker, t_rect dest_rect, unsigned char force)                                                                                      
{
	SDL_BlendMode erase_mode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
    if (!sticker || !sticker->texture)
		return;
	if (dest_rect.width <= 0)
		dest_rect.width = 1;
	if (dest_rect.height <= 0)
		dest_rect.height = 1;
    SDL_SetRenderTarget(layer->renderer, layer->texture);
    SDL_SetTextureBlendMode(sticker->texture, erase_mode);
    SDL_SetTextureAlphaMod(sticker->texture, force);
    SDL_RenderCopy(layer->renderer, sticker->texture, NULL, &(SDL_Rect){dest_rect.x, dest_rect.y, dest_rect.width, dest_rect.height});
    SDL_SetTextureBlendMode(sticker->texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(layer->renderer, NULL);
    ui_window_set_is_dirty(layer->owning_window, true);
}

void ui_draw_sticker(t_widget *layer, t_widget *sticker, t_rect dest_rect, unsigned int color_mod)
{
	SDL_Color		color;

	if (!sticker || !sticker->texture)
		return;
	if (dest_rect.width <= 0)
		dest_rect.width = 1;
	if (dest_rect.height <= 0)
		dest_rect.height = 1;
	color = ui_unpack_color(color_mod);
	SDL_SetTextureBlendMode(sticker->texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_SetTextureColorMod(sticker->texture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(sticker->texture, color.a);
	SDL_RenderCopy(layer->renderer, sticker->texture, NULL, &(SDL_Rect){dest_rect.x, dest_rect.y, dest_rect.width, dest_rect.height});
	SDL_SetRenderTarget(layer->renderer, NULL);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void ui_layer_export(t_widget *layer, const char *path)
{
	t_layer_data	*data;

	data = (t_layer_data *)layer->data;
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderReadPixels(layer->renderer, NULL, SDL_PIXELFORMAT_ARGB8888, data->surface->pixels, data->surface->pitch);
	SDL_SetRenderTarget(layer->renderer, NULL);
	IMG_SaveJPG(data->surface, path, 100);
}

void ui_layer_snapshot(t_widget *layer)
{
	t_layer_data	*data;
	SDL_Texture		*tmp;
	int				i;

	data = (t_layer_data *)layer->data;
	if (data->snapshot_index < UI_MAX_SNAPSHOTS - 1)
		data->snapshot_index++;
	else
	{
		tmp = data->snapshots[0];
		i = 0;
		while (i < UI_MAX_SNAPSHOTS - 1) 
		{
			data->snapshots[i] = data->snapshots[i + 1];
			i++;
		}
		data->snapshots[i] = tmp;
	}
	if (data->snapshots[data->snapshot_index] == NULL)
		data->snapshots[data->snapshot_index] = SDL_CreateTexture(layer->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, data->texture_width, data->texture_height);
	SDL_SetRenderTarget(layer->renderer, data->snapshots[data->snapshot_index]);
	SDL_SetTextureBlendMode(layer->texture, SDL_BLENDMODE_NONE);
	SDL_RenderCopy(layer->renderer, layer->texture, NULL, NULL);
	SDL_SetTextureBlendMode(layer->texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(layer->renderer, NULL);
	ft_printf("snapshot at index :%d\n", data->snapshot_index);
	data->snapshots_length = data->snapshot_index;
	ft_printf("new index :%d, new lenght:%d\n", data->snapshot_index, data->snapshots_length);
}

void ui_layer_restore_previous_snapshot(t_widget *layer)
{
	t_layer_data	*data;

	data = (t_layer_data *)layer->data;
	if (data->snapshot_index == 0)
		return ;
	data->snapshot_index--;
	SDL_SetTextureBlendMode(data->snapshots[data->snapshot_index], SDL_BLENDMODE_NONE);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderCopy(layer->renderer, data->snapshots[data->snapshot_index], NULL, NULL);
	SDL_SetRenderTarget(layer->renderer, NULL);
	ft_printf("restore snap index :%d\n", data->snapshot_index);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void ui_layer_restore_next_snapshot(t_widget *layer)
{
	t_layer_data	*data;

	data = (t_layer_data *)layer->data;
	if (data->snapshot_index == data->snapshots_length)
		return ;
	data->snapshot_index++;
	SDL_SetTextureBlendMode(data->snapshots[data->snapshot_index], SDL_BLENDMODE_NONE);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderCopy(layer->renderer, data->snapshots[data->snapshot_index], NULL, NULL);
	SDL_SetRenderTarget(layer->renderer, NULL);
	ft_printf("restore snap index :%d\n", data->snapshot_index);
	ui_window_set_is_dirty(layer->owning_window, true);
}

void  ui_draw_text(t_widget *layer, int x, int y, const char *text, unsigned int color)                                                                                                    
{                                                                                                                                                                                                                                                                                                                                                       
	SDL_Surface             *text_surface;                                                                                                                                                
	SDL_Texture             *text_texture;
	SDL_Color               sdl_color;

	sdl_color.r = (color >> 16) & 0xFF;                                                                                                                                                   
	sdl_color.g = (color >> 8) & 0xFF;                                                                                                                                                    
	sdl_color.b = color & 0xFF;
	sdl_color.a = (color >> 24) & 0xFF;                                                                                                                                                   
	text_surface = TTF_RenderUTF8_Blended(layer->core->font, text, sdl_color);                                                                                                           
	if (!text_surface)                                                                                                                                                                    
			return ;                                                                                                                                                                      
	text_texture = SDL_CreateTextureFromSurface(layer->renderer, text_surface);                                                                                                    
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_RenderCopy(layer->renderer, text_texture, NULL, &(SDL_Rect){x, y, text_surface->w, text_surface->h});                                                                                                                                           
	SDL_SetRenderTarget(layer->renderer, NULL);
	SDL_DestroyTexture(text_texture);                                                                                                                                                     
	SDL_FreeSurface(text_surface);
	ui_window_set_is_dirty(layer->owning_window, true);                                                                                                                                  
}

unsigned int	ui_layer_get_pixel(t_widget *layer, int x, int y)
{
	t_img			img;

	img = ui_layer_get_img(layer);
	return (img.pixels[y * img.width + x]);
}

void	ui_layer_write(t_widget *layer, char *str, char *font_path, int font_size, unsigned int color)
{
	TTF_Font		*font;
	SDL_Surface		*surface;
	SDL_Texture		*texture;
	t_layer_data	*data;

	font = TTF_OpenFont(font_path, font_size);
	surface = TTF_RenderText_Blended_Wrapped(font, str, ui_unpack_color(color), 0);
	if (!surface)
	{
		ui_clear_layer(layer, 0);
		return ;
	}
	texture = SDL_CreateTextureFromSurface(layer->renderer, surface);
	if (layer->texture)
		SDL_DestroyTexture(layer->texture);
	data = layer->data;
	data->texture_width = surface->w;
	data->texture_height = surface->h;
	layer->width = data->texture_width * data->zoom;
	layer->height = data->texture_height * data->zoom;
	layer->texture = SDL_CreateTexture(layer->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, layer->width, layer->height);
	SDL_SetTextureBlendMode(layer->texture, SDL_BLENDMODE_BLEND);
	ui_clear_layer(layer, 0);
	SDL_SetRenderTarget(layer->renderer, layer->texture);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_NONE);
	SDL_RenderCopy(layer->renderer, texture, NULL, NULL);
	SDL_SetRenderTarget(layer->renderer, NULL);
	SDL_SetRenderDrawBlendMode(layer->renderer, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}
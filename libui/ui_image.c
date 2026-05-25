/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_image.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 03:22:43 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"


void	ui_image_destroy(t_widget *image)
{
	t_image_data	*data;

	data = (t_image_data *)image->data;
	if (data->surface)
		SDL_FreeSurface(data->surface);
}

unsigned int	ui_image_get_pixel(t_widget *image, int x, int y)
{
	t_image_data 	*data;
	unsigned int	*pixels;
	if (image->type != IMAGE)
		return (0);
	data = (t_image_data *)image->data;
	SDL_LockSurface(data->surface);
	x = x * ((float)data->surface->w / image->width);
	y = y * ((float)data->surface->h / image->height);
	if (x < 0 || y < 0 || x >= data->surface->w || y >= data->surface->h)
		return 0;
	pixels = data->surface->pixels;
	SDL_UnlockSurface(data->surface);
	return (pixels[y * data->surface->w + x]);
}

void		ui_image_event(t_widget *image)
{
	SDL_Point	absolute;
	if (image->core->event.type == SDL_MOUSEMOTION || image->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		absolute = ui_get_absolute_position(image);
		if (image->core->mouse.buttons[SDL_BUTTON_LEFT])
			ui_widget_call_onclicked(image->core, image, image->core->mouse.position.x - absolute.x, image->core->mouse.position.y - absolute.y, SDL_BUTTON_LEFT);
		else if (image->core->mouse.buttons[SDL_BUTTON_RIGHT])
			ui_widget_call_onclicked(image->core, image, image->core->mouse.position.x - absolute.x, image->core->mouse.position.y - absolute.y, SDL_BUTTON_RIGHT);
	}
	else if (image->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(image);
		ui_widget_call_onreleased(image->core, image, image->core->mouse.position.x - absolute.x, image->core->mouse.position.y - absolute.y, image->core->event.button.button);
	}
}

const char	*ui_image_get_image_path(t_widget *image)
{
	t_image_data	*data;

	if (image->type != IMAGE)
		return (NULL);
	data = (t_image_data *)image->data;
	return (data->image_path);
}

void	ui_image_set_texture(t_widget *image, const char *image_path)
{
	t_image_data	*data;

	data = (t_image_data *)image->data;
	if (image->type != IMAGE)
		return ;
	if (ft_strcmp(image_path, data->image_path) == 0)
		return ;
	if (image->texture)
		SDL_DestroyTexture(image->texture);
	IMG_LoadTexture(image->renderer, image_path);
	ft_strcpy(data->image_path, image_path);
}

t_widget	*ui_create_image(t_widget *parent, t_rect relative_rect, const char *image_path)
{
	t_widget		*image;
	t_image_data	*data;
	SDL_Surface		*surface;

	if (parent == NULL)
		return (NULL);
	image = ui_new_widget(parent, (SDL_Rect){relative_rect.x, relative_rect.y, relative_rect.width, relative_rect.height}, IMAGE, UI_MAX_IMAGE_CHILDS);
	if (image == NULL)
		return (ui_destroy_widget(image), NULL);
	image->data = malloc(sizeof(t_image_data));
	if (!image->data)
		return (ui_destroy_widget(image), NULL);
	data = (t_image_data *)image->data;
	*data = (t_image_data){0};
	if (image_path == NULL)
	{
		ft_strcpy(image->name, "image");
		data->surface =  SDL_CreateRGBSurfaceWithFormat(0, relative_rect.width, relative_rect.height, 32, SDL_PIXELFORMAT_ARGB8888);
	}
	else
	{
		surface = IMG_Load(image_path);
		if (surface == NULL)
		{
			ft_printf("ui_create_image: image load failed\n");
			return (ui_destroy_widget(image), NULL);
		}
		data->surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
		SDL_FreeSurface(surface);
		ft_strcpy(data->image_path, image_path);
	}
	if (data->surface == NULL)
		return (ui_destroy_widget(image), NULL);
	image->outline = 0;
	ui_set_widget_colors(image, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	image->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	image->render = ui_widget_basic_render;
	image->destroy = ui_image_destroy;
	image->event = ui_image_event;
	image->texture = SDL_CreateTextureFromSurface(image->renderer, data->surface);
	SDL_SetTextureBlendMode(image->texture, SDL_BLENDMODE_BLEND);
	if (image->texture == NULL)
		return (ui_destroy_widget(image), NULL);
	if (ui_add_child(parent, image) != UI_SUCCESS)
		return (ui_destroy_widget(image), NULL);
	return (image);
}
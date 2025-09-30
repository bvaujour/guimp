/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_container.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:00:11 by injah             #+#    #+#             */
/*   Updated: 2025/09/30 09:49:03 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_rebuild_container(t_container *container)
{
	SDL_Surface	*surface;
	t_widget	*widget;
	SDL_Rect	current_rect;
	int			i;

	i = -1;
	current_rect = (SDL_Rect){0};
	current_rect.x = container->wheel_offset.x;
	current_rect.y = container->wheel_offset.y;
	surface = ui_new_surf(container->rect.w, container->rect.h);
	ui_fill_surf(surface, container->config->container_color);
	while (++i < container->nb_widget)
	{
		widget = &container->widgets[i];
		ui_blit(widget->surface, surface, current_rect.x, current_rect.y);
		widget->rect.x = container->rect.x + current_rect.x;
		widget->rect.y = container->rect.y + current_rect.y;
		if (container->direction == LEFTTORIGHT)
		{
			current_rect.x += widget->rect.w;
			current_rect.w += widget->rect.w;
		}
		else if (container->direction == TOPTOBOTTOM)
		{
			current_rect.y += widget->rect.h;
			current_rect.h += widget->rect.h;
		}
	}
	container->max_scroll.x = current_rect.w - container->rect.w;
	container->max_scroll.y = current_rect.h - container->rect.h;
	if (container->texture)
		SDL_DestroyTexture(container->texture);
	container->texture = SDL_CreateTextureFromSurface(container->ctx->renderer, surface);
	SDL_FreeSurface(surface);
}

t_container	*ui_create_container(t_ctx *context, e_direction direction, SDL_Rect rect)
{
	t_container	*container;
	int	index;

	index = context->nb_container;
	container = &context->containers[index];
	*container = (t_container){0};
	container->config = context->config;
	container->rect.x = rect.x;
	container->rect.y = rect.y;
	container->rect.w = rect.w;
	container->rect.h = rect.h;
	container->ctx = context;
	container->core = context->core;
	container->direction = direction;
	container->is_durty = true;
	context->nb_container++;
	return (container);
}

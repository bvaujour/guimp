/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_container.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:00:11 by injah             #+#    #+#             */
/*   Updated: 2025/09/30 19:20:05 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_rebuild_container(t_container *container)
{
	SDL_Surface	*surface;
	t_widget	*widget;
	SDL_Rect	total_widget_rect;
	int			i;

	i = -1;
	total_widget_rect = (SDL_Rect){0};
	total_widget_rect.x = container->wheel_offset.x;
	total_widget_rect.y = container->wheel_offset.y;
	surface = ui_new_surf(container->rect.w, container->rect.h, &container->config->container_color);
	while (++i < container->nb_widget)
	{
		widget = &container->widgets[i];
		ui_blit(widget->surface, surface, total_widget_rect.x, total_widget_rect.y);
		widget->rect.x = container->rect.x + total_widget_rect.x;
		widget->rect.y = container->rect.y + total_widget_rect.y;
		if (container->direction == LEFTTORIGHT)
		{
			total_widget_rect.x += widget->rect.w;
			total_widget_rect.h += widget->rect.h;
		}
		else if (container->direction == TOPTOBOTTOM)
		{
			total_widget_rect.y += widget->rect.h;
			total_widget_rect.h += widget->rect.h;
		}
	}
	if (total_widget_rect.h > container->rect.h)
		container->max_scroll.y = total_widget_rect.h - container->rect.h;
	if (total_widget_rect.w > container->rect.w)
		container->max_scroll.x = total_widget_rect.w - container->rect.w;
	if (container->texture)
		SDL_DestroyTexture(container->texture);
	container->texture = SDL_CreateTextureFromSurface(container->ctx->renderer, surface);
	SDL_FreeSurface(surface);
}

void	ui_build_container(t_container *container)
{
	SDL_Surface	*surface_widgets;
	SDL_Surface	*surface_container;
	t_widget	*widget;
	int			i;

	i = 0;
	surface_widgets = ui_new_surf(container->total_widget_rect.w, container->total_widget_rect.h, &container->core->config.container_color);
	while (i < container->nb_widget)
	{
		widget = &container->widgets[i];
		ui_blit(widget->surface, surface_widgets, widget->relative.x, widget->relative.y);
		i++;
	}
	if (container->texture)
		SDL_DestroyTexture(container->texture);
	surface_container = ui_new_surf(container->rect.w, container->rect.h, &container->core->config.container_color);
	ui_blit(surface_widgets, surface_container, container->wheel_offset.x, container->wheel_offset.y);
	container->texture = SDL_CreateTextureFromSurface(container->ctx->renderer, surface_container);
	SDL_FreeSurface(surface_widgets);
	SDL_FreeSurface(surface_container);
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

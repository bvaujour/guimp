/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_box.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:00:11 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 06:18:05 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_build_box(t_box *box)
{
	int			i;
	SDL_Surface	*surface;
	t_widget	*widget;

	i = 0;
	surface = ui_new_surf(box->rect.w, box->rect.h, &box->core->config.box_color);
	while (i < box->nb_widget)
	{
		widget = &box->widgets[i];
		// SDL_BlitScaled(widget->surface, NULL, surface, &(SDL_Rect){widget->relative.x, widget->relative.y, widget->rect.w, box->rect.h});
		ui_blit(widget->surface, surface, widget->relative.x, widget->relative.y);
		i++;
	}
	if (box->texture)
		SDL_DestroyTexture(box->texture);
	box->texture = SDL_CreateTextureFromSurface(box->context->renderer, surface);
	SDL_FreeSurface(surface);
}

void	ui_box_consume_scroll(t_box *box)
{
	int			i;
	t_widget	*widget;

	i = 0;
	box->scroll.x += box->core->wheel.x * SCROLL_SPEED;
	box->scroll.y += box->core->wheel.y * SCROLL_SPEED;
	if (box->scroll.y > 0)
	{
		box->scroll.y = 0;
		return ;
	}
	else if (box->scroll.y < box->max_scroll.y)
	{
		box->scroll.y = box->max_scroll.y;
		return ;
	}
	if (box->scroll.x > 0)
	{
		box->scroll.x = 0;
		return ;
	}
	else if (box->scroll.x < box->max_scroll.x)
	{
		box->scroll.x = box->max_scroll.x;
		return ;
	}
	while (i < box->nb_widget)
	{
		widget = &box->widgets[i];
		widget->relative.x += box->core->wheel.x * SCROLL_SPEED;
		widget->relative.y += box->core->wheel.y * SCROLL_SPEED;
		widget->rect.x = box->rect.x + widget->relative.x;
		widget->rect.y = box->rect.y + widget->relative.y;
		i++;
	}
	printf("\nbox->scroll.y: %d\n", box->scroll.y);
	printf("box->max_scroll.y: %d\n", box->max_scroll.y);
	printf("box->scroll.x: %d\n", box->scroll.x);
	printf("box->max_scroll.x: %d\n", box->max_scroll.x);
}

t_box	*ui_create_box(t_context *context, e_direction direction, SDL_Rect rect)
{
	t_box	*box;
	int	index;

	index = context->nb_box;
	box = &context->boxs[index];
	*box = (t_box){0};
	box->rect.x = rect.x;
	box->rect.y = rect.y;
	box->rect.w = rect.w;
	box->rect.h = rect.h;
	box->context = context;
	box->core = context->core;
	box->flex_direction = direction;
	box->is_durty = true;
	context->nb_box++;
	return (box);
}

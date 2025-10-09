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

void	ui_build_box_texture(t_box *box)
{
	int			i;
	SDL_Surface	*surface;

	i = 0;
	surface = ui_new_surf(box->rect.w, box->rect.h, &box->core->config.box_color);
	while (i < box->nb_widget)
	{
		ui_blit(box->widgets[i].surface, surface, box->widgets[i].relative.x, box->widgets[i].relative.y);
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
	if (box->scroll.y + box->core->wheel.y * SCROLL_SPEED > 0 || box->scroll.y + box->core->wheel.y * SCROLL_SPEED < box->max_scroll.y)
		return ;
	if (box->scroll.x + box->core->wheel.x * SCROLL_SPEED > 0 || box->scroll.x + box->core->wheel.x * SCROLL_SPEED < box->max_scroll.x)
		return ;
	box->scroll.x += box->core->wheel.x * SCROLL_SPEED;
	box->scroll.y += box->core->wheel.y * SCROLL_SPEED;
	while (i < box->nb_widget)
	{
		widget = &box->widgets[i];
		widget->relative.x += box->core->wheel.x * SCROLL_SPEED;
		widget->relative.y += box->core->wheel.y * SCROLL_SPEED;
		i++;
	}
	printf("\nbox->scroll.y: %d\n", box->scroll.y);
	printf("box->max_scroll.y: %d\n", box->max_scroll.y);
	printf("box->scroll.x: %d\n", box->scroll.x);
	printf("box->max_scroll.x: %d\n", box->max_scroll.x);
}

void	ui_build_horizontal_box(t_box *box)
{
	int	i;
	SDL_Rect	widget_rect;
	int			nb_line;

	i = 0;
	if (box->inner_elements == -1)
		box->inner_elements = box->nb_widget;
	if (box->nb_widget == 0)
		return ;
	widget_rect = (SDL_Rect){0};
	nb_line = (box->nb_widget - 1) / box->wrap_at + 1;
	widget_rect.w = (box->rect.w - box->padding.left - box->padding.right - box->gap * (box->inner_elements - 1)) / box->inner_elements;
	widget_rect.h = (box->rect.h - box->padding.top - box->padding.bottom - box->gap * (nb_line - 1)) / nb_line;
	widget_rect.x = box->padding.left;
	widget_rect.y = box->padding.top;
	while (i < box->nb_widget)
	{
		box->widgets[i].relative = widget_rect;
		box->widgets[i].build(&box->widgets[i]);
		widget_rect.x += widget_rect.w + box->gap;
		if (box->wrap_at != -1 && (i + 1) % box->wrap_at == 0)
		{
			widget_rect.y += widget_rect.h + box->gap;
			widget_rect.x = box->padding.left;
		}
		i++;
	}
	if (box->nb_widget > box->inner_elements)
	{
		if (box->nb_widget < box->wrap_at)
			box->max_scroll.x = box->rect.w - (widget_rect.w * box->nb_widget + box->gap * (box->nb_widget - 1) + box->padding.left + box->padding.right);
		else
			box->max_scroll.x = box->rect.w - (widget_rect.w * box->wrap_at + box->gap * (box->wrap_at - 1) + box->padding.left + box->padding.right);
	}
}

void	ui_build_vertical_box(t_box *box)
{
	int	i;
	SDL_Rect	widget_rect;
	int			nb_line;

	if (box->inner_elements == -1)
		box->inner_elements = box->nb_widget;
	if (box->nb_widget == 0)
		return ;
	i = 0;
	widget_rect = (SDL_Rect){0};
	nb_line = (box->nb_widget - 1) / box->wrap_at + 1;
	widget_rect.w = (box->rect.w - box->padding.left - box->padding.right - box->gap * (nb_line - 1)) / nb_line;
	widget_rect.h = (box->rect.h - box->padding.top - box->padding.bottom - box->gap * (box->inner_elements - 1)) / box->inner_elements;
	widget_rect.x = box->padding.left;
	widget_rect.y = box->padding.top;
	while (i < box->nb_widget)
	{
		box->widgets[i].relative = widget_rect;
		box->widgets[i].build(&box->widgets[i]);
		widget_rect.y += widget_rect.h + box->gap;
		if ((i + 1) % box->wrap_at == 0)
		{
			widget_rect.x += widget_rect.w + box->gap;
			widget_rect.y = box->padding.left;
		}
		i++;
	}
	if (box->nb_widget > box->inner_elements)
	{
		if (box->nb_widget < box->wrap_at)
			box->max_scroll.y = box->rect.h - (widget_rect.h * box->nb_widget + box->gap * (box->nb_widget - 1) + box->padding.top + box->padding.bottom);
		else
			box->max_scroll.y = box->rect.h - (widget_rect.h * box->wrap_at + box->gap * (box->wrap_at - 1) + box->padding.top + box->padding.bottom);
	}
}

void	ui_init_box(t_box *box)
{
	box->gap = 2;
	box->padding.left = 2;
	box->padding.right = 2;
	box->padding.top = 2;
	box->padding.bottom = 2;
	box->wrap_at = 100;
	box->inner_elements = -1;
}

t_box	*ui_create_horizontal_box(t_context *context, int flex)
{
	t_box	*box;
	int		index;

	index = context->nb_box;
	box = &context->boxs[index];
	*box = (t_box){0};
	box->flex = flex;
	ui_init_box(box);
	box->build = &ui_build_horizontal_box;
	box->context = context;
	box->core = context->core;
	context->total_boxs_flex += flex;
	context->nb_box++;
	return (box);
}

t_box	*ui_create_vertical_box(t_context *context, int flex)
{
	t_box	*box;
	int		index;

	index = context->nb_box;
	box = &context->boxs[index];
	*box = (t_box){0};
	box->flex = flex;
	ui_init_box(box);
	box->build = &ui_build_vertical_box;
	box->context = context;
	box->core = context->core;
	context->total_boxs_flex += flex;
	context->nb_box++;
	return (box);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_build.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:19:36 by bvaujour          #+#    #+#             */
/*   Updated: 2025/10/04 13:09:48 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_place_windows(t_core *core)
{
	int			i;
	int			total_flex;
	SDL_Rect	window_rect;

	i = 0;
	total_flex = 0;
	window_rect = (SDL_Rect){0};
	while (i < core->nb_contexts)
	{
		total_flex += core->contexts[i].flex;
		i++;
	}
	i = 0;
	while (i < core->nb_contexts)
	{
		SDL_SetWindowPosition(core->contexts[i].window, window_rect.x, window_rect.y);
		if (core->flex_direction == HORIZONTAL)
		{
			window_rect.w = (core->screen_w * core->contexts[i].flex) / total_flex;
			window_rect.h = core->screen_h;
			SDL_SetWindowSize(core->contexts[i].window, window_rect.w, window_rect.h);
			core->contexts[i].rect = window_rect;
			window_rect.x += window_rect.w;
		}
		else
		{
			window_rect.w = core->screen_w;
			window_rect.h = (core->screen_h * core->contexts[i].flex) / total_flex;
			SDL_SetWindowSize(core->contexts[i].window, window_rect.w, window_rect.h);
			core->contexts[i].rect = window_rect;
			window_rect.y += window_rect.h;
		}
		i++;
	}
}

void	ui_place_boxs(t_context *context)
{
	int			i;
	int			total_flex;
	SDL_Rect	box_rect;

	i = 0;
	total_flex = 0;
	box_rect = (SDL_Rect){0};
	while (i < context->nb_box)
	{
		total_flex += context->boxs[i].flex;
		i++;
	}
	i = 0;
	box_rect.x = context->padding.left;
	box_rect.y = context->padding.top;
	while (i < context->nb_box)
	{
		if (context->flex_direction == HORIZONTAL)
		{
			box_rect.w = ((context->rect.w - context->padding.left - context->padding.right - (context->gap * (context->nb_box - 1))) * context->boxs[i].flex) / total_flex;
			box_rect.h = context->rect.h - context->padding.top - context->padding.bottom;
			context->boxs[i].rect = box_rect;
			box_rect.x += box_rect.w + context->gap;
		}
		else
		{
			box_rect.w = context->rect.w - context->padding.right - context->padding.left;
			box_rect.h = ((context->rect.h - context->padding.bottom - context->padding.top - (context->gap * (context->nb_box - 1))) * context->boxs[i].flex) / total_flex ;
			context->boxs[i].rect = box_rect;
			box_rect.y += box_rect.h + context->gap;
		}
		i++;
	}
}

void	ui_place_widgets(t_box *box)
{
	int	i;
	SDL_Rect	total_widget_rect;
	SDL_Rect	current_rect;
	SDL_Rect	avalaible_area;

	i = 0;
	total_widget_rect = (SDL_Rect){0};
	current_rect = (SDL_Rect){0};
	avalaible_area = (SDL_Rect){0};
	
	avalaible_area.w = box->rect.w - box->padding.left - box->padding.right - ((box->nb_widget - 1) * box->gap) + 1;
	avalaible_area.h = box->rect.h - box->padding.top - box->padding.bottom;
	if (avalaible_area.w <= 0 || avalaible_area.h <= 0 || box->nb_widget == 0)
		return ;
	current_rect.w = avalaible_area.w / box->nb_widget;
	current_rect.h = avalaible_area.h;
	current_rect.x = box->padding.left;
	current_rect.y = box->padding.top;
	while (i < box->nb_widget)
	{
		box->widgets[i].relative = current_rect;
		total_widget_rect.w += current_rect.w;
		if (total_widget_rect.h < current_rect.h)
			total_widget_rect.h = current_rect.h;
		current_rect.x += current_rect.w + box->gap;
		i++;
	}
	// box->rect.h = total_widget_rect.h;
	box->max_scroll.x = box->rect.w - total_widget_rect.w;
	if (box->max_scroll.x > 0)
		box->max_scroll.x = 0;
	box->max_scroll.y = box->rect.h - total_widget_rect.h;
	if (box->max_scroll.y > 0)
		box->max_scroll.y = 0;
}

void	ui_build_widgets(t_box *box)
{
	int	i;

	i = 0;
	while (i < box->nb_widget)
	{
		box->widgets[i].build(&box->widgets[i]);
		i++;
	}
}


void	ui_build_boxs(t_context *context)
{
	int	i;
	
	i = 0;
	while (i < context->nb_box)
	{
		ui_place_widgets(&context->boxs[i]);
		ui_build_widgets(&context->boxs[i]);
		ui_build_box_texture(&context->boxs[i]);
		i++;
	}
	
}

void	ui_build_windows(t_core *core)
{
	int	i;

	i = 0;
	while (i < core->nb_contexts)
	{
		SDL_GetWindowSize(core->contexts[i].window, &core->contexts[i].rect.w, &core->contexts[i].rect.h);
		ui_place_boxs(&core->contexts[i]);
		ui_build_boxs(&core->contexts[i]);
		core->contexts[i].is_durty = true;
		i++;
	}
}

void	ui_build_everything(t_core *core)
{
	ui_place_windows(core);
	ui_build_windows(core);
}
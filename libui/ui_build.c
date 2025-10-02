/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_build.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:19:36 by bvaujour          #+#    #+#             */
/*   Updated: 2025/10/02 17:04:03 by bvaujour         ###   ########.fr       */
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
	SDL_GetWindowSize(context->window, &context->rect.w, &context->rect.h);
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

// void	ui_build_widgets(t_context *context)
// {
// 	int	i;
	
// 	i = 0;
// 	while (i < context->nb_box)
// 	{
// 		ui_place_widgets(&context->boxs[i]);
// 		i++;
// 	}
// }

void	ui_build_everything(t_core *core)
{
	int	i;

	ui_place_windows(core);
	i = 0;
	while (i < core->nb_contexts)
	{
		ui_place_boxs(&core->contexts[i]);
		i++;
	}
}
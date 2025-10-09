/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_build.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:19:36 by bvaujour          #+#    #+#             */
/*   Updated: 2025/10/09 16:08:43 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_place_windows(t_core *core)
{
	int			i;
	SDL_Rect	window_rect;


	i = 0;
	window_rect = (SDL_Rect){0};
	while (i < core->nb_contexts)
	{
		SDL_SetWindowPosition(core->contexts[i].window, window_rect.x, window_rect.y);
		if (core->flex_direction == HORIZONTAL)
		{
			window_rect.w = (core->screen_w * core->contexts[i].flex) / core->total_contexts_flex;
			window_rect.h = core->screen_h;
			SDL_SetWindowSize(core->contexts[i].window, window_rect.w, window_rect.h);
			core->contexts[i].rect = window_rect;
			window_rect.x += window_rect.w;
		}
		else
		{
			window_rect.w = core->screen_w;
			window_rect.h = (core->screen_h * core->contexts[i].flex) / core->total_contexts_flex;
			SDL_SetWindowSize(core->contexts[i].window, window_rect.w, window_rect.h);
			core->contexts[i].rect = window_rect;
			window_rect.y += window_rect.h;
		}
		i++;
	}
}

void	ui_place_boxs_vertically(t_context *context)
{
	int			i;
	SDL_Rect	box_rect;
	SDL_Rect	avalaible_area;

	i = 0;
	box_rect = (SDL_Rect){0};
	avalaible_area = (SDL_Rect){0};
	avalaible_area.w = context->rect.w - context->padding.left - context->padding.right;
	avalaible_area.h = context->rect.h - context->padding.top - context->padding.bottom - context->gap * (context->nb_box - 1);
	box_rect.x = context->padding.left;
	box_rect.y = context->padding.top;
	while (i < context->nb_box)
	{
		box_rect.w = avalaible_area.w;
		box_rect.h = (avalaible_area.h * context->boxs[i].flex) / context->total_boxs_flex;
		context->boxs[i].rect = box_rect;
		box_rect.y += box_rect.h + context->gap;
		i++;
	}
}

void	ui_place_boxs_horizontally(t_context *context)
{
	int			i;
	SDL_Rect	box_rect;
	SDL_Rect	avalaible_area;

	i = 0;
	box_rect = (SDL_Rect){0};
	avalaible_area = (SDL_Rect){0};
	avalaible_area.w = (context->rect.w - context->padding.left - context->padding.right - (context->gap * (context->nb_box - 1)));
	avalaible_area.h = context->rect.h - context->padding.top - context->padding.bottom;
	box_rect.x = context->padding.left;
	box_rect.y = context->padding.top;
	while (i < context->nb_box)
	{
		box_rect.w = (avalaible_area.w * context->boxs[i].flex) / context->total_boxs_flex;
		box_rect.h = avalaible_area.h;
		context->boxs[i].rect = box_rect;
		box_rect.x += box_rect.w + context->gap;
		i++;
	}
}

void	ui_build_boxs(t_context *context)
{
	int	i;
	
	i = 0;
	while (i < context->nb_box)
	{
		context->boxs[i].build(&context->boxs[i]);
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
		if (core->contexts[i].nb_box > 0)
		{
			if (core->contexts[i].flex_direction == HORIZONTAL)
				ui_place_boxs_horizontally(&core->contexts[i]);
			else if (core->contexts[i].flex_direction == VERTICAL)
				ui_place_boxs_vertically(&core->contexts[i]);
			ui_build_boxs(&core->contexts[i]);
		}
		i++;
	}
}

void	ui_build_everything(t_core *core)
{
	ui_place_windows(core);
	ui_build_windows(core);
}
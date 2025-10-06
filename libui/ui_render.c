/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:42:58 by injah             #+#    #+#             */
/*   Updated: 2025/10/04 12:22:37 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

// void	ui_render_button(t_widget *button)
// {
// 	t_button_data	*data;

// 	data = &button->button_data;
// 	SDL_RenderCopy(button->context->renderer, data->textures[data->state], NULL, &button->rect);
// }


// static void	ui_render_box_elements(t_box *box)
// {
// 	t_widget	*widget;
// 	int			i;

// 	i = 0;
// 	while (i < box->nb_widget)
// 	{
// 		widget = &box->widgets[i];
// 		widget->render(widget);
// 		i++;
// 	}
// }


// static void	ui_render_box(t_box *box)
// {
// 	SDL_RenderCopy(box->context->renderer, box->texture, NULL, &box->rect);
// }




static void	ui_render_context_elements(t_context *context)
{
	int			i;
	
	i = 0;
	while (i < context->nb_box)
	{
		SDL_RenderCopy(context->renderer, context->boxs[i].texture, NULL, &context->boxs[i].rect);
		SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(context->renderer, &context->boxs[i].rect);
		i++;
	}
}

static void	ui_render_context(t_context *context)
{
	SDL_Color	color;

	color = context->core->config.window_color;
	SDL_SetRenderDrawColor(context->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(context->renderer);
}

void	ui_render(t_core *core)
{
	int 	i;

	i = 0;
	while (i < core->nb_contexts)
	{
		if (core->contexts[i].is_durty)
		{
			ui_render_context(&core->contexts[i]);
			ui_render_context_elements(&core->contexts[i]);	
			SDL_RenderPresent(core->contexts[i].renderer);
			core->contexts[i].is_durty = false;
		}
		i++;
	}
}
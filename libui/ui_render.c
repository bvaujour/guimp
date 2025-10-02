/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:42:58 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 16:31:53 by bvaujour         ###   ########.fr       */
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
	t_box	*box;
	int			i;
	
	i = 0;
	while (i < context->nb_box)
	{
		box = &context->boxs[i];
		if (box->is_durty)
		{
			ui_build_box_texture(box);
			box->is_durty = false;
		}
		SDL_RenderCopy(context->renderer, box->texture, NULL, &box->rect);
		SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(context->renderer, &box->rect);
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
	t_context	*context;
	int 	i;

	i = 0;
	while (i < core->nb_contexts)
	{
		context = &core->contexts[i];
		if (context->is_durty)
		{
			ui_render_context(context);
			ui_render_context_elements(context);	
			SDL_RenderPresent(context->renderer);
			context->is_durty = false;
		}
		i++;
	}
}
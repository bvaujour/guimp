/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_render.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:42:58 by injah             #+#    #+#             */
/*   Updated: 2025/09/30 19:13:51 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

// void	ui_render_button(t_widget *button)
// {
// 	t_button_data	*data;

// 	data = &button->button_data;
// 	SDL_RenderCopy(button->ctx->renderer, data->textures[data->state], NULL, &button->rect);
// }


// static void	ui_render_container_elements(t_container *container)
// {
// 	t_widget	*widget;
// 	int			i;

// 	i = 0;
// 	while (i < container->nb_widget)
// 	{
// 		widget = &container->widgets[i];
// 		widget->render(widget);
// 		i++;
// 	}
// }


// static void	ui_render_container(t_container *container)
// {
// 	SDL_RenderCopy(container->ctx->renderer, container->texture, NULL, &container->rect);
// }




static void	ui_render_ctx_elements(t_ctx *ctx)
{
	t_container	*container;
	int			i;
	
	i = 0;
	while (i < ctx->nb_container)
	{
		container = &ctx->containers[i];
		if (container->is_durty)
		{
			ui_build_container(container);
			container->is_durty = false;
		}
		SDL_RenderCopy(ctx->renderer, container->texture, NULL, &container->rect);
		i++;
	}
}

static void	ui_render_ctx(t_ctx *ctx)
{
	SDL_Color	color;

	color = ctx->config->window_color;
	SDL_SetRenderDrawColor(ctx->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(ctx->renderer);
}

void	ui_render(t_core *core)
{
	t_ctx	*ctx;
	int 	i;

	i = 0;
	while (i < core->nb_ctxs)
	{
		ctx = &core->ctxs[i];
		if (ctx->is_durty)
		{
			ui_render_ctx(ctx);
			ui_render_ctx_elements(ctx);	
			SDL_RenderPresent(ctx->renderer);
			ctx->is_durty = false;
		}
		i++;
	}
}
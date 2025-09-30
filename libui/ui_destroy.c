/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:25:00 by injah             #+#    #+#             */
/*   Updated: 2025/09/29 10:05:22 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_destroy_button(t_widget *button)
{
	if (button->button_data.surfaces[DEFAULT])
		SDL_FreeSurface(button->button_data.surfaces[DEFAULT]);
	if (button->button_data.surfaces[HOVERED])
		SDL_FreeSurface(button->button_data.surfaces[HOVERED]);
	if (button->button_data.surfaces[CLICKED])
		SDL_FreeSurface(button->button_data.surfaces[CLICKED]);
}

static void	ui_destroy_container(t_container *container)
{
	t_widget	*widget;
	int			i;

	if (container->texture)
		SDL_DestroyTexture(container->texture);
	i = 0;
	while (i < container->nb_widget)
	{
		widget = &container->widgets[i];
		widget->destroy(widget);
		i++;
	}
}

static void	ui_destroy_ctx(t_ctx *ctx)
{
	t_container	*container;
	int	i;

	i = 0;
	while (i < ctx->nb_container)
	{
		container = &ctx->containers[i];
		ui_destroy_container(container);
		i++;
	}
	if (ctx->renderer)
		SDL_DestroyRenderer(ctx->renderer);
	if (ctx->window)
		SDL_DestroyWindow(ctx->window);
}

void	ui_destroy(t_core *core)
{
	t_ctx	*ctx;
	int		i;

	i = 0;
	while (i < core->nb_ctxs)
	{
		ctx = &core->ctxs[i];
		ui_destroy_ctx(ctx);
		i++;
	}
	TTF_CloseFont(core->config.font);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
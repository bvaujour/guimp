/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_destroy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:25:00 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 06:47:28 by injah            ###   ########.fr       */
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

static void	ui_destroy_box(t_box *box)
{
	t_widget	*widget;
	int			i;

	i = 0;
	while (i < box->nb_widget)
	{
		widget = &box->widgets[i];
		widget->destroy(widget);
		i++;
	}
	if (box->texture)
		SDL_DestroyTexture(box->texture);
}

static void	ui_destroy_context(t_context *context)
{
	t_box	*box;
	int	i;

	i = 0;
	while (i < context->nb_box)
	{
		box = &context->boxs[i];
		ui_destroy_box(box);
		i++;
	}
	if (context->renderer)
		SDL_DestroyRenderer(context->renderer);
	if (context->window)
		SDL_DestroyWindow(context->window);
}

void	ui_destroy(t_core *core)
{
	t_context	*context;
	int		i;

	i = 0;
	while (i < core->nb_contexts)
	{
		context = &core->contexts[i];
		ui_destroy_context(context);
		i++;
	}
	TTF_CloseFont(core->config.font);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
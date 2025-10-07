/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:41:16 by injah             #+#    #+#             */
/*   Updated: 2025/10/07 15:54:50 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_change_button_state(t_widget *button, e_button_state state)
{
	if (button->button_data.state != state)
	{
		button->button_data.state = state;
		ui_refresh_button_surface(button);
	}
}

void	ui_update_button(t_widget *button)
{
	SDL_Rect	absolute;

	absolute.x = button->box->rect.x + button->relative.x;
	absolute.y = button->box->rect.y + button->relative.y;
	absolute.w = button->relative.w;
	absolute.h = button->relative.h;
	if (SDL_PointInRect(&button->core->mouse, &absolute) && SDL_PointInRect(&button->core->mouse, &button->box->rect))
	{
		if (button->core->left_click)
		{
			ui_change_button_state(button, CLICKED);
			if (button->button_data.on_click)
				button->button_data.on_click(button->button_data.param);
		}
		else
			ui_change_button_state(button, HOVERED);
	}
	else
		ui_change_button_state(button, DEFAULT);
}

void	ui_update_box(t_box *box)
{
	int			i;

	i = 0;
	if (box->core->scrolled && SDL_PointInRect(&box->core->mouse, &box->rect))
		ui_box_consume_scroll(box);
	while (i < box->nb_widget)
	{
		box->widgets[i].update(&box->widgets[i]);
		i++;
	}
	ui_build_box_texture(box);
}

void	ui_update_context(t_context *context)
{
	int			i;

	i = 0;
	while (i < context->nb_box)
	{
		ui_update_box(&context->boxs[i]);
		i++;
	}
}

void	ui_update(t_core *core)
{
	int	i;

	i = 0;
	while (i < core->nb_contexts)
	{
		if (core->focused_window_id == SDL_GetWindowID(core->contexts[i].window))
		{
			ui_update_context(&core->contexts[i]);
			break ;
		}
		i++;
	}
}
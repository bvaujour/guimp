/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:41:16 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 06:18:52 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_change_button_state(t_widget *button, t_button_state state)
{
	if (button->button_data.state != state)
	{
		button->button_data.state = state;
		button->surface = button->button_data.surfaces[state];
		button->box->is_durty = true;
		button->context->is_durty = true;
	}
}

void	ui_update_button(t_widget *button)
{
	if (SDL_PointInRect(&button->core->mouse, &button->rect) && SDL_PointInRect(&button->core->mouse, &button->box->rect))
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

void	ui_update_widget_position(t_box *box, t_widget *widget)
{
	widget->rect.x = widget->relative.x + box->rect.x + box->scroll.x;
	widget->rect.y = widget->relative.y + box->rect.y + box->scroll.y;
}

void	ui_update_box(t_box *box)
{
	t_widget	*widget;
	int			i;

	i = 0;
	if (box->core->scrolled && SDL_PointInRect(&box->core->mouse, &box->rect))
	{
		ui_box_consume_scroll(box);
		box->is_durty = true;
		box->context->is_durty = true;
	}
	while (i < box->nb_widget)
	{
		widget = &box->widgets[i];
		ui_update_widget_position(box, widget);
		widget->update(widget);
		i++;
	}
}

void	ui_update_context(t_context *context)
{
	t_box	*box;
	int			i;

	i = 0;
	while (i < context->nb_box)
	{
		box = &context->boxs[i];
		ui_update_box(box);
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
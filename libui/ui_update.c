/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_update.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:41:16 by injah             #+#    #+#             */
/*   Updated: 2025/09/30 09:58:16 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_change_button_state(t_widget *button, t_button_state state)
{
	if (button->button_data.state != state)
	{
		button->button_data.state = state;
		button->surface = button->button_data.surfaces[state];
		button->container->is_durty = true;
		button->ctx->is_durty = true;
	}
}

void	ui_update_button(t_widget *button)
{
	if (SDL_PointInRect(&button->core->mouse, &button->rect) && SDL_PointInRect(&button->core->mouse, &button->container->rect))
	{
		button->core->focused_widget = button;
		if (button->core->left_click)
		{
			ui_change_button_state(button, CLICKED);
			if (button->button_data.binded_function)
				button->button_data.binded_function(button->button_data.param, 3, 4);
		}
		else
			ui_change_button_state(button, HOVERED);
	}
	else
		ui_change_button_state(button, DEFAULT);
}

void	ui_container_consume_scroll(t_container *container)
{
	container->wheel_offset.x += container->core->wheel.x * SCROLL_SPEED;
	container->wheel_offset.y += container->core->wheel.y * SCROLL_SPEED;
	printf("container->core->wheel.x: %d\n", container->core->wheel.x);
	printf("container->wheel_offset.x: %d\n", container->wheel_offset.x);
	printf("container->max_scroll.x: %d\n", container->max_scroll.x);
	if (container->wheel_offset.x > 0)
		container->wheel_offset.x = 0;
	else if (container->wheel_offset.x < -container->max_scroll.x)
		container->wheel_offset.x = -container->max_scroll.x;
	if (container->wheel_offset.y > 0)
		container->wheel_offset.y = 0;
	else if (container->wheel_offset.y < -container->max_scroll.y)
		container->wheel_offset.y = -container->max_scroll.y;
	container->core->wheel.x = 0;
	container->core->wheel.y = 0;
}

void	ui_update_container(t_container *container)
{
	t_widget	*widget;
	int			i;

	i = 0;
	if (SDL_PointInRect(&container->core->mouse, &container->rect))
	{
		container->core->focused_container = container;
		ui_container_consume_scroll(container);
	}
	container->core->focused_widget = NULL;
	while (i < container->nb_widget)
	{
		widget = &container->widgets[i];
		widget->update(widget);
		i++;
	}
}

void	ui_update_ctx(t_ctx *ctx)
{
	t_container	*container;
	int			i;

	i = 0;
	ctx->core->focused_container = NULL;
	while (i < ctx->nb_container)
	{
		container = &ctx->containers[i];
		ui_update_container(container);
		i++;
	}
}

void	ui_update(t_core *core)
{
	int	i;

	i = 0;
	while (i < core->nb_ctxs)
	{
		if (core->focused_window_id == SDL_GetWindowID(core->ctxs[i].window))
		{
			ui_update_ctx(&core->ctxs[i]);
			break ;
		}
		i++;
	}
}
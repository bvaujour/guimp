/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_button.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:36:13 by injah             #+#    #+#             */
/*   Updated: 2026/05/05 16:14:48 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

static void	ui_button_destroy(t_widget *button)
{
	(void)button;
}

static void	ui_button_event(t_widget *button)
{
	SDL_Point	absolute;

	if (button->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		button->core->focused_widget = button;
		absolute = ui_get_absolute_position(button);
		ui_widget_call_onclicked(button->core, button, button->core->mouse.position.x - absolute.x, button->core->mouse.position.y - absolute.y, button->core->event.button.button);
	}
	else if (button->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(button);
		ui_widget_call_onreleased(button->core, button, button->core->mouse.position.x - absolute.x, button->core->mouse.position.y - absolute.y, button->core->event.button.button);
	}
	
}

static void	ui_button_update(t_widget *button)
{
	SDL_Point	absolute;

	if (button->core->event.type == SDL_KEYDOWN)
	{
		if (button->core->event.key.keysym.scancode == SDL_SCANCODE_SPACE)
		{
			absolute = ui_get_absolute_position(button);
			ui_widget_call_onclicked(button->core, button, button->core->mouse.position.x - absolute.x, button->core->mouse.position.y - absolute.y, 1);
		}
	}
}

// static void	ui_button_onunhovered(t_widget *button)
// {
// 	ui_widget_change_state(button, NORMAL);
// }

bool	ui_button_get_onoff_state(t_widget *button)
{
	if (button->type != ONOFF_BUTTON)
		return (false);
	return (button->is_on);
}

t_widget	*ui_create_button(t_widget *parent, t_rect	relative_rect)
{
	t_widget			*button;
	t_button_data		*data;

	if (parent == NULL)
		return (NULL);
	button = ui_new_widget(parent, (SDL_Rect){relative_rect.x, relative_rect.y, relative_rect.width, relative_rect.height}, BUTTON, UI_MAX_BUTTON_CHILDS);
	if (!button)
		return (ui_destroy_widget(button), NULL);
	button->data = malloc(sizeof(t_button_data));
	if (!button->data)
		return (ui_destroy_widget(button), NULL);
	data = (t_button_data *)button->data;
	*data = (t_button_data){0};
	ft_strcpy(button->name, "button");
	ui_set_widget_colors(button, 0xFF6F6F6F, 0xFF7F7F7F, 0xFF8F8F8F);
	button->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	button->render = ui_widget_basic_render;
	button->destroy = ui_button_destroy;
	button->event = ui_button_event;
	button->update = ui_button_update;
	button->texture = ui_new_texture(parent->renderer, 32, 32, button->colors[button->state]);
	if (ui_add_child(parent, button) != UI_SUCCESS)
		return (ui_destroy_widget(button), NULL);
	return (button);
}

t_widget	*ui_create_onoff_button(t_widget *parent, t_rect relative_rect)
{
	t_widget			*button;
	t_button_data		*data;

	if (parent == NULL)
		return (NULL);
	button = ui_new_widget(parent, (SDL_Rect){relative_rect.x, relative_rect.y, relative_rect.width, relative_rect.height}, ONOFF_BUTTON, UI_MAX_BUTTON_CHILDS);
	if (!button)
		return (ui_destroy_widget(button), NULL);
	button->data = malloc(sizeof(t_button_data));
	if (!button->data)
		return (ui_destroy_widget(button), NULL);
	data = (t_button_data *)button->data;
	*data = (t_button_data){0};
	ft_strcpy(button->name, "onoff_button");
	ui_set_widget_colors(button, 0xFF6F6F6F, 0xFF7F7F7F, 0xFF8F8F8F);
	button->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	button->render = ui_widget_basic_render;
	button->destroy = ui_button_destroy;
	button->event = ui_button_event;
	button->texture = ui_new_texture(parent->renderer, 32, 32, button->colors[button->state]);
	button->is_on = false;
	if (ui_add_child(parent, button) != UI_SUCCESS)
		return (ui_destroy_widget(button), NULL);
	return (button);
}
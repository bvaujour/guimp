/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_slider.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:33:33 by injah             #+#    #+#             */
/*   Updated: 2026/05/05 15:11:18 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

void	ui_slider_render(t_widget *slider)
{
	t_slider_data	*data;
	SDL_Point		absolute;

	data = (t_slider_data *)slider->data;
	absolute = ui_get_absolute_position(slider);

	SDL_RenderCopy(slider->renderer, slider->texture, NULL, &(SDL_Rect){absolute.x, absolute.y, slider->width, slider->height}); // le slider background
	SDL_SetRenderDrawColor(slider->renderer, 10, 10, 10, 255);
	SDL_RenderFillRect(slider->renderer, &(SDL_Rect){absolute.x + data->inner.x, absolute.y + data->inner.y, data->inner.w, data->inner.h}); // la partie du slider vide
	SDL_SetRenderDrawColor(slider->renderer, data->fill_color.r, data->fill_color.g, data->fill_color.b, data->fill_color.a);
	SDL_RenderFillRect(slider->renderer, &(SDL_Rect){absolute.x + data->inner.x, absolute.y+ data->inner.y, data->inner.w * data->value, data->inner.h}); // la partie du slider remplie
}

void	ui_slider_destroy(t_widget *slider)
{
	(void)slider;
}

void	ui_slider_update(t_widget *slider)
{	
	t_slider_data	*data;

	if (slider->core->event.type == SDL_KEYDOWN)
	{
		if (slider->core->event.key.keysym.scancode == SDL_SCANCODE_LEFT)
		{
			data = (t_slider_data *)slider->data;
			data->value -= 0.01f;
			if (data->value <= 0.0f)
				data->value = 0.0f;
			if (slider->core->onsilderslided)
				slider->core->onsilderslided(slider, data->value, slider->core->onsilderslided_param);
			ui_window_set_is_dirty(slider->owning_window, true);
		}
		else if (slider->core->event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			data = (t_slider_data *)slider->data;
			data->value += 0.01f;
			if (data->value >= 1.0f)
				data->value = 1.0f;
			if (slider->core->onsilderslided)
				slider->core->onsilderslided(slider, data->value, slider->core->onsilderslided_param);
			ui_window_set_is_dirty(slider->owning_window, true);
		}
	}
}

void	ui_set_slider_value(t_widget *slider, float value)
{
	t_slider_data	*data;

	if (slider->type != SLIDER)
		return ;
	data = (t_slider_data *)slider->data;
	if (value < 0.0f)
		data->value = 0.0f;
	else if (value > 1.0f)
		data->value = 1.0f;
	else
		data->value = value;
	ui_window_set_is_dirty(slider->owning_window, true);
}

static void	ui_slider_event(t_widget *slider)
{
	t_slider_data	*data;
	SDL_Point		click_relative;
	SDL_Point		absolute;


	if (slider->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		slider->core->focused_widget = slider;
		absolute = ui_get_absolute_position(slider);
		ui_widget_call_onclicked(slider->core, slider, slider->core->mouse.position.x - absolute.x, slider->core->mouse.position.y - absolute.y, slider->core->event.button.button);
	}
	else if (slider->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(slider);
		ui_widget_call_onreleased(slider->core, slider, slider->core->mouse.position.x - absolute.x, slider->core->mouse.position.y - absolute.y, slider->core->event.button.button);
	}

	if (slider->core->mouse.buttons[SDL_BUTTON_LEFT])
	{
		slider->core->focused_widget = slider;
		absolute = ui_get_absolute_position(slider);
		data = (t_slider_data *)slider->data;
		click_relative.x = slider->core->mouse.position.x - (absolute.x + data->inner.x);
		data->value = (float)click_relative.x / (data->inner.w - 1);
		if (data->value < 0.0f)
			data->value = 0.0f;
		else if (data->value > 1.0f)
			data->value = 1.0f;
		if (slider->core->onsilderslided)
			slider->core->onsilderslided(slider, data->value, slider->core->onsilderslided_param);
		ui_window_set_is_dirty(slider->owning_window, true);
	}
}

void	ui_slider_set_fill_color(t_widget *slider, unsigned int color)
{
	t_slider_data		*data;

	if (slider->type != SLIDER)
		return ;
	data = (t_slider_data *)slider->data;
	data->fill_color = ui_unpack_color(color);
}

void	ui_slider_build(t_widget *slider)
{
	t_slider_data		*data;

	data = (t_slider_data *)slider->data;
	data->inner.w = 3 * slider->width / 4;
	data->inner.h = slider->height / 3;
	data->inner.x = slider->width / 2 - data->inner.w / 2;
	data->inner.y = slider->height / 2 - data->inner.h / 2;
}

t_widget	*ui_create_slider(t_widget *parent, t_rect relative_rect)
{
	t_widget			*slider;
	t_slider_data		*data;

	if (parent == NULL)
		return (NULL);
	slider = ui_new_widget(parent, (SDL_Rect){relative_rect.x, relative_rect.y, relative_rect.width, relative_rect.height}, SLIDER, UI_MAX_SLIDER_CHILDS);
	if (slider == NULL)
		return (ui_destroy_widget(slider), NULL);
	slider->data = malloc(sizeof(t_slider_data));
	if (!slider->data)
		return (ui_destroy_widget(slider), NULL);
	data = (t_slider_data *)slider->data;
	*data = (t_slider_data){0};
	slider->outline = 0;
	data->value = 0.5f;
	ft_strcpy(slider->name, "slider");
	ui_set_widget_colors(slider, 0x00000000, 0x00000000, 0x00000000);
	data->fill_color = (SDL_Color){255, 255, 255, 255};
	slider->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	slider->render = ui_slider_render;
	slider->update = ui_slider_update;
	slider->build = ui_slider_build;
	slider->event = ui_slider_event;
	slider->destroy = ui_slider_destroy;
	slider->texture = ui_new_texture(slider->renderer, 32, 32, slider->colors[slider->state]);
	if (ui_add_child(parent, slider) != UI_SUCCESS)
		return (ui_destroy_widget(slider), NULL);
	return (slider);
}
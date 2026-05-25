/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_text.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 17:04:56 by bvaujour          #+#    #+#             */
/*   Updated: 2026/04/29 14:29:18 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

static void	ui_text_render(t_widget *text)
{
	SDL_Point		absolute;

	absolute = ui_get_absolute_position(text);
	SDL_RenderCopy(text->renderer, text->texture, NULL, &(SDL_Rect){absolute.x, absolute.y, text->width, text->height});
}

void	ui_set_text_str(t_widget *text, char *str)
{
	t_text_data	*data;

	data = (t_text_data *)text->data;
	ft_strcpy(data->str, str);
	data->str_is_dirty = true;
	text->build(text);
}

void	ui_set_text_str_by_int(t_widget *text, int nbr)
{
	char		*str;
	t_text_data	*data;

	data = (t_text_data *)text->data;
	str = ft_itoa(nbr);
	ft_strcpy(data->str, str);
	free(str);
	data->str_is_dirty = true;
	text->build(text);
}

void	ui_set_text_str_by_float(t_widget *text, float nbr, int precision)
{
	char		*str;
	t_text_data	*data;

	data = (t_text_data *)text->data;
	str = ft_ftoa(nbr, precision);
	ft_strcpy(data->str, str);
	free(str);
	data->str_is_dirty = true;
	text->build(text);
}

void	ui_set_text_font_color(t_widget *text, unsigned int font_color)
{
	SDL_Color	color;

	color = ui_unpack_color(font_color);
	text->colors[NORMAL] = color;
}

void	ui_text_event(t_widget *text)
{
	SDL_Point	absolute;
	text->core->focused_widget = text;
	if (text->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		absolute = ui_get_absolute_position(text);
		ui_widget_call_onclicked(text->core, text, text->core->mouse.position.x - absolute.x, text->core->mouse.position.y - absolute.y, text->core->event.button.button);
	}
	else if (text->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(text);
		ui_widget_call_onreleased(text->core, text, text->core->mouse.position.x - absolute.x, text->core->mouse.position.y - absolute.y, text->core->event.button.button);
	}
}

static void	ui_text_build(t_widget *text)
{
	t_text_data	*data;

	data = (t_text_data *)text->data;
	if (data->str_is_dirty)
	{
		if (text->texture)
			SDL_DestroyTexture(text->texture);
		if (data->mode == DEFAULT_TEXT || data->mode == CENTERED)
			text->texture = ui_create_str_texture(text->renderer, data->str, text->core->font, data->font_size, text->parent->width);
		else
			text->texture = ui_create_str_texture(text->renderer, data->str, text->core->font, text->parent->height / 2, 0);
		data->str_is_dirty = false;
		SDL_SetTextureColorMod(text->texture, text->colors[NORMAL].r, text->colors[NORMAL].g, text->colors[NORMAL].b);
		SDL_SetTextureAlphaMod(text->texture, text->colors[NORMAL].a);
	}
	SDL_QueryTexture(text->texture, NULL, NULL, &text->width, &text->height);
	if (data->mode == SINGLE_LINE)
	{
		text->position.y = text->parent->height / 2 - text->height / 2;
		if (text->width > text->parent->width)
			text->position.x = text->parent->width - text->width;
		else
			text->position.x = 5;
	}
	if (data->mode == CENTERED)
	{
		text->position.x = text->parent->width / 2 - text->width / 2;
		text->position.y = text->parent->height / 2 - text->height / 2;
	}
	else if (data->mode == FIT)
	{
		if (text->width > text->parent->width)
			text->width = text->parent->width;
		text->position.x = text->parent->width / 2 - text->width / 2;
		text->position.y = text->parent->height / 2 - text->height / 2;
	}
}

t_widget	*ui_create_text(t_widget *parent, int x, int y, const char *str, e_text_mode mode, int font_size)
{
	t_widget			*text;
	t_text_data	*data;

	if (parent == NULL)
		return (NULL);
	text = ui_new_widget(parent, (SDL_Rect){x, y, 100, 100}, TEXT, UI_MAX_TEXT_CHILDS);
	if (!text)
		return (ui_destroy_widget(text), NULL);
	text->data = malloc(sizeof(t_text_data));
	if (!text->data)
		return (ui_destroy_widget(text), NULL);
	data = (t_text_data *)text->data;
	*data = (t_text_data){0};
	text->hit_testable = false;
	ft_strcpy(text->name, "text");
	ft_strcpy(data->str, str);
	data->str_is_dirty = true;
	data->font_size = font_size;
	ui_set_widget_colors(text, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	text->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	text->render = ui_widget_basic_render;
	text->build = ui_text_build;
	text->render = ui_text_render;
	text->event = ui_text_event;
	data->mode = mode;
	ui_text_build(text);
	data->str_is_dirty = true;
	if (ui_add_child(parent, text) != UI_SUCCESS)
		return (ui_destroy_widget(text), NULL);
	return (text);
}
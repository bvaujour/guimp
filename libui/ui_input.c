/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 17:04:56 by bvaujour          #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

static void		ui_input_render(t_widget *input)
{
	SDL_Point		absolute;
	t_input_data	*data;
	data = (t_input_data *)input->data;

	absolute = ui_get_absolute_position(input);
	if (input == input->core->focused_widget)
		SDL_SetTextureColorMod(input->texture, input->colors[CLICKED].r, input->colors[CLICKED].g, input->colors[CLICKED].b);
	else
		SDL_SetTextureColorMod(input->texture, input->colors[NORMAL].r, input->colors[NORMAL].g, input->colors[NORMAL].b);
	SDL_RenderCopy(input->renderer, input->texture, NULL, &(SDL_Rect){absolute.x, absolute.y, input->width, input->height});
	SDL_RenderCopy(input->renderer, data->str_texture, NULL, &(SDL_Rect){absolute.x + data->str_rect.x, absolute.y + data->str_rect.y, data->str_rect.w, data->str_rect.h});
	if (input == input->core->focused_widget)
	{
		SDL_SetRenderDrawColor(input->renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(input->renderer, &(SDL_Rect){absolute.x + data->str_rect.x + data->cursor_pos, absolute.y, 5, input->height});
	}
}

static void	ui_create_input_str(t_widget *input)
{
	t_input_data	*data;
	SDL_Surface		*str_surface;

	data = (t_input_data *)input->data;
	if (ft_strlen(data->str) == 0)
	{
		data->str_rect.w = 0;
		data->cursor_pos = 0;
		ui_window_set_is_dirty(input->owning_window, true);
		if(input->core->oninputchanged)
			input->core->oninputchanged(input, data->str, input->core->oninputchanged_param);
		return ;
	}
	TTF_SetFontSize(input->core->font, data->font_size);
	if (!input->core->font)
	{
		ft_printf("input: font not open\n");
		return ;
	}
	str_surface = TTF_RenderText_Blended(input->core->font, data->str, data->str_color);
	if (!str_surface)
	{
		ft_printf("str_surface invalid\n");
		return ;
	}
	if (data->str_texture)
		SDL_DestroyTexture(data->str_texture);
	data->str_texture = SDL_CreateTextureFromSurface(input->renderer, str_surface);
	data->str_rect.w = str_surface->w;
	data->str_rect.h = str_surface->h;
	SDL_FreeSurface(str_surface);
	input->build(input);
	if(input->core->oninputchanged)
		input->core->oninputchanged(input, data->str, input->core->oninputchanged_param);
}


static void		ui_input_update(t_widget *input)
{	
	t_input_data			*data;
	
	data = (t_input_data *)input->data;
	if (input == input->core->focused_widget)
	{
		SDL_StartTextInput();
		if (input->core->event.type == SDL_TEXTINPUT)
		{
			if (ft_strlen(input->core->event.text.text) + ft_strlen(data->str) < UI_MAX_TEXT_SIZE)
			{
				ft_insert(data->str, input->core->event.text.text, data->str_index, UI_MAX_TEXT_SIZE);
				data->str_index += ft_strlen(input->core->event.text.text);
				ui_create_input_str(input);
			}
		}
		if (input->core->event.type == SDL_KEYDOWN)
		{
			if (input->core->event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && data->str_index > 0)
			{
				data->str_index--;
				data->str[data->str_index] = '\0';
				ui_create_input_str(input);
			}
			if (input->core->event.key.keysym.scancode == SDL_SCANCODE_LEFT && data->str_index > 0)
			{
				data->str_index--;
				input->build(input);
			}
			if (input->core->event.key.keysym.scancode == SDL_SCANCODE_RIGHT && data->str_index < ft_strlen(data->str))
			{
				data->str_index++;
				input->build(input);
			}
			if (input->core->keys[SDL_SCANCODE_LCTRL] && input->core->event.key.keysym.scancode == SDL_SCANCODE_V)
			{
				char *text = SDL_GetClipboardText();
				if (text)
				{
					ft_insert(data->str, text, data->str_index, UI_MAX_TEXT_SIZE);
					data->str_index += ft_strlen(text);
					ui_create_input_str(input);
				}
			}
			if (input->core->event.key.keysym.scancode == SDL_SCANCODE_RETURN || input->core->event.key.keysym.sym == SDL_SCANCODE_KP_ENTER)
			{
				if (input->core->keys[SDL_SCANCODE_RSHIFT])
				{
					data->str[data->str_index] = '\n';
					data->str_index++;
					ui_create_input_str(input);
				}
				else
					if (input->core->oninputvalidated)
						input->core->oninputvalidated(input, data->str, input->core->oninputvalidated_param);
			}
		}
	}
	else
		SDL_StopTextInput();
}

static void	ui_input_destroy(t_widget *input)
{
	(void)input;
}

char	*ui_input_get_text(t_widget *input)
{
	t_input_data	*data;

	data = (t_input_data *)input->data;
	return (data->str);
}

void	ui_input_set_text(t_widget *input, const char *str)
{
	t_input_data	*data;

	if (input == NULL || input->type != INPUT)
		return ;
	data = (t_input_data *)input->data;
	ft_strcpy(data->str, str);
	data->str_index = ft_strlen(data->str);
	ui_create_input_str(input);
}

static void	ui_input_build(t_widget *input)
{
	t_input_data	*data;
	char			*str;

	data = (t_input_data *)input->data;
	if (data->str_rect.w > input->width - 5)
		data->str_rect.x = input->width - data->str_rect.w - 5;
	else
		data->str_rect.x = 0;
	data->str_rect.y = input->height / 2 - data->str_rect.h / 2;
	str = ft_strdup(data->str);
	str[data->str_index] = '\0';
	TTF_SizeText(input->core->font, str, &data->cursor_pos, NULL);
	if (data->str_rect.x + data->cursor_pos < 0)
	{
		int	delta = data->str_rect.x + data->cursor_pos;
		data->str_rect.x -= delta;
	}
	free(str);
	ui_window_set_is_dirty(input->owning_window, true);
	// ft_printf("build success\n");
}

static void	ui_input_event(t_widget *input)
{
	SDL_Point	absolute;

	if (input->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (input != input->core->focused_widget)
		{
			input->core->focused_widget = input;
			ui_window_set_is_dirty(input->owning_window, true);
		}
		absolute = ui_get_absolute_position(input);
		ui_widget_call_onclicked(input->core, input, input->core->mouse.position.x - absolute.x, input->core->mouse.position.y - absolute.y, input->core->event.button.button);
	}
	else if (input->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(input);
		ui_widget_call_onreleased(input->core, input, input->core->mouse.position.x - absolute.x, input->core->mouse.position.y - absolute.y, input->core->event.button.button);
	}
}




t_widget	*ui_create_input(t_widget *parent, t_rect relative_rect, int font_size)
{
	t_widget			*input;
	t_input_data		*data;

	if (parent == NULL)
		return (NULL);
	input = ui_new_widget(parent, (SDL_Rect){relative_rect.x, relative_rect.y, relative_rect.width, relative_rect.height}, INPUT, UI_MAX_INPUT_CHILDS);
	if (!input)
		return (NULL);
	input->data = malloc(sizeof(t_input_data));
	if (!input->data)
		return (ui_destroy_widget(input), NULL);
	data = (t_input_data *)input->data;
	*data = (t_input_data){0};
	ft_strcpy(input->name, "input");
	data->str_color = (SDL_Color){255, 255, 255, 255};
	data->font_size = font_size;
	data->str[0] = '\0';
	ui_set_widget_colors(input, 0xFF111111, 0xFF111111, 0xFF000000);
	input->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	input->render = ui_input_render;
	input->destroy = ui_input_destroy;
	input->build = ui_input_build;
	input->event = ui_input_event;
	input->update = ui_input_update;
	input->texture = ui_new_texture(parent->renderer, 32, 32, input->colors[input->state]);
	if (ui_add_child(parent, input) != UI_SUCCESS)
		return (ui_destroy_widget(input), NULL);
	return (input);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_window.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 12:53:46 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

static void		ui_window_render(t_widget *window)
{
	SDL_RenderCopy(window->renderer, window->texture, NULL, NULL);
}

static void	ui_window_destroy(t_widget *window)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	if (data->custom_cursor.texture)
		SDL_DestroyTexture(data->custom_cursor.texture);
	if (window->renderer)
		SDL_DestroyRenderer(window->renderer);
	if (data->window)
		SDL_DestroyWindow(data->window);
	if (data->surface)
		SDL_FreeSurface(data->surface);
}

void	ui_window_set_is_dirty(t_widget *window, bool new_dirty)
{
	t_window_data	*data;

	data = window->data;
	data->is_durty = new_dirty;
}

bool	ui_window_get_is_dirty(t_widget *window)
{
	t_window_data	*data;

	data = window->data;
	return (data->is_durty);
}

void	ui_window_event(t_widget *window)
{
	SDL_Point	absolute;

	if (window->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		absolute = ui_get_absolute_position(window);
		ui_widget_call_onclicked(window->core, window, window->core->mouse.position.x - absolute.x, window->core->mouse.position.y - absolute.y, window->core->event.button.button);
	}
	else if (window->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(window);
		ui_widget_call_onreleased(window->core, window, window->core->mouse.position.x - absolute.x, window->core->mouse.position.y - absolute.y, window->core->event.button.button);
	}
}

void	ui_window_selection_line(t_widget *window)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	if (data->is_selectionning_line == false)
		return ;
	data->selection.x = window->core->mouse.position.x;
	data->selection.y = window->core->mouse.position.y;
	ui_window_set_is_dirty(window, true);
}

void	ui_window_selection_rect(t_widget *window)
{
	t_window_data	*data;
	SDL_Point		absolute;

	data = (t_window_data *)window->data;
	if (data->is_selectionning == false)
		return ;
	if (data->selection_start.x < window->core->mouse.position.x)
		data->selection.x = data->selection_start.x;
	else
		data->selection.x = window->core->mouse.position.x;
	if (data->selection_start.y < window->core->mouse.position.y)
		data->selection.y = data->selection_start.y;
	else
		data->selection.y = window->core->mouse.position.y;
	data->selection.w = abs(data->selection_start.x - window->core->mouse.position.x);
	data->selection.h = abs(data->selection_start.y - window->core->mouse.position.y);
	if (data->on_widget != NULL)
	{
		absolute = ui_get_absolute_position(data->on_widget);
		if (data->selection.x < absolute.x)
			data->selection.x = absolute.x;
		else if (data->selection.x + data->selection.w > absolute.x + data->on_widget->width)
			data->selection.w = absolute.x + data->on_widget->width - data->selection.x;
		if (data->selection.y < absolute.y)
			data->selection.y = absolute.y;
		else if (data->selection.y + data->selection.h > absolute.y + data->on_widget->height)
			data->selection.h = absolute.y + data->on_widget->height - data->selection.y;
	}
	ui_window_set_is_dirty(window, true);
}

void	ui_window_selection_circle(t_widget *window)
{
	t_window_data	*data;
	// SDL_Point		absolute;
	int				center_x;
	int				center_y;
	int				delta_x;
	int				delta_y;

	data = (t_window_data *)window->data;
	if (data->is_selectionning_circle == false)
		return ;
	center_x = (data->selection_start.x + window->core->mouse.position.x) / 2;
	center_y = (data->selection_start.y + window->core->mouse.position.y) / 2;
	delta_x = abs(window->core->mouse.position.x - data->selection_start.x);
	delta_y = abs(window->core->mouse.position.y - data->selection_start.y);
	data->selection.x = center_x;
	data->selection.y = center_y;
	data->selection.w = delta_x;
	data->selection.h = delta_y;
	ui_window_set_is_dirty(window, true);
}

void	ui_window_show_selection(t_widget *window)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	if (data->show_selection == false && data->show_selection_circle == false && data->show_selection_line == false)
		return ;
	if (data->show_selection == true)
	{
		ft_printf("show selection x: %d y: %d w: %d h: %d\n", data->selection.x, data->selection.y, data->selection.w, data->selection.h);
		SDL_SetRenderDrawColor(window->renderer, 22, 154, 0, 100);
		SDL_RenderFillRect(window->renderer, &data->selection);
		SDL_SetRenderDrawColor(window->renderer, 22, 226, 0, 255);
		SDL_RenderDrawRect(window->renderer, &data->selection);
	}
	else if (data->is_selectionning_line == true)
	{
		SDL_SetRenderDrawColor(window->renderer, 22, 226, 0, 255);
		SDL_RenderDrawLine(window->renderer, data->selection_start.x, data->selection_start.y, data->selection.x, data->selection.y);
	}
	else if (data->show_selection_circle == true)
	{
		int radius = (int)floorf(sqrt(pow(data->selection.w, 2) + pow(data->selection.h, 2)) / 2);
		int x = (radius - 1);
		int y = 0;
		int tx = 1;
		int ty = 1;
		int error = tx - (radius * 2);

		if (radius == 0)
			radius = 1;
		SDL_SetRenderDrawColor(window->renderer, 22, 226, 0, 255);
		ft_printf("show selection circle x: %d y: %d radius: %d\n", data->selection.x, data->selection.y, radius);
		while (x >= y)
		{
			SDL_RenderDrawPoint(window->renderer, data->selection.x + x, data->selection.y - y);
			SDL_RenderDrawPoint(window->renderer, data->selection.x + x, data->selection.y + y);
			SDL_RenderDrawPoint(window->renderer, data->selection.x - x, data->selection.y - y);
			SDL_RenderDrawPoint(window->renderer, data->selection.x - x, data->selection.y + y);
			SDL_RenderDrawPoint(window->renderer, data->selection.x + y, data->selection.y - x);
			SDL_RenderDrawPoint(window->renderer, data->selection.x + y, data->selection.y + x);
			SDL_RenderDrawPoint(window->renderer, data->selection.x - y, data->selection.y - x);
			SDL_RenderDrawPoint(window->renderer, data->selection.x - y, data->selection.y + x);
			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}
			if (error > 0)
			{
				--x;
				tx += 2;
				error += tx - (radius * 2);
			}
		}
		SDL_Texture *texture = SDL_CreateTextureFromSurface(window->renderer, data->surface);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureColorMod(texture, 22, 154, 0);
		SDL_SetTextureAlphaMod(texture, 100);
		SDL_RenderCopy(window->renderer, texture, NULL, &(SDL_Rect){data->selection.x - radius, data->selection.y - radius, radius * 2, radius * 2});
		SDL_DestroyTexture(texture);
	}
}

t_widget 	*ui_create_window(t_core *core, t_rect screen_rect, e_window_type type, const char *name)
{
	t_widget			*window;
	t_window_data		*data;

	if (core == NULL)
		return (NULL);
	window = ui_new_widget(NULL, (SDL_Rect){0, 0, screen_rect.width, screen_rect.height}, WINDOW, UI_MAX_WINDOW_CHILDS);
	if (!window)
		return (ui_destroy_widget(window), NULL);
	window->data = malloc(sizeof(t_window_data));
	if (!window->data)
		return (ui_destroy_widget(window), NULL);
	data = (t_window_data *)window->data;
	*data = (t_window_data){0};
	if (name)
		ft_strcpy(window->name, name);
	else
		ft_strcpy(window->name, "window");
	data->is_durty = true;
	window->core = core;
	window->owning_window = window;
	ui_set_widget_colors(window, 0xFF444444, 0xFF444444, 0xFF444444);
	window->render = ui_window_render;
	window->event = ui_window_event;
	window->destroy = ui_window_destroy;
	window->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW); 
	data->type = type;
	if (type == MODAL_ACCEPT_WINDOW)
		data->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_rect.width, screen_rect.height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
	else if (type == MODAL_INFO_WINDOW)
		data->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_rect.width, screen_rect.height, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS);
	else
		data->window = SDL_CreateWindow(name, screen_rect.x, screen_rect.y, screen_rect.width, screen_rect.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (data->window == NULL)
		return (ui_destroy_widget(window), NULL);
	window->renderer = SDL_CreateRenderer(data->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (window->renderer == NULL)
		return (ui_destroy_widget(window), NULL);
	SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);
	data->id = SDL_GetWindowID(data->window);
	window->texture = ui_new_texture(window->renderer, 32, 32, window->colors[window->state]);
	data->custom_cursor.color = (SDL_Color){255, 255, 255, 255};
	data->surface = IMG_Load("assets/stickers/brush3.png");
	if (type == MODAL_ACCEPT_WINDOW || type == MODAL_INFO_WINDOW)
	{
		if (core->modal.window)
			ui_destroy_widget(core->modal.window);
		core->modal.window = window;

		if (core->hovered_widget && core->onwidgetunhovered)
			ui_widget_call_onunhovered(core, core->hovered_widget);
		core->hovered_widget = NULL;
		ui_set_cursor(core, window->cursor);
		core->focused_window = window;
		if (type == MODAL_INFO_WINDOW)
		{
			core->modal.button_cancel = ui_create_prefab_button(window, (t_rect){screen_rect.width / 2 - 40, screen_rect.height - 50, 80, 30}, "OK", FIT);
			core->modal.button_ok.button = NULL;
			core->modal.button_ok.text = NULL;
		}
		else if (type == MODAL_ACCEPT_WINDOW)
		{
			core->modal.button_ok = ui_create_prefab_button(window, (t_rect){screen_rect.width / 2 - 100, screen_rect.height - 50, 80, 30}, "ACCEPT", FIT);
			core->modal.button_cancel = ui_create_prefab_button(window, (t_rect){screen_rect.width / 2 + 20, screen_rect.height - 50, 80, 30}, "CANCEL", FIT);
		}
		ui_build_widget(window);
		SDL_RaiseWindow(data->window);
	}
	else
		if (ui_core_add_window(core, window) == -1)
			return (ui_destroy_widget(window), NULL);
	return (window);
}

void	ui_set_custom_cursor(t_widget *window, const char *path, int width, int height)
{
	t_window_data	*data;

	if (window->type != WINDOW)
		return ;
	data = window->data;
	if (ft_strcmp(path, data->custom_cursor.path) == 0)
		return ;
	if (data->custom_cursor.texture)
		SDL_DestroyTexture(data->custom_cursor.texture);
	data->custom_cursor.texture = IMG_LoadTexture(window->renderer, path);
	SDL_SetTextureBlendMode(data->custom_cursor.texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureColorMod(data->custom_cursor.texture, data->custom_cursor.color.r, data->custom_cursor.color.g, data->custom_cursor.color.b);
	SDL_SetTextureAlphaMod(data->custom_cursor.texture, data->custom_cursor.color.a);
	data->custom_cursor.rect.w = width;
	data->custom_cursor.rect.h = height;
	data->custom_cursor.hot_spot.x = width / 2;
	data->custom_cursor.hot_spot.y = height / 2;
	ft_strcpy(data->custom_cursor.path, path);
	// ft_printf("ui_set_custom_cursor\n");
}

void	ui_set_custom_cursor_size(t_widget *window, int width, int height)
{
	t_window_data	*data;

	data = window->data;
	data->custom_cursor.rect.w = width;
	data->custom_cursor.rect.h = height;
	data->custom_cursor.hot_spot.x = width / 2;
	data->custom_cursor.hot_spot.y = height / 2;
	data->custom_cursor.rect.x = window->core->mouse.position.x - data->custom_cursor.hot_spot.x;
	data->custom_cursor.rect.y = window->core->mouse.position.y - data->custom_cursor.hot_spot.y;
	ui_window_set_is_dirty(window, true);
	// ft_printf("ui_set_custom_cursor_size\n");

}

void	ui_set_custom_cursor_color(t_widget *window, unsigned int packed_color)
{
	SDL_Color		color;
	t_window_data	*data;

	color = ui_unpack_color(packed_color);
	data = window->data;
	SDL_SetTextureColorMod(data->custom_cursor.texture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(data->custom_cursor.texture, color.a);
	data->custom_cursor.color = color;
	// ft_printf("ui_set_custom_cursor_color\n");

}

void	ui_set_use_custom_cursor(t_widget *window, bool newUse)
{
	t_window_data	*data;

	data = window->data;
	if (data->use_custom_cursor == newUse)
		return ;
	data->use_custom_cursor = newUse;
	SDL_ShowCursor(!newUse);
	// ft_printf("ui_set_use_custom_cursor %d\n", newUse);
	ui_window_set_is_dirty(window, true);
}

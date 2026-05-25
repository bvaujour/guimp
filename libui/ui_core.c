/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:23:08 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

t_core	*ui_init()
{
	int 	img_flags;
	t_core	*core;
	int		i;

	i = 0;
	core = malloc(sizeof(t_core));
	if (core == NULL)
		return (NULL);
	*core = (t_core){0};
	img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0 || (IMG_Init(img_flags) & img_flags) != img_flags)
		return (ui_destroy(core), NULL);
	core->windows = ui_new_widget_tab(UI_MAX_WINDOWS);
	if (!core->windows)
	{
		free (core);
		return (NULL);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	core->font_file = ft_strdup("libui/assets/fonts/Roboto-Regular.ttf");
	core->font = TTF_OpenFont(core->font_file, 25);
	if (!core->font_file)
		return (NULL);
	while (i < SDL_NUM_SYSTEM_CURSORS)
	{
		core->mouse.system_cursors[i] = SDL_CreateSystemCursor(i);
		i++;
	}
	return (core);
}

void	ui_quit(t_core *core)
{
	core->is_running = false;
}

void	ui_set_cursor(t_core *core, SDL_Cursor *cursor)
{
	if (cursor != core->mouse.current_cursor)
	{
		SDL_SetCursor(cursor);
		core->mouse.current_cursor = cursor;
	}
}

void	ui_destroy(t_core *core)
{
	int	i;

	i = 0;
	if (core->override_cursor)
		SDL_FreeCursor(core->override_cursor);
	if (core->copy_info.texture)
		SDL_DestroyTexture(core->copy_info.texture);
	if (core->modal.window)
		ui_destroy_widget(core->modal.window);
	while (core->windows[i])
	{
		ui_destroy_widget(core->windows[i]);
		i++;
	}
	i = 0;
	while (i < SDL_NUM_SYSTEM_CURSORS)
	{
		if (core->mouse.system_cursors[i])
			SDL_FreeCursor(core->mouse.system_cursors[i]);
		i++;
	}
	TTF_CloseFont(core->font);
	free(core->windows);
	free(core->font_file);
	free(core);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void	ui_close_modal(t_core *core)
{
	ui_destroy_widget(core->modal.window);
	core->modal.window = NULL;
	core->focused_window = NULL;
	core->mouse.buttons[SDL_BUTTON_LEFT] = false;
	core->mouse.buttons[SDL_BUTTON_RIGHT] = false;
}

void	ui_close_window(t_core *core, t_widget *window)
{
	int				i;

	i = 0;
	if (window == core->modal.window)
	{
		ui_close_modal(core);
		return ;
	}
	while (core->windows[i])
	{
		if (core->windows[i] == window)
		{
			ui_destroy_widget(window);
			core->windows[i] = NULL;
			break ;
		}
		i++;
	}
	while (i < UI_MAX_WINDOWS - 1)
	{
		core->windows[i] = core->windows[i + 1];
		i++;
	}
}

void	ui_start_selection_line(t_widget *window, t_widget *container)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	data->selection_start = window->core->mouse.position;
	data->selection.x = data->selection_start.x;
	data->selection.y = data->selection_start.y;
	data->on_widget = container;
	data->is_selectionning_line = true;
	data->show_selection_line = true;
	ui_window_set_is_dirty(window, true);
}

void	ui_start_selection_circle(t_widget *window, t_widget *container)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	data->selection_start = window->core->mouse.position;
	data->on_widget = container;
	data->is_selectionning_circle = true;
	data->show_selection_circle = true;
	ui_window_set_is_dirty(window, true);
}

void	ui_start_selection(t_widget *window, t_widget *container)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	data->selection_start = window->core->mouse.position;
	data->selection.w = 0;
	data->selection.h = 0;
	data->on_widget = container;
	data->is_selectionning = true;
	data->show_selection = true;
	ui_window_set_is_dirty(window, true);
}

void	ui_stop_selection(t_widget *window)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	data->is_selectionning = false;
	data->is_selectionning_circle = false;
	data->is_selectionning_line = false;
	// data->show_selection = false;
	ui_window_set_is_dirty(window, true);
}

void	ui_stop_hide_selection(t_widget *window)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	data->is_selectionning = false;
	data->is_selectionning_circle = false;
	data->is_selectionning_line = false;
	data->show_selection = false;
	data->show_selection_circle = false;
	data->show_selection_line = false;
	ui_window_set_is_dirty(window, true);
}

t_rect	ui_get_selection(t_widget *window)
{
	t_window_data	*data;

	data = (t_window_data *)window->data;
	return ((t_rect){data->selection.x, data->selection.y, data->selection.w, data->selection.h});
}

void	ui_set_use_override_cursor(t_core *core, bool new_use)
{
	core->use_override_cursor = new_use;
}

void	ui_set_override_cursor(t_core *core, const char *path, int hot_x, int hot_y)
{
	SDL_Surface	*surface;

	surface = IMG_Load(path);
	if (surface == NULL)
		return ;
	if (core->override_cursor)
		SDL_FreeCursor(core->override_cursor);
	core->override_cursor = SDL_CreateColorCursor(surface, hot_x, hot_y);
	if (core->override_cursor == NULL)
		ft_printf("core->override_cursor == NULL %s\n", SDL_GetError());
	SDL_FreeSurface(surface);
}


static void	ui_mark_texts_dirty(t_widget *widget)
{
	t_text_data	*data;
	int			i;

	if (!widget)
		return ;
	if (widget->type == TEXT)
	{
		data = (t_text_data *)widget->data;
		data->str_is_dirty = true;
		widget->build(widget);
	}
	i = 0;
	while (i < widget->nb_child)
	{
		ui_mark_texts_dirty(widget->childs[i]);
		i++;
	}
}

void	ui_set_font(t_core *core, const char *path, int size)
{
	TTF_Font	*new_font;
	int			i;

	new_font = TTF_OpenFont(path, size);
	if (!new_font)
		return ;
	TTF_CloseFont(core->font);
	free(core->font_file);
	core->font = new_font;
	core->font_file = ft_strdup(path);
	i = 0;
	while (core->windows[i])
	{
		ui_mark_texts_dirty(core->windows[i]);
		i++;
	}
	if (core->modal.window)
		ui_mark_texts_dirty(core->modal.window);
}

void	ui_bind_onkeypress(t_core *core, void (*f)(int key, void *param), void *param)
{
	core->onkeypress = f;
	core->onkeypress_param = param;
}

void	ui_bind_onkeyrelease(t_core *core, void (*f)(int key, void *param), void *param)
{
	core->onkeyrelease = f;
	core->onkeyrelease_param = param;
}

void	ui_bind_onwidgetclicked(t_core *core, void (*f)(t_widget *widget, int mouse_button, int x, int y, void *param), void *param)
{
	core->onwidgetclicked = f;
	core->onwidgetclicked_param = param;
}

void	ui_bind_onwidgetwheeled(t_core *core, void (*f)(t_widget *widget, int value, void *param), void *param)
{
	core->onwidgetwheeled = f;
	core->onwidgetwheeled_param = param;
}

void	ui_bind_onwidgetreleased(t_core *core, void (*f)(t_widget *widget, int mouse_button, int x, int y, void *param), void *param)
{
	core->onwidgetreleased = f;
	core->onwidgetreleased_param = param;
}

void	ui_bind_onwidgethovered(t_core *core, void (*f)(t_widget *widget, int x, int y, void *param), void *param)
{
	core->onwidgethovered = f;
	core->onwidgethovered_param = param;
}

void	ui_bind_onwidgetunhovered(t_core *core, void (*f)(t_widget *widget, void *param), void *param)
{
	core->onwidgetunhovered = f;
	core->onwidgetunhovered_param = param;
}

void	ui_bind_onsliderslided(t_core *core, void (*f)(t_widget *slider, float value, void *param), void *param)
{
	core->onsilderslided = f;
	core->onsilderslided_param = param;
}

void	ui_bind_oninputvalidated(t_core *core, void (*f)(t_widget *input, const char *str, void *param), void *param)
{
	core->oninputvalidated = f;
	core->oninputvalidated_param = param;
}

void	ui_bind_onfiledropped(t_core *core, void (*f)(t_widget *widget, const char *path, void *param), void *param)
{
	core->onfiledropped = f;
	core->onfiledropped_param = param;
}

void	ui_bind_onwindowclosed(t_core *core, void (*f)(t_widget *window, bool accepted, void *param), void *param)
{
	core->onwindowclosed = f;
	core->onwindowclosed_param = param;
}

void	ui_bind_oninputchanged(t_core *core, void (*f)(t_widget *input, char *str, void *param), void *param)
{
	core->oninputchanged = f;
	core->oninputchanged_param = param;
}
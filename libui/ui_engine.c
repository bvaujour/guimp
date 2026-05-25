/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_engine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:31:56 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

void ui_destroy_widget(t_widget *widget)
{
	int	i;

	if (widget == NULL)
		return ;
	if (widget->childs)
	{
		i = 0;
		while (widget->childs[i])
		{
			ui_destroy_widget(widget->childs[i]);
			i++;
		}
		free(widget->childs);
	}
	if (widget == widget->core->focused_widget)
		widget->core->focused_widget = NULL;
	if (widget == widget->core->clicked_widget)
		widget->core->clicked_widget = NULL;
	if (widget == widget->core->resizable_widget)
		widget->core->resizable_widget = NULL;
	if (widget == widget->core->draggable_widget)
		widget->core->draggable_widget = NULL;
	if (widget == widget->core->hovered_widget)
		widget->core->hovered_widget = NULL;
	if (widget == widget->core->last_hovered_widget)
		widget->core->last_hovered_widget = NULL;
	if (widget == widget->core->scrollable_widget)
		widget->core->scrollable_widget = NULL;
	if (widget->texture && widget->is_clone == false)
		SDL_DestroyTexture(widget->texture);
	if (widget->cursor)
		SDL_FreeCursor(widget->cursor);
	if (widget->destroy)
		widget->destroy(widget);
	free(widget->data);
	free(widget);
}

void	ui_build_widget(t_widget *widget)
{
	int	i;

	i = 0;
	if (widget->build)
		widget->build(widget);
	if (widget->childs)
		while (widget->childs[i])
		{
			ui_build_widget(widget->childs[i]);
			i++;
		}
}

static void ui_render_widget(t_widget *widget)
{
	int	i;

	if (widget->is_visible == false)
		return ;
	widget->clip = ui_get_clip_rect(widget);
	SDL_RenderSetClipRect(widget->renderer, &widget->clip);
	if (widget->render)
		widget->render(widget);
	SDL_RenderSetClipRect(widget->renderer, NULL);
	if (widget->childs)
	{
		i = 0;
		while (widget->childs[i])
		{
			ui_render_widget(widget->childs[i]);
			i++;
		}
	}
	SDL_RenderSetClipRect(widget->renderer, &widget->clip);
	SDL_SetRenderDrawColor(widget->renderer, 0, 0, 0, 255);
	ui_widget_outline(widget, widget->outline);
	if (widget == widget->core->focused_widget)
	{
		SDL_SetRenderDrawColor(widget->renderer, 255, 255, 255, 255);
		ui_widget_outline(widget, 2);
	}
	SDL_RenderSetClipRect(widget->renderer, NULL);
}

void	ui_render(t_core *core)
{
	int	i;
	t_window_data	*data;
	i = 0;
	while (core->windows[i])
	{
		data = core->windows[i]->data;
		if (data->is_durty || (data->use_custom_cursor && core->use_override_cursor == false))
		{
			ui_render_widget(core->windows[i]);
			ui_window_show_selection(core->windows[i]);
			if (data->use_custom_cursor && core->use_override_cursor == false)
				SDL_RenderCopy(core->windows[i]->renderer, data->custom_cursor.texture, NULL, &data->custom_cursor.rect);
			SDL_RenderPresent(core->windows[i]->renderer);
			ui_window_set_is_dirty(core->windows[i], false);
		}
		i++;
	}
	if (core->modal.window && ui_window_get_is_dirty(core->modal.window))
	{
		ui_render_widget(core->modal.window);
		SDL_RenderPresent(core->modal.window->renderer);
		ui_window_set_is_dirty(core->modal.window, false);
	}
	
}

t_widget	*ui_find_window_by_id(t_core *core, unsigned int id)
{
	int	i;
	t_window_data	*data;

	if (core->modal.window)
	{
		data = core->modal.window->data;
		if (id == data->id)
			return (core->modal.window);
	}
	i = 0;
	while (core->windows[i])
	{
		data = core->windows[i]->data;
		if (id == data->id)
			return (core->windows[i]);
		i++;
	}
	return (NULL);
}

void	ui_find_essential_widgets(t_core *core, t_widget *widget)
{
	int			i;
	SDL_Point	absolute;

	if (widget->is_visible == false)
		return ;
	absolute = ui_get_absolute_position(widget);
	if (SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x, absolute.y, widget->width, widget->height}))
	{
		if (ui_widget_can_scroll(widget))
			core->scrollable_widget = widget;
		if (widget->hit_testable == true)
			core->hovered_widget = widget;
		if (widget->childs)
		{
			i = 0;
			while (widget->childs[i])
			{
				ui_find_essential_widgets(core, widget->childs[i]);
				i++;
			}
		}
	}
}

void	ui_build(t_core *core)
{
	int				i;

	i = 0;
	while (core->windows[i])
	{
		ui_build_widget(core->windows[i]);
		ui_render_widget(core->windows[i]);
		SDL_RenderPresent(core->windows[i]->renderer);
		i++;
	}
	if (core->modal.window)
	{
		ui_build_widget(core->modal.window);
		ui_render_widget(core->modal.window);
		SDL_RenderPresent(core->modal.window->renderer);
	}
}

void	ui_resize_window(t_core *core, int window_id, int width, int height)
{
	t_widget	*window;
	
	ft_printf("window_resize\n");
	window = ui_find_window_by_id(core, window_id);
	ui_set_widget_size(window, width, height);
}

void	ui_call_close_window(t_core *core, bool accepted, t_widget *window)
{
	if (core->onwindowclosed)
		core->onwindowclosed(window, accepted, core->onwindowclosed_param);
	if (window == core->modal.window)
		ui_close_modal(core);
}

void ui_focus_brother(t_core *core)
{
	int			i;
	t_widget	*parent;

	if (core->focused_widget == NULL || core->focused_widget->parent == NULL)
		return ;
	parent = core->focused_widget->parent;
	i = 0;
	while (parent->childs[i])
	{
		if (parent->childs[i] == core->focused_widget)
			break ;
		i++;
	}
	if (parent->childs[i + 1])
		core->focused_widget = parent->childs[i + 1];
	else
		core->focused_widget = parent->childs[0];
	ui_window_set_is_dirty(core->focused_widget->owning_window, true);
}


void	ui_manage_cursor(t_core *core)
{
	if (core->use_override_cursor)
	{
		ui_set_cursor(core, core->override_cursor);
		ft_printf("set override cursor\n");
	}
	else if (core->resizable_widget && core->can_resize_w + core->can_resize_h + core->can_resize_x + core->can_resize_y >= 2)
		ui_set_cursor(core, core->mouse.system_cursors[SDL_SYSTEM_CURSOR_SIZENWSE]);
	else if (core->resizable_widget && (core->can_resize_w || core->can_resize_x))
		ui_set_cursor(core, core->mouse.system_cursors[SDL_SYSTEM_CURSOR_SIZEWE]);
	else if (core->resizable_widget && (core->can_resize_h || core->can_resize_y))
		ui_set_cursor(core, core->mouse.system_cursors[SDL_SYSTEM_CURSOR_SIZENS]);
	else if (core->draggable_widget)
		ui_set_cursor(core, core->mouse.system_cursors[SDL_SYSTEM_CURSOR_SIZEALL]);
	else if (core->hovered_widget)
		ui_set_cursor(core, core->hovered_widget->cursor);
}

void	ui_manage_resize(t_core *core)
{
	if (core->can_resize_w)
		core->resizable_widget->width += core->resizable_widget->core->mouse.motion.x;
	if (core->can_resize_h)
		core->resizable_widget->height += core->resizable_widget->core->mouse.motion.y;
	if (core->can_resize_x)
	{
		core->resizable_widget->position.x += core->resizable_widget->core->mouse.motion.x;
		core->resizable_widget->width -= core->resizable_widget->core->mouse.motion.x;
	}
	if (core->can_resize_y)
	{
		core->resizable_widget->position.y += core->resizable_widget->core->mouse.motion.y;
		core->resizable_widget->height -= core->resizable_widget->core->mouse.motion.y;
	}
	if (core->resizable_widget->width < UI_MIN_RESIZE_W)
		core->resizable_widget->width = UI_MIN_RESIZE_W;
	if (core->resizable_widget->height < UI_MIN_RESIZE_H)
		core->resizable_widget->height = UI_MIN_RESIZE_H;
	ui_build_widget(core->resizable_widget);
	ui_window_set_is_dirty(core->resizable_widget->owning_window, true);
}

void	ui_run(t_core *core)
{
	t_window_data *data;

	core->is_running = true;
	ui_build(core);
	if (core->windows && core->windows[0])
	{
		data = core->windows[0]->data;
		SDL_RaiseWindow(data->window);
	}
	while (core->is_running)
	{
		if (SDL_WaitEvent(&core->event))
		{
			core->keys = SDL_GetKeyboardState(NULL);
			if (core->event.type == SDL_WINDOWEVENT)
			{
				if (core->event.window.event == SDL_WINDOWEVENT_RESIZED)
					ui_resize_window(core, core->event.window.windowID, core->event.window.data1, core->event.window.data2);
				else if (core->event.window.event == SDL_WINDOWEVENT_LEAVE)
				{
					// ft_printf("window_leave\n");
					core->focused_window = NULL;
				}
				else if (core->event.window.event == SDL_WINDOWEVENT_ENTER)
				{
					// ft_printf("window_enter\n");
					core->focused_window = ui_find_window_by_id(core, core->event.window.windowID);
					if (core->modal.window && core->focused_window != core->modal.window)
						core->focused_window = NULL;
				}
				else if (core->event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					ui_call_close_window(core, false, ui_find_window_by_id(core, core->event.window.windowID));
					continue ;
				}
			}
			else if (core->event.type == SDL_KEYDOWN)
			{
				if (core->modal.window && core->event.key.keysym.scancode == SDL_SCANCODE_RETURN)
				{
					ui_call_close_window(core, true, core->modal.window);
					continue ;
				}
				else if (core->modal.window && core->event.key.keysym.scancode == SDL_SCANCODE_DELETE)
				{
					ui_call_close_window(core, false, core->modal.window);
					continue ;
				}
				else if (core->onkeypress && core->modal.window == NULL)
					core->onkeypress(core->event.key.keysym.scancode, core->onkeypress_param);
				if (core->focused_widget && core->event.key.keysym.scancode == SDL_SCANCODE_TAB)
					ui_focus_brother(core);
			}
			else if (core->event.type == SDL_KEYUP)
			{
				if (core->onkeyrelease && core->modal.window == NULL)
					core->onkeyrelease(core->event.key.keysym.scancode, core->onkeyrelease_param);
			}
			else if (core->event.type == SDL_MOUSEBUTTONDOWN)
			{
				core->mouse.buttons[core->event.button.button] = true;
				if (core->modal.window && core->hovered_widget == core->modal.button_cancel.button)
				{
					ui_call_close_window(core, false, core->modal.window);
					continue ;
				}
				else if (core->modal.window && core->hovered_widget == core->modal.button_ok.button)
				{
					ui_call_close_window(core, true, core->modal.window);
					continue ;
				}
			}
			else if (core->event.type == SDL_MOUSEBUTTONUP)
			{
				core->mouse.buttons[core->event.button.button] = false;
				core->draggable_widget = NULL;
				core->resizable_widget = NULL;
				core->clicked_widget = NULL;
			}
			else if (core->event.type == SDL_DROPFILE)
			{
				if (core->onfiledropped)
					core->onfiledropped(core->hovered_widget, core->event.drop.file, core->onfiledropped_param);
				free(core->event.drop.file);
			}
			else if (core->event.type == SDL_MOUSEWHEEL)
			{
				if (core->keys[SDL_SCANCODE_LCTRL] == true && core->hovered_widget)
				{
					if (core->onwidgetwheeled)
						core->onwidgetwheeled(core->hovered_widget, core->event.wheel.y, core->onwidgetwheeled_param);
				}
				if (core->scrollable_widget)
					ui_manage_wheel(core->scrollable_widget);
			}
			else if (core->event.type == SDL_MOUSEMOTION)
			{
				t_window_data *data;
				core->mouse.motion.x = core->event.motion.x - core->mouse.position.x;
				core->mouse.motion.y = core->event.motion.y - core->mouse.position.y;
				core->mouse.position.x = core->event.motion.x;
				core->mouse.position.y = core->event.motion.y;
				if (core->resizable_widget && core->mouse.buttons[SDL_BUTTON_LEFT])
					ui_manage_resize(core);
				else if (core->draggable_widget && core->mouse.buttons[SDL_BUTTON_RIGHT])
					ui_add_widget_position(core->draggable_widget, core->mouse.motion.x, core->mouse.motion.y);
				if (core->focused_window)
				{
					data = core->focused_window->data;
					data->custom_cursor.rect.x = core->mouse.position.x - data->custom_cursor.hot_spot.x;
					data->custom_cursor.rect.y = core->mouse.position.y - data->custom_cursor.hot_spot.y;
				}
			}
			if (core->focused_window)
			{
				t_window_data *data = (t_window_data *)core->focused_window->data;
				if (data->is_selectionning)
					ui_window_selection_rect(core->focused_window);
				if (data->is_selectionning_circle)
					ui_window_selection_circle(core->focused_window);
				if (data->is_selectionning_line)
					ui_window_selection_line(core->focused_window);
				core->can_resize_w = false;
				core->can_resize_h = false;
				core->can_resize_x = false;
				core->can_resize_y = false;
				core->draggable_widget = NULL;
				core->resizable_widget = NULL;
				core->scrollable_widget = NULL;
				core->last_hovered_widget = core->hovered_widget;
				ui_find_essential_widgets(core, core->focused_window);
				if (core->hovered_widget)
				{
					SDL_Point	absolute = ui_get_absolute_position(core->hovered_widget);
					
					if (core->hovered_widget->is_resizable == true && !SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x + UI_RESIZE_ZONE, absolute.y + UI_RESIZE_ZONE, core->hovered_widget->width - 2 * UI_RESIZE_ZONE, core->hovered_widget->height - 2 * UI_RESIZE_ZONE}))
					{
						if (SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x, absolute.y, UI_RESIZE_ZONE, core->hovered_widget->height}))
							core->can_resize_x = true;
						if (SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x + core->hovered_widget->width - UI_RESIZE_ZONE, absolute.y, UI_RESIZE_ZONE, core->hovered_widget->height}))
							core->can_resize_w = true;
						if (SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x, absolute.y + core->hovered_widget->height - UI_RESIZE_ZONE, core->hovered_widget->width, UI_RESIZE_ZONE}))
							core->can_resize_h = true;
						if (SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x, absolute.y, core->hovered_widget->width, UI_RESIZE_ZONE}))
							core->can_resize_y = true;
						core->resizable_widget = core->hovered_widget;
					}
					else if (core->hovered_widget->is_draggable == true)
					{
						if (core->hovered_widget->drag_full_width &&  core->hovered_widget->drag_full_height)
							core->draggable_widget = core->hovered_widget;
						else if (core->hovered_widget->drag_full_width && SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x, absolute.y + core->hovered_widget->drag_zone.y, core->hovered_widget->width, core->hovered_widget->drag_zone.h}))
							core->draggable_widget = core->hovered_widget;
						else if (core->hovered_widget->drag_full_height && SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x + core->hovered_widget->drag_zone.x, absolute.y, core->hovered_widget->drag_zone.w, core->hovered_widget->height}))
							core->draggable_widget = core->hovered_widget;
						else if (SDL_PointInRect(&core->mouse.position, &(SDL_Rect){absolute.x + core->hovered_widget->drag_zone.x, absolute.y + core->hovered_widget->drag_zone.y, core->hovered_widget->drag_zone.w, core->hovered_widget->drag_zone.h}))
							core->draggable_widget = core->hovered_widget;
					}
					if (core->last_hovered_widget != core->hovered_widget)
					{
						if (core->onwidgetunhovered && core->last_hovered_widget)
							ui_widget_call_onunhovered(core, core->last_hovered_widget);
						if (core->onwidgethovered && core->hovered_widget)
							ui_widget_call_onhovered(core, core->hovered_widget);
					}
					ui_manage_cursor(core);
				}
					
			}
			if (core->hovered_widget && core->hovered_widget->event)
				core->hovered_widget->event(core->hovered_widget);
			if (core->focused_widget && core->focused_widget->update)
				core->focused_widget->update(core->focused_widget);
			ui_render(core);
		}
		// else
		// {
		// 	if (core->focused_window)
		// 		ui_manage_cursor(core);
		// 	ui_render(core);
		// }
	}
	ui_destroy(core);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_widget.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 16:12:45 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:25:26 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

SDL_Point	ui_get_absolute_position(t_widget *widget)
{
	SDL_Point	absolute;
	t_widget	*reference;

	absolute = widget->position;
	reference = widget->parent;
	while (reference != NULL)
	{
		absolute.x += reference->position.x;
		absolute.y += reference->position.y;
		reference = reference->parent;
	}
	return (absolute);
}

// bool	ui_is_in_familly_of(t_widget *widget, t_widget *other)
// {
// 	t_widget	*reference;

// 	reference = widget;
// 	while (reference != NULL)
// 	{
// 		if (reference == other)
// 			return (true);
// 		reference = reference->parent;
// 	}
// 	return (false);
// }


SDL_Rect ui_get_clip_rect(t_widget *widget)
{
	SDL_Rect 	clip;
	SDL_Point	absolute;

	absolute = ui_get_absolute_position(widget);
	clip = (SDL_Rect){absolute.x, absolute.y, widget->width, widget->height};
	if (widget->parent == NULL)
		return (clip);
	if (!SDL_IntersectRect(&clip, &widget->parent->clip, &clip))
		return (SDL_Rect){0, 0, 0, 0};
	return (clip);
}


void	ui_widget_change_state(t_widget *widget, e_widget_state new_state)
{
	SDL_Color	color;
	SDL_Color	last_color;

	if (widget->state != new_state)
	{
		widget->state = new_state;
		if (widget->is_clone)
			return ;
		color = widget->colors[new_state];
		if (SDL_GetTextureColorMod(widget->texture, &last_color.r, &last_color.g, &last_color.b) == 0 && (last_color.r != color.r || last_color.g != color.g || last_color.b != color.b))
		{
			SDL_SetTextureColorMod(widget->texture, color.r, color.g, color.b);
			ui_window_set_is_dirty(widget->owning_window, true);
		}
		if (SDL_GetTextureAlphaMod(widget->texture, &last_color.a) == 0 && (last_color.a != color.a))
		{
			SDL_SetTextureAlphaMod(widget->texture, color.a);
			ui_window_set_is_dirty(widget->owning_window, true);
		}
	}
}

void	ui_default_hovered(t_widget *widget, void *param)
{
	(void)param;
	ft_printf("widget %s(%p) hovered\n", widget->name, widget);
}

void	ui_default_unhovered(t_widget *widget, void *param)
{
	(void)param;
	ft_printf("widget %s(%p) unhovered\n", widget->name, widget);
}

void	ui_default_clicked(t_widget *widget, int button, int x, int y, void *param)
{
	(void)param;
	ft_printf("widget %s(%p) clicked at %d, %d by button %d\n", widget->name, widget, x, y, button);
}


t_widget *ui_new_widget(t_widget *parent, SDL_Rect rect, e_widget_type type, int max_child)
{
	t_widget		*widget;

	widget = malloc(sizeof(t_widget));
	if (widget == NULL)
		return (NULL);
	*widget = (t_widget){0};
	if (max_child)
	{
		widget->childs = ui_new_widget_tab(max_child);
		if (widget->childs == NULL)
			return (free(widget), NULL);
	}
	widget->max_child = max_child;
	widget->position.x = rect.x;
	widget->position.y = rect.y;
	widget->width = rect.w;
	widget->height = rect.h;
	widget->type = type;
	widget->is_visible = true;
	widget->hit_testable = true;
	if (parent)
	{
		widget->parent = parent;
		widget->core = parent->core;
		widget->renderer = parent->renderer;
		widget->owning_window = parent->owning_window;
	}
	return (widget);
}

int	ui_add_child(t_widget *parent, t_widget *child)
{
	if (parent->nb_child == parent->max_child)
	{
		ft_printf("ui_add_child: parent %s has maximum child\n", parent->name);
		return (UI_ERROR);
	}
	parent->childs[parent->nb_child] = child;
	parent->nb_child++;
	if (parent->core->is_running)
		ui_build_widget(parent);
	return (UI_SUCCESS);
}

t_widget 	**ui_new_widget_tab(int tab_len)
{
	t_widget	**childs;
	int			i;

	i = 0;
	childs = malloc(sizeof(t_widget *) * (tab_len + 1));
	if (childs == NULL)
	{
		ft_printf("ui_new_widget_tab failed\n");
		return (NULL);
	}
	while (i <= tab_len)
	{
		childs[i] = NULL;
		i++;
	}
	return (childs);
}

int	ui_core_add_window(t_core *core, t_widget *window)
{
	int	i;

	i = 0;
	while (i < UI_MAX_WINDOWS)
	{
		if (core->windows[i] == NULL)
		{
			core->windows[i] = window;
			return (0);
		}
		i++;
	}
	return (-1);
}

void	ui_widget_outline(t_widget *widget, int thickness)
{
	SDL_Point	absolute;
	int	i;

	if (thickness == 0)
		return ;
	i = 0;
	absolute = ui_get_absolute_position(widget);
	while (i < thickness)
	{
		SDL_RenderDrawRect(widget->renderer, &(SDL_Rect){absolute.x + i, absolute.y + i, widget->width - 2 * i, widget->height - 2 * i});
		i++;
	}
}

void		ui_widget_call_onclicked(t_core *core, t_widget *widget, int x, int y, int mouse_button)
{
	if (widget->type == ONOFF_BUTTON)
		widget->is_on = !widget->is_on;
	ui_widget_change_state(widget, CLICKED);
	core->clicked_widget = widget;
	if (core->onwidgetclicked)
		core->onwidgetclicked(widget, mouse_button, x, y, core->onwidgetclicked_param);
}

void		ui_widget_call_onreleased(t_core *core, t_widget *widget, int x, int y, int mouse_button)
{
	if (widget == core->hovered_widget)
	{
		if (widget->type == ONOFF_BUTTON && widget->is_on == true)
			ui_widget_change_state(widget, CLICKED);
		else
			ui_widget_change_state(widget, HOVERED);
	}
	else
	{
		if (widget->type == ONOFF_BUTTON && widget->is_on == true)
			ui_widget_change_state(widget, CLICKED);
		else
			ui_widget_change_state(widget, NORMAL);
	}
	if (core->onwidgetreleased)
		core->onwidgetreleased(widget, mouse_button, x, y, core->onwidgetreleased_param);
}

void		ui_widget_call_onhovered(t_core *core, t_widget *widget)
{
	SDL_Point	relative_mouse_position;
	SDL_Point	absolute;

	absolute = ui_get_absolute_position(widget);
	relative_mouse_position.x = widget->core->mouse.position.x - absolute.x;
	relative_mouse_position.y = widget->core->mouse.position.y - absolute.y;
	if (widget->type == ONOFF_BUTTON && widget->is_on == true)
		ui_widget_change_state(widget, CLICKED);
	else
		ui_widget_change_state(widget, HOVERED);
	core->onwidgethovered(widget, relative_mouse_position.x, relative_mouse_position.y, core->onwidgethovered_param);
}

void		ui_widget_call_onunhovered(t_core *core, t_widget *widget)
{
	if (widget->type == ONOFF_BUTTON && widget->is_on == true)
		ui_widget_change_state(widget, CLICKED);
	else
		ui_widget_change_state(widget, NORMAL);
	core->onwidgetunhovered(widget, core->onwidgetunhovered_param);
}

SDL_Rect ui_calculate_inner(t_widget *widget)
{
	int	i;
	if (widget->nb_child == 0)
		return ((SDL_Rect){0, 0, 0, 0});
	int	min_x = 0;
	int	min_y = 0;
	int	max_x = 0;
	int	max_y = 0;

	i = 0;
	while (widget->childs[i])
	{
		if (widget->childs[i]->position.x < min_x)
			min_x = widget->childs[i]->position.x;
		if (widget->childs[i]->position.y < min_y)
			min_y = widget->childs[i]->position.y;
		if (widget->childs[i]->position.x + widget->childs[i]->width > max_x)
			max_x = widget->childs[i]->position.x + widget->childs[i]->width;
		if (widget->childs[i]->position.y + widget->childs[i]->height > max_y)
			max_y = widget->childs[i]->position.y + widget->childs[i]->height;
		i++;
	}
	return ((SDL_Rect){min_x, min_y, max_x - min_x, max_y - min_y});
}

bool	ui_widget_can_scroll(t_widget *widget)
{
	int	i;

	if (widget->nb_child == 0)
		return (false);
	if (widget->scroll_mode == NO_SCROLL)
		return (false);
	if (widget->scroll.y != 0 || widget->scroll.x != 0 || widget->scroll_mode == FREE_SCROLL)
		return (true);
	i = 0;
	while (widget->childs[i])
	{
		if (widget->childs[i]->position.y < 0)
			return (true);
		if (widget->childs[i]->position.x < 0)
			return (true);
		if (widget->childs[i]->position.y + widget->childs[i]->height > widget->height)
			return (true);
		if (widget->childs[i]->position.x + widget->childs[i]->width > widget->width)
			return (true);
		i++;
	}
	return (false);
}

bool	ui_widget_can_scroll_down(t_widget *widget)
{
	int	i;

	if (widget->scroll.y < 0 || widget->scroll_mode == FREE_SCROLL)
		return (true);
	i = 0;
	while (widget->childs[i])
	{
		if (widget->childs[i]->position.y < 0)
			return (true);
		i++;
	}
	return (false);
}

bool	ui_widget_can_scroll_up(t_widget *widget)
{
	int	i;

	if (widget->scroll.y > 0 || widget->scroll_mode == FREE_SCROLL)
		return (true);
	i = 0;
	while (widget->childs[i])
	{
		if (widget->childs[i]->position.y + widget->childs[i]->height > widget->height)
			return (true);
		i++;
	}
	return (false);
}

bool	ui_widget_can_scroll_right(t_widget *widget)
{
	int	i;

	if (widget->scroll.x < 0 || widget->scroll_mode == FREE_SCROLL)
		return (true);
	i = 0;
	while (widget->childs[i])
	{
		if (widget->childs[i]->position.x < 0)
			return (true);
		i++;
	}
	return (false);
}

bool	ui_widget_can_scroll_left(t_widget *widget)
{
	int	i;

	if (widget->scroll.x > 0 || widget->scroll_mode == FREE_SCROLL)
		return (true);
	i = 0;
	while (widget->childs[i])
	{
		if (widget->childs[i]->position.x + widget->childs[i]->width > widget->width)
			return (true);
		i++;
	}
	return (false);
}


void	ui_manage_wheel(t_widget *widget)
{
	
	int			i;

	if (widget->core->keys[SDL_SCANCODE_LCTRL] == true)
	{
		return ;
	}
	i = 0;
	if (widget->core->keys[SDL_SCANCODE_LSHIFT] == true)
	{
		if ((widget->core->event.wheel.y < 0 && ui_widget_can_scroll_left(widget)) || (widget->core->event.wheel.y > 0 && ui_widget_can_scroll_right(widget)))
		{
			while (widget->childs[i])
			{
				widget->childs[i]->position.x += UI_SCROLL_SPEED * widget->core->event.wheel.y;
				i++;
			}
			widget->scroll.x += UI_SCROLL_SPEED * widget->core->event.wheel.y;
			ui_window_set_is_dirty(widget->owning_window, true);
		}
	}
	else
	{
		if ((widget->core->event.wheel.y < 0 && ui_widget_can_scroll_up(widget)) || (widget->core->event.wheel.y > 0 && ui_widget_can_scroll_down(widget)))
		{
			while (widget->childs[i])
			{
				widget->childs[i]->position.y += UI_SCROLL_SPEED * widget->core->event.wheel.y;
				i++;
			}
			widget->scroll.y += UI_SCROLL_SPEED * widget->core->event.wheel.y;
			ui_window_set_is_dirty(widget->owning_window, true);
		}
	}
}

void		ui_widget_basic_render(t_widget *widget)
{
	SDL_Point	absolute;

	absolute = ui_get_absolute_position(widget);
	SDL_RenderCopy(widget->renderer, widget->texture, NULL, &(SDL_Rect){absolute.x, absolute.y, widget->width, widget->height});
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_box.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 21:09:13 by injah             #+#    #+#             */
/*   Updated: 2026/05/01 02:04:52 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

void	ui_set_box_mode(t_widget *box, e_box_mode mode)
{
	t_box_data	*data;

	data = (t_box_data *)box->data;
	data->mode = mode;
}

void	ui_build_wrap_box(t_widget *box, t_margin margin)
{
	int			i;
	SDL_Rect	measures;

	i = 0;
	measures.x = margin.left;
	measures.y = margin.top;
	measures.w = 0;
	measures.h = 0;
	while (box->childs[i])
	{
		if (measures.x + box->childs[i]->width + margin.left + margin.right > box->width)
		{
			measures.y = measures.h;
			measures.x = margin.left;
		}
		box->childs[i]->position.x = measures.x;
		box->childs[i]->position.y = measures.y;
		measures.x += box->childs[i]->width + margin.left + margin.right;
		if (measures.x > measures.w)
			measures.w = measures.x;
		if (measures.y + box->childs[i]->height + margin.top + margin.bottom > measures.h)
			measures.h = measures.y + box->childs[i]->height + margin.top + margin.bottom;
		i++;
	}
}

void	ui_build_center_child_box(t_widget *box, t_margin margin)
{
	int	i;
	int	start_y;
	int	y;
	int	total_height;
	(void)margin;
	i = 0;
	total_height = 0;
	while (box->childs[i])
	{
		total_height += box->childs[i]->height;
		i++;
	}
	i = 0;
	y = 0;
	start_y = box->height / 2 - total_height / 2;
	while (box->childs[i])
	{
		box->childs[i]->position.x = box->width / 2 - box->childs[i]->width / 2;
		box->childs[i]->position.y = start_y + y;
		y += box->childs[i]->height;
		if (box->childs[i]->type == TEXT)
			box->childs[i]->build(box->childs[i]);
		i++;
	}
}

void	ui_build_vertical_box(t_widget *box, t_margin margin)
{
	SDL_Rect	child_rect;
	int			i;
	int			y;

	child_rect.w = box->width - (margin.left + margin.right);
	child_rect.h = (box->height - (box->nb_child * (margin.top + margin.bottom))) / box->nb_child;
	y = margin.top;
	i = 0;
	while (box->childs[i])
	{
		box->childs[i]->width = child_rect.w;
		box->childs[i]->height = child_rect.h;
		box->childs[i]->position.x = margin.left;
		box->childs[i]->position.y = y;
		y += margin.top + margin.bottom + child_rect.h;
		i++;
	}
}

void	ui_build_horizontal_box(t_widget *box, t_margin margin)
{
	SDL_Rect	child_rect;
	int			x;
	int			i;

	child_rect.w = (box->width - (box->nb_child * (margin.left + margin.right))) / box->nb_child;
	child_rect.h = box->height - (margin.top + margin.bottom);
	x = margin.left;
	i = 0;
	while (box->childs[i])
	{
		box->childs[i]->width = child_rect.w;
		box->childs[i]->height = child_rect.h;
		box->childs[i]->position.x = x;
		box->childs[i]->position.y = margin.top;
		x += margin.left + margin.right + child_rect.w;
		i++;
	}
}

void	ui_build_stacking_box(t_widget *box, t_margin margin)
{
	int			i;

	i = 0;
	while (box->childs[i])
	{
		box->childs[i]->position.x = margin.left;
		box->childs[i]->position.y = margin.top;
		box->childs[i]->width = box->width - (margin.left + margin.right);
		box->childs[i]->height = box->height - (margin.top + margin.bottom);
		i++;
	}
}
		
void	ui_build_list_horizontal_box(t_widget *box, t_margin margin)
{
	int			i;
	int			child_height;
	int			x;

	i = 0;
	child_height = box->height - (margin.top + margin.bottom);
	x = margin.left;
	while (box->childs[i])
	{
		box->childs[i]->height = child_height;
		box->childs[i]->position.x = x;
		box->childs[i]->position.y = margin.top;
		x += box->childs[i]->width + margin.left + margin.right;
		i++;
	}
}

void	ui_build_list_vertical_box(t_widget *box, t_margin margin)
{
	int			i;
	int			child_width;
	int			y;

	i = 0;
	child_width = box->width - (margin.left + margin.right);
	y = margin.top;
	while (box->childs[i])
	{
		box->childs[i]->width = child_width;
		box->childs[i]->position.x = margin.left;
		box->childs[i]->position.y = y;
		y += box->childs[i]->height + margin.bottom + margin.top;
		i++;
	}
}

void	ui_box_event(t_widget *box)
{
	SDL_Point	absolute;
	if (box->core->event.type == SDL_MOUSEBUTTONDOWN)
	{
		absolute = ui_get_absolute_position(box);
		ui_widget_call_onclicked(box->core, box, box->core->mouse.position.x - absolute.x, box->core->mouse.position.y - absolute.y, box->core->event.button.button);
	}
	else if (box->core->event.type == SDL_MOUSEBUTTONUP)
	{
		absolute = ui_get_absolute_position(box);
		ui_widget_call_onreleased(box->core, box, box->core->mouse.position.x - absolute.x, box->core->mouse.position.y - absolute.y, box->core->event.button.button);
	}
}

void	ui_box_build(t_widget *box)
{
	t_box_data	*data;

	data = (t_box_data *)box->data;
	if (box->nb_child == 0)
		return ;
	if (data->mode == STACK_CHILDS)
		ui_build_stacking_box(box, data->margin);
	else if (data->mode == WRAP_CHILDS)
		ui_build_wrap_box(box, data->margin);
	else if (data->mode == EVEN_CHILDS_HORIZONTAL)
		ui_build_horizontal_box(box, data->margin);
	else if (data->mode == EVEN_CHILDS_VERTICAL)
		ui_build_vertical_box(box, data->margin);
	else if (data->mode == LIST_CHILDS_HORIZONTAL)
		ui_build_list_horizontal_box(box, data->margin);
	else if (data->mode == LIST_CHILDS_VERTICAL)
		ui_build_list_vertical_box(box, data->margin);
	else if (data->mode == CENTER_CHILDS)
		ui_build_center_child_box(box, data->margin);
	box->scroll.x = 0;
	box->scroll.y = 0;
	// else if (data->mode == CHILDS_FILL_WIDTH)
	// 	ui_keep_width_box(box, data->space);
	// else if (data->mode == CHILDS_ADAPT)
	// 	ui_childs_adapt_box(box);
}

t_widget	*ui_create_box(t_widget *parent, t_rect	relative_rect, e_box_mode mode, t_margin margin)
{
	t_widget	*box;
	t_box_data	*data;

	if (parent == NULL)
		return (NULL);
	box = ui_new_widget(parent, (SDL_Rect){relative_rect.x, relative_rect.y, relative_rect.width, relative_rect.height}, BOX, UI_MAX_BOX_CHILDS);
	if (!box)
		return (ui_destroy_widget(box), NULL);
	box->data = malloc(sizeof(t_box_data));
	if (!box->data)
		return (ui_destroy_widget(box), NULL);
	data = (t_box_data *)box->data;
	*data = (t_box_data){0};
	data->mode = mode;
	data->margin = margin;
	box->outline = 1;
	ft_strcpy(box->name, "box");
	box->cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	box->render = ui_widget_basic_render;
	box->build = ui_box_build;
	box->event = ui_box_event;
	box->texture = ui_new_texture(parent->renderer, 32, 32, box->colors[box->state]);
	ui_set_widget_colors(box, 0x7F5F5F5F, 0x7F5F5F5F, 0x7F5F5F5F);
	if (ui_add_child(parent, box) != UI_SUCCESS)
		return (ui_destroy_widget(box), NULL);
	return (box);
}
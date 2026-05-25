/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draws.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 02:17:57 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"

void	pencil(t_data *data, t_widget *layer, int x, int y, int spacing)
{
	int	dist;
	int	i;
	int	actual_x;
	int	actual_y;

	if (data->draw_infos.is_drawing == false && data->shift_pressed == false)
	{
			ft_printf("spacing_pix = %d spacing = %d\n", spacing, data->param[PENCIL].spacing);

		ui_draw_filled_rect(layer, (t_rect){x, y, data->param[PENCIL].thickness, data->param[PENCIL].thickness}, pack_color(data->color, data->brightness));
		data->draw_infos.is_drawing = true;
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
	}
	else
	{
		dist = distance(data->draw_infos.last_x, data->draw_infos.last_y, x, y);
		if (dist < spacing)
			return ;
		i = spacing;
		while (i <= dist)
		{
			ft_printf("spacing_pix = %d spacing = %d\n", spacing, data->param[PENCIL].spacing);
			actual_x = lerp(data->draw_infos.last_x, x, i, dist);
			actual_y = lerp(data->draw_infos.last_y, y, i, dist);
			ui_draw_filled_rect(layer, (t_rect){actual_x, actual_y, data->param[PENCIL].thickness, data->param[PENCIL].thickness}, pack_color(data->color, data->brightness));
			i += spacing;
		}
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
	}
}

void	brush(t_data *data, t_widget *layer, int x, int y, int spacing)
{
	int	dist;
	int	i;
	int	actual_x;
	int	actual_y;

	if (data->draw_infos.is_drawing == false && data->shift_pressed == false)
	{
		ui_draw_sticker(layer, data->wstickers.textures[data->active_sticker], (t_rect){x, y, data->param[BRUSH].width, data->param[BRUSH].height}, pack_color(data->color, data->brightness));
		data->draw_infos.is_drawing = true;
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
	}
	else
	{
		dist = distance(data->draw_infos.last_x, data->draw_infos.last_y, x, y);
		if (dist < spacing)
			return ;
		i = spacing;
		while (i <= dist)
		{
			actual_x = lerp(data->draw_infos.last_x, x, i, dist);
			actual_y = lerp(data->draw_infos.last_y, y, i, dist);
			ui_draw_sticker(layer, data->wstickers.textures[data->active_sticker], (t_rect){actual_x, actual_y, data->param[BRUSH].width, data->param[BRUSH].height}, pack_color(data->color, data->brightness));
			i += spacing;
		}
		data->draw_infos.last_x = actual_x;
		data->draw_infos.last_y = actual_y;
	}
}

void	filled_rect(t_data *data, t_widget *layer, int x, int y, int spacing)
{
	int	dist;
	int	i;
	int	actual_x;
	int	actual_y;

	if (data->draw_infos.is_drawing == false && data->shift_pressed == false)
	{
		ui_draw_filled_rect(layer, (t_rect){x, y, data->param[RECT].width, data->param[RECT].height}, pack_color(data->color, data->brightness));
		data->draw_infos.is_drawing = true;
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
	}
	else
	{
		dist = distance(data->draw_infos.last_x, data->draw_infos.last_y, x, y);
		if (dist < spacing)
			return ;
		i = spacing;
		while (i <= dist)
		{
			actual_x = lerp(data->draw_infos.last_x, x, i, dist);
			actual_y = lerp(data->draw_infos.last_y, y, i, dist);
			ui_draw_filled_rect(layer, (t_rect){actual_x, actual_y, data->param[RECT].width, data->param[RECT].height}, pack_color(data->color, data->brightness));
			i += spacing;
		}
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
	}
}

void	rect(t_data *data, t_widget *layer, int x, int y, int spacing)
{
	int	dist;
	int	i;
	int	actual_x;
	int	actual_y;

	if (data->draw_infos.is_drawing == false && data->shift_pressed == false)
	{
		ui_draw_rect(layer, (t_rect){x, y, data->param[RECT].width, data->param[RECT].height}, data->param[RECT].thickness, pack_color(data->color, data->brightness));
		data->draw_infos.is_drawing = true;
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
	}
	else
	{
		dist = distance(data->draw_infos.last_x, data->draw_infos.last_y, x, y);
		if (dist < spacing)
			return ;
		i = spacing;
		while (i <= dist)
		{
			actual_x = lerp(data->draw_infos.last_x, x, i, dist);
			actual_y = lerp(data->draw_infos.last_y, y, i, dist);
			ui_draw_rect(layer, (t_rect){actual_x, actual_y, data->param[RECT].width, data->param[RECT].height}, data->param[RECT].thickness, pack_color(data->color, data->brightness));
			i += spacing;
		}
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
	}
}

void	rect_select(t_data *data, t_widget *layer, int x, int y)
{	
	if (data->draw_infos.is_drawing == false)
	{
		ui_start_selection(data->wwindows.render, layer);
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
		data->draw_infos.is_drawing = true;
	}
}

void	rectangles(t_data *data, t_widget *layer, int x, int y, int spacing)
{
	if (ui_button_get_onoff_state(data->param[RECT].onoff_buttons[FILL_MODE].button) == false && ui_button_get_onoff_state(data->param[RECT].onoff_buttons[DRAW_MODE].button) == false)
		rect(data, layer, x - data->param[RECT].width / 2 , y - data->param[RECT].height / 2, spacing);
	else if (ui_button_get_onoff_state(data->param[RECT].onoff_buttons[FILL_MODE].button) == true && ui_button_get_onoff_state(data->param[RECT].onoff_buttons[DRAW_MODE].button) == false)
		filled_rect(data, layer, x - data->param[RECT].width / 2 , y - data->param[RECT].height / 2, spacing);
	else if (ui_button_get_onoff_state(data->param[RECT].onoff_buttons[DRAW_MODE].button) == true)
		rect_select(data, layer, x, y);
}

void	line(t_data *data, t_widget *layer, int x, int y)
{
	if (data->draw_infos.is_drawing == false)
	{
		ui_start_selection_line(data->wwindows.render, layer);
		data->draw_infos.last_x = x;
		data->draw_infos.last_y = y;
		data->draw_infos.is_drawing = true;
	}
}


void	circle(t_data *data, t_widget *layer, int x, int y, int spacing)
{
	if (ui_button_get_onoff_state(data->param[CIRCLE].onoff_buttons[DRAW_MODE].button) == true)
	{
		if (data->draw_infos.is_drawing == false)
		{
			ui_start_selection_circle(data->wwindows.render, layer);
			data->draw_infos.last_x = x;
			data->draw_infos.last_y = y;
			data->draw_infos.is_drawing = true;
		}
	}
	else
	{
		int	dist;
		int	i;
		int	actual_x;
		int	actual_y;
		if (data->draw_infos.is_drawing == false)
		{
			ui_draw_circle_moins_opti_mais_plus_beau(layer, x, y, data->param[CIRCLE].radius, data->param[CIRCLE].thickness, pack_color(data->color, data->brightness));
			data->draw_infos.is_drawing = true;
			data->draw_infos.last_x = x;
			data->draw_infos.last_y = y;
		}
		else
		{
			dist = distance(data->draw_infos.last_x, data->draw_infos.last_y, x, y);
			if (dist < spacing)
				return ;
			i = spacing;
			while (i <= dist)
			{
				actual_x = lerp(data->draw_infos.last_x, x, i, dist);
				actual_y = lerp(data->draw_infos.last_y, y, i, dist);
				ui_draw_circle_moins_opti_mais_plus_beau(layer, actual_x, actual_y, data->param[CIRCLE].radius, data->param[CIRCLE].thickness, pack_color(data->color, data->brightness));
				i += spacing;
			}
			data->draw_infos.last_x = x;
			data->draw_infos.last_y = y;
		}
	}
}

void	draw_on_layer(t_data *data, t_widget *layer, int x, int y)
{
	int	spacing_pix;

	spacing_pix = 1;
	if (data->active_tool == BRUSH || data->active_tool == RECT)
	{
		if (data->param[data->active_tool].width > data->param[data->active_tool].height)
			spacing_pix = 0.01f * data->param[data->active_tool].width * data->param[data->active_tool].spacing;
		else
			spacing_pix = 0.01f * data->param[data->active_tool].height * data->param[data->active_tool].spacing;
	}
	else if (data->active_tool == CIRCLE)
		spacing_pix = 0.01f * data->param[data->active_tool].radius * data->param[data->active_tool].spacing;
	else if (data->active_tool == PENCIL)
		spacing_pix = 0.01f * data->param[data->active_tool].thickness * data->param[data->active_tool].spacing;
	if (spacing_pix < 1)
		spacing_pix = 1;

	if (data->active_tool == RECT)
		rectangles(data, layer, x, y, spacing_pix);
	else if (data->active_tool == LINE)
		line(data, layer, x, y);
	else if (data->active_tool == BUCKET)
		ui_bucket_layer(layer, x, y, pack_color(data->color, data->brightness));
	else if (data->active_tool == PENCIL)
		pencil(data, layer, x - data->param[PENCIL].thickness / 2, y - data->param[PENCIL].thickness / 2, spacing_pix);
	else if (data->active_tool == CIRCLE)
		circle(data, layer, x, y, spacing_pix);
	else if (data->active_tool == ERASER)
		ui_erase_layer_sticker(layer, data->wstickers.textures[data->active_sticker], (t_rect){x - data->param[ERASER].width / 2, y - data->param[ERASER].height / 2, data->param[ERASER].width, data->param[ERASER].height}, data->param[ERASER].thickness);
	else if (data->active_tool == BRUSH)
		brush(data, layer, x - data->param[BRUSH].width / 2, y - data->param[BRUSH].height / 2, spacing_pix);
	else if (data->active_tool == SELECT && data->draw_infos.is_drawing == false)
	{
		ui_start_selection(data->wwindows.render, layer);
		data->draw_infos.is_drawing = true;
	}
}


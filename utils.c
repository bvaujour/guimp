/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:02:42 by injah             #+#    #+#             */
/*   Updated: 2026/05/04 14:11:29 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"

t_rgba	unpack_color(unsigned int color)
{
	t_rgba	unpacked_color;

	unpacked_color.a = (color >> 24) & 0xFF;
	unpacked_color.r = (color >> 16) & 0xFF;
	unpacked_color.g = (color >> 8) & 0xFF;
	unpacked_color.b = color & 0xFF;
	return (unpacked_color);
}

unsigned int pack_color(t_rgba color, float brightness)
{
	unsigned int	packed_color;

	color.r = brightness * color.r;
	color.g = brightness * color.g;
	color.b = brightness * color.b;
	packed_color = color.a << 24 | color.r << 16 | color.g << 8 | color.b;
	return (packed_color);
}

int	lerp(int a, int b, int step, int max)
{
	return (a + (b - a) * step / max);
}

int	distance(int x1, int y1, int x2, int y2)
{
	int	dist;
	int	delta_x;
	int	delta_y;
	
	delta_x = x1 - x2;
	delta_y = y1 - y2;
	dist = sqrt(delta_x * delta_x + delta_y * delta_y);
	return (dist);
}

void	set_active_tool(t_data *data, enum e_tool tool)
{
	int	i;

	i = 0;
	data->active_tool = tool;
	while (i < NUM_TOOL)
	{
		ui_set_widget_visibility(data->settings_boxs[i], false);
		ui_set_widget_outline(data->tool_buttons[i], 0);
		i++;
	}
	ui_set_widget_visibility(data->settings_boxs[tool], true);
	ui_set_widget_outline(data->tool_buttons[tool], 3);
	if (tool == SELECT)
		ui_set_text_str(data->tool_settings_container.title.text, "SELECTION SETTINGS");
	else if (tool == BRUSH)
		ui_set_text_str(data->tool_settings_container.title.text, "BRUSH SETTINGS");
	else if (tool == PENCIL)
		ui_set_text_str(data->tool_settings_container.title.text, "PENCIL SETTINGS");
	else if (tool == ERASER)
		ui_set_text_str(data->tool_settings_container.title.text, "ERASER SETTINGS");
	else if (tool == RECT)
		ui_set_text_str(data->tool_settings_container.title.text, "RECTANGLE SETTINGS");
	else if (tool == CIRCLE)
		ui_set_text_str(data->tool_settings_container.title.text, "CIRCLE SETTINGS");
	else if (tool == LINE)
		ui_set_text_str(data->tool_settings_container.title.text, "LINE SETTINGS");
	else if (tool == BUCKET)
		ui_set_text_str(data->tool_settings_container.title.text, "BUCKET SETTINGS");
}

void	add_sticker(t_data *data, const char *path)
{
	char	*str;

	if (data->nb_stickers == MAX_STICKERS)
	{
		data->wwindows.info = ui_create_prefab_modal_info_window(data->core, 500, 300, CENTER_CHILDS, "You Reached Max Stickers", 40);
		return ;
	}
	data->wstickers.textures[data->nb_stickers] = ui_create_image(data->wwindows.render, (t_rect){0, 0, 0, 0}, path);
	if (data->wstickers.textures[data->nb_stickers] == NULL)
	{
		str = ft_strjoin("Waring Sticker\nFailed open:\n", (char *)path, 0);
		data->wwindows.info = ui_create_prefab_modal_info_window(data->core, 500, 300, CENTER_CHILDS, str, 40);
		free(str);
		return ;
	}
	ui_set_widget_hit_testable(data->wstickers.textures[data->nb_stickers], false);
	data->wstickers.sticker_buttons[data->nb_stickers] = ui_create_button(data->wstickers.container.inner_box, (t_rect){0, 0, 50, 50});
	ui_set_widget_colors(data->wstickers.sticker_buttons[data->nb_stickers], 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	ui_set_widget_texture(data->wstickers.sticker_buttons[data->nb_stickers], path);
	data->nb_stickers++;
}

void	add_layer(t_data *data, const char *path, int index)
{
	char *str;
	data->wcanvas[data->active_canvas].layers[index] = ui_create_layer_from_image(data->wcanvas[data->active_canvas].canvas, path);
	if (data->wcanvas[data->active_canvas].layers[index])
	{
		ui_set_widget_drag_zone(data->wcanvas[data->active_canvas].layers[index], true, true, (t_rect){0, 0, 0, 0});
		data->wcanvas[data->active_canvas].layers_buttons[index] = ui_create_button(data->wcanvas[data->active_canvas].layers_buttons_box, (t_rect){0, 0, 160, 50});
		ui_widget_clone_texture(data->wcanvas[data->active_canvas].layers_buttons[index], data->wcanvas[data->active_canvas].layers[index]);
		set_active_layer(&data->wcanvas[data->active_canvas], index);
		data->wcanvas[data->active_canvas].nb_layer++;
	}
	else
	{
		str = ft_strjoin("Waring Layer\nFailed open:\n", (char *)path, 0);
		data->wwindows.info = ui_create_prefab_modal_info_window(data->core, 500, 300, CENTER_CHILDS, str, 40);
		free(str);
	}
}

void	add_empty_layer(t_data *data)
{
	int			i;

	i = 0;
	while (i < MAX_LAYERS)
	{
		if (data->wcanvas[data->active_canvas].layers[i] == NULL)
		{
			data->wcanvas[data->active_canvas].layers[i] = ui_create_layer(data->wcanvas[data->active_canvas].canvas);
			if (data->wcanvas[data->active_canvas].layers[i])
			{
				ui_set_widget_drag_zone(data->wcanvas[data->active_canvas].layers[i], true, true, (t_rect){0, 0, 0, 0});
				data->wcanvas[data->active_canvas].layers_buttons[i] = ui_create_button(data->wcanvas[data->active_canvas].layers_buttons_box, (t_rect){0, 0, 160, 50});
				ui_widget_clone_texture(data->wcanvas[data->active_canvas].layers_buttons[i], data->wcanvas[data->active_canvas].layers[i]);
				set_active_layer(&data->wcanvas[data->active_canvas], i);
				data->wcanvas[data->active_canvas].nb_layer++;
			}
			return ;
		}
		i++;	
	}
}

void	remove_layer(t_canvas_widgets *wcanvas, int index)
{
	int	i;

	i = index;
	if (wcanvas->nb_layer == 1)
		return ;
	ui_widget_remove_child(wcanvas->layers_buttons_box, wcanvas->layers_buttons[index]);
	ui_widget_remove_child(wcanvas->canvas, wcanvas->layers[index]);
	while (i < wcanvas->nb_layer - 1)
	{
		wcanvas->layers[i] = wcanvas->layers[i + 1];
		wcanvas->layers_buttons[i] = wcanvas->layers_buttons[i + 1];
		i++;
	}
	wcanvas->layers[i] = NULL;
	wcanvas->layers_buttons[i] = NULL;
	wcanvas->nb_layer--;
	if (wcanvas->active_layer > wcanvas->nb_layer - 1)
		wcanvas->active_layer = wcanvas->nb_layer - 1;
	ui_set_widget_outline(wcanvas->layers_buttons[wcanvas->active_layer], 2);
	ui_set_widget_outline(wcanvas->layers[wcanvas->active_layer], 3);
	ui_set_widget_hit_testable(wcanvas->layers[wcanvas->active_layer], true);
}

void	add_copy_layer(t_data *data, int x, int y)
{
	int			i;

	i = 0;
	while (i < MAX_LAYERS)
	{
		if (data->wcanvas[data->active_canvas].layers[i] == NULL)
		{
			data->wcanvas[data->active_canvas].layers[i] = ui_create_layer_from_copy_selection(data->wcanvas[data->active_canvas].canvas, x, y);
			if (data->wcanvas[data->active_canvas].layers[i])
			{
				ui_set_widget_drag_zone(data->wcanvas[data->active_canvas].layers[i], true, true, (t_rect){0, 0, 0, 0});
				data->wcanvas[data->active_canvas].layers_buttons[i] = ui_create_button(data->wcanvas[data->active_canvas].layers_buttons_box, (t_rect){0, 0, 160, 50});
				ui_widget_clone_texture(data->wcanvas[data->active_canvas].layers_buttons[i], data->wcanvas[data->active_canvas].layers[i]);
				set_active_layer(&data->wcanvas[data->active_canvas], i);
				data->wcanvas[data->active_canvas].nb_layer++;
			}
			return ;
		}
		i++;	
	}
}

void	set_active_layer(t_canvas_widgets *wcanvas, int index)
{
	ui_set_widget_outline(wcanvas->layers_buttons[wcanvas->active_layer], 1);
	ui_set_widget_outline(wcanvas->layers[wcanvas->active_layer], 1);
	ui_set_widget_hit_testable(wcanvas->layers[wcanvas->active_layer], false);
	wcanvas->active_layer = index;
	ui_set_widget_outline(wcanvas->layers_buttons[wcanvas->active_layer], 2);
	ui_set_widget_outline(wcanvas->layers[wcanvas->active_layer], 3);
	ui_set_widget_hit_testable(wcanvas->layers[wcanvas->active_layer], true);
}

void	set_active_sticker(t_data *data, int index)
{
	ui_set_widget_outline(data->wstickers.sticker_buttons[data->active_sticker], 0);
	data->active_sticker = index;
	ui_set_widget_outline(data->wstickers.sticker_buttons[data->active_sticker], 1);
	if (data->nb_canvas)
		ui_set_custom_cursor(data->wwindows.render, ui_image_get_image_path(data->wstickers.textures[data->active_sticker]), ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[BRUSH].width, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[BRUSH].height);
}

void	set_active_canvas(t_data *data, int index)
{
	ui_set_widget_visibility(data->wcanvas[data->active_canvas].container, false);
	ui_set_widget_visibility(data->wcanvas[index].container, true);
	ui_set_widget_outline(data->canvas_buttons[data->active_canvas], 0);
	ui_set_custom_cursor(data->wwindows.render, ui_image_get_image_path(data->wstickers.textures[data->active_sticker]), ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[BRUSH].width, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[BRUSH].height);
	data->active_canvas = index;
	ui_set_widget_outline(data->canvas_buttons[data->active_canvas], 1);
}


void	get_color_from_wheel(t_data *data, int x, int y)
{
	t_rgba			unpack;
	unsigned int	color;
	
	color = ui_image_get_pixel(data->wcolor.wheel, x, y);
	if (color != 0)
	{
		unpack = unpack_color(color);
		data->color.r = unpack.r;
		data->color.g = unpack.g;
		data->color.b = unpack.b;
		ui_set_slider_value(data->wcolor.sliders[RED].slider, (float)data->color.r / 255);
		ui_set_slider_value(data->wcolor.sliders[GREEN].slider, (float)data->color.g / 255);
		ui_set_slider_value(data->wcolor.sliders[BLUE].slider, (float)data->color.b / 255);
		ui_set_text_str_by_float(data->wcolor.sliders[RED].value_text.text, data->brightness * data->color.r, 1);
		ui_set_text_str_by_float(data->wcolor.sliders[GREEN].value_text.text, data->brightness * data->color.g, 1);
		ui_set_text_str_by_float(data->wcolor.sliders[BLUE].value_text.text, data->brightness * data->color.b, 1);
		ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));
	}
}
void	toggle_header_menus(t_data *data, int menu)
{
	int	i;

	i = 0;
	if (ui_get_widget_visibility(data->wheader.header.box[menu]) == true)
		close_header(data);
	else
	{
		while (i < NUM_HEADER)
		{
			ui_set_widget_visibility(data->wheader.header.box[i], false);
			ui_set_widget_outline(data->wheader.header.buttons[i].button, 1);
			i++;
		}
		ui_set_widget_visibility(data->edit_font_list.box, false);
		ui_toggle_widget_visibility(data->wheader.header.box[menu]);
		ui_set_widget_outline(data->wheader.header.buttons[menu].button, 2);
		data->header_opened = true;
	}
}

void	close_header(t_data *data)
{
	int	i;

	i = 0;
	while (i < NUM_HEADER)
	{
		ui_set_widget_visibility(data->wheader.header.box[i], false);
		ui_set_widget_outline(data->wheader.header.buttons[i].button, 1);
		i++;
	}
	ui_set_widget_visibility(data->edit_font_list.box, false);
	data->header_opened = false;
}


bool	is_header(t_data *data, t_widget *widget)
{
	int	i;

	i = 0;
	while (i < NUM_HEADER)
	{
		if (widget == data->wheader.header.buttons[i].button || widget == data->wheader.header.box[i])
			return (true);
		i++;
	}
	i = 0;
	while (i < NUM_FILE_MENU)
	{
		if (widget == data->wheader.file_menu_buttons[i].button)
			return (true);
		i++;
	}
	i = 0;
	while (i < NUM_EDIT_MENU)
	{
		if (widget == data->wheader.edit_menu_buttons[i].button)
			return (true);
		i++;
	}
	i = 0;
	while (i < NUM_HELP_MENU)
	{
		if (widget == data->wheader.help_menu_buttons[i].button)
			return (true);
		i++;
	}
	i = 0;
	while (i < NUM_FONT)
	{
		if (widget == data->edit_font_list.buttons[i].button)
			return (true);
		i++;
	}
	return (false);
}

// bool	is_tool_param_box(t_widget *widget, t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < NUM_TOOL)
// 	{
// 		if (widget == data->tool_parameters_boxes[i].box)
// 			return (true);
// 		i++;
// 	}
// 	return (false);
// }

bool	is_canvas(t_widget *widget, t_data *data)
{
	int	i;

	i = 0;
	if (widget == data->wcanvas[data->active_canvas].canvas || widget == data->wcanvas[data->active_canvas].render_box)
		return (true);
	while (i < data->wcanvas[data->active_canvas].nb_layer)
	{
		if (widget == data->wcanvas[data->active_canvas].layers[i])
			return (true);
		i++;
	}
	return (false);
}

// void	toggle_resizable_all(t_data *data, t_widget *widget, bool new_resizable)
// {
// 	int	i;

// 	if (widget == data->tool_buttons_box)
// 	{
// 		ui_set_widget_resizable(widget, true);
// 		ui_set_widget_resizable(data->container.box, new_resizable);
// 		i = 0;
// 		while (i < NUM_TOOL)
// 		{
// 			ui_set_widget_resizable(data->tool_parameters_boxes[i].box, new_resizable);
// 			i++;
// 		}
// 	}
// 	else if (widget == data->container.box)
// 	{
// 		ui_set_widget_resizable(widget, true);
// 		ui_set_widget_resizable(data->tool_buttons_box, new_resizable);
// 		i = 0;
// 		while (i < NUM_TOOL)
// 		{
// 			ui_set_widget_resizable(data->tool_parameters_boxes[i].box, new_resizable);
// 			i++;
// 		}
// 	}
// 	else if (is_tool_param_box(widget, data))
// 	{
// 		ui_set_widget_resizable(widget, true);
// 		ui_set_widget_resizable(data->container.box, new_resizable);
// 		ui_set_widget_resizable(data->tool_buttons_box, new_resizable);
// 	}
// 	else
// 	{
// 		ui_set_widget_resizable(data->tool_buttons_box, new_resizable);
// 		ui_set_widget_resizable(data->container.box, new_resizable);
// 		i = 0;
// 		while (i < NUM_TOOL)
// 		{
// 			ui_set_widget_resizable(data->tool_parameters_boxes[i].box, new_resizable);
// 			i++;
// 		}
// 	}
// }


void	handle_font_button_click(t_widget *widget, t_data *data)
{
	int	i;

	i = 0;
	while (i < NUM_FONT)
	{
		if (widget == data->edit_font_list.buttons[i].button)
		{
			data->selected_font = i;
			ui_set_font(data->core, data->edit_font_list.paths[i], 25);
			return ;
		}
		i++;
	}
}
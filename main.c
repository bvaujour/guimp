/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:53:38 by bvaujour          #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"

void	on_key_pressed(int key, void *param)
{
	t_data *data;

	data = (t_data *)param;
	if (key == UI_SCANCODE_ESCAPE)
		ui_quit(data->core);
	if (key == UI_SCANCODE_LCTRL)
		data->ctrl_pressed = true;
	if (key == UI_SCANCODE_LALT)
		data->alt_pressed = true;
	if (key == UI_SCANCODE_LSHIFT)
		data->shift_pressed = true;
	else if (key == UI_SCANCODE_Z && data->ctrl_pressed == true)
	{
		ui_layer_restore_previous_snapshot(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
	}
	else if (key == UI_SCANCODE_Y && data->ctrl_pressed == true)
	{
		ui_layer_restore_next_snapshot(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
	}
	else if (key == UI_SCANCODE_C && data->ctrl_pressed == true)
	{
		ui_copy_layer(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], ui_get_selection(data->wwindows.render));
		ui_stop_hide_selection(data->wwindows.render);
	}
	else if (key == UI_SCANCODE_X && data->ctrl_pressed == true)
	{
		ui_copy_layer(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], ui_get_selection(data->wwindows.render));
		ui_clear_layer_from_selection(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
		ui_stop_hide_selection(data->wwindows.render);
	}
	else if (key == UI_SCANCODE_V && data->ctrl_pressed == true)
	{
		add_copy_layer(data, 0, 0);
	}
	else if (key == UI_SCANCODE_E)
	{
		ui_canvas_export_layers_as_jpg(data->wcanvas[data->active_canvas].canvas, "test.jpg", 100);
	}
	else if (key == UI_SCANCODE_R)
	{
		ui_canvas_export_layers_as_png(data->wcanvas[data->active_canvas].canvas, "test.png");
	}
	else if (key == UI_SCANCODE_N)
	{
		ui_widget_downgrade(data->wcanvas[data->active_canvas].layers_buttons[data->wcanvas[data->active_canvas].active_layer]);
		ui_widget_downgrade(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
	}
	else if (key == UI_SCANCODE_M)
	{
		ui_widget_upgrade(data->wcanvas[data->active_canvas].layers_buttons[data->wcanvas[data->active_canvas].active_layer]);
		ui_widget_upgrade(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
	}
	else if (key == UI_SCANCODE_B)
		set_active_tool(data, BRUSH);
	else if (key == UI_SCANCODE_P)
		set_active_tool(data, PENCIL);
	else if (key == UI_SCANCODE_L)
		set_active_tool(data, LINE);
	else if (key == UI_SCANCODE_O)
		set_active_tool(data, CIRCLE);
	else if (key == UI_SCANCODE_U)
		set_active_tool(data, RECT);
	else if (key == UI_SCANCODE_I)
		set_active_tool(data, BUCKET);
	else if (key == UI_SCANCODE_T)
		set_active_tool(data, TEXT_TOOL);
		
	ft_printf("key %d pressed\n", key);
}


void	on_key_released(int key, void *param)
{
	t_data *data;

	data = (t_data *)param;
	if (key == UI_SCANCODE_LCTRL)
		data->ctrl_pressed = false;
	if (key == UI_SCANCODE_LALT)
		data->alt_pressed = false;
	if (key == UI_SCANCODE_LSHIFT)
		data->shift_pressed = false;
	ft_printf("key %d released lol\n", key);
}

void	on_widget_clicked(t_widget *widget, int button, int x, int y, void *param)
{
	t_data *data;

	data = (t_data *)param;
	// ft_printf("widget %p clicked with button %d at x:%d y:%d\n", widget, button, x, y);
	if (data->wcolor.pipette_active == true)
	{
		t_rgba			unpack;
		unsigned int	color;
		
		color = ui_get_pixel(widget, x, y);
		if (color != 0)
		{
			unpack = unpack_color(color);
			data->color.r = unpack.r;
			data->color.g = unpack.g;
			data->color.b = unpack.b;
			data->color.a = unpack.a;
			ui_set_slider_value(data->wcolor.sliders[RED].slider, (float)data->color.r / 255);
			ui_set_slider_value(data->wcolor.sliders[GREEN].slider, (float)data->color.g / 255);
			ui_set_slider_value(data->wcolor.sliders[BLUE].slider, (float)data->color.b / 255);
			ui_set_slider_value(data->wcolor.sliders[ALPHA].slider, (float)data->color.a / 255);
			ui_set_text_str_by_float(data->wcolor.sliders[RED].value_text.text, data->brightness * data->color.r, 1);
			ui_set_text_str_by_float(data->wcolor.sliders[GREEN].value_text.text, data->brightness * data->color.g, 1);
			ui_set_text_str_by_float(data->wcolor.sliders[BLUE].value_text.text, data->brightness * data->color.b, 1);
			ui_set_text_str_by_float(data->wcolor.sliders[ALPHA].value_text.text, data->brightness * data->color.a, 1);
			ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));
		}
		ui_set_use_override_cursor(data->core, false);
		data->wcolor.pipette_active = false;
	}
	else
	{
		if (is_header(data, widget) == false && data->header_opened)
		{
			close_header(data);
			ft_printf("close header\n");
		}
		if (button == 1)
		{
			if (widget == data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer])
				draw_on_layer(data, data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], x, y);
			else if (widget == data->wheader.header.buttons[FILE_BUTTON].button)
				toggle_header_menus(data, FILE_BUTTON);
			else if (widget == data->wheader.header.buttons[EDIT_BUTTON].button)
				toggle_header_menus(data, EDIT_BUTTON);
			else if (widget == data->wheader.header.buttons[HELP_BUTTON].button)
				toggle_header_menus(data, HELP_BUTTON);
			else if (widget == data->wheader.edit_menu_buttons[FONT_BUTTON].button)
				ui_toggle_widget_visibility(data->edit_font_list.box);
			else if (widget == data->wheader.edit_menu_buttons[CLEAR_BUTTON].button)
				ui_clear_layer(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], 0x00000000);
			else if (widget == data->wheader.file_menu_buttons[EXIT_BUTTON].button)
				ui_quit(data->core);
			else if (widget == data->wheader.edit_menu_buttons[COPY_BUTTON].button)
			{
				ui_copy_layer(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], ui_get_selection(data->wwindows.render));
				ui_stop_hide_selection(data->wwindows.render);
			}
			else if (widget == data->wheader.edit_menu_buttons[CUT_BUTTON].button)
			{
				ui_copy_layer(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], ui_get_selection(data->wwindows.render));
				ui_clear_layer_from_selection(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
				ui_stop_hide_selection(data->wwindows.render);
			}
			else if (widget == data->wheader.edit_menu_buttons[PASTE_BUTTON].button)
				add_copy_layer(data, 0, 0);
			else if (widget == data->wheader.file_menu_buttons[NEW_LAYER_BUTTON].button)
				add_empty_layer(data);
			else if (widget == data->wheader.file_menu_buttons[NEW_BUTTON].button)
				open_modal_new(data);
			else if (widget == data->tool_button_container.button1)
				ui_toggle_minimize_prefab_box(&data->tool_button_container);
			else if (widget == data->wstickers.container.button1)
				ui_toggle_minimize_prefab_box(&data->wstickers.container);
			else if (widget == data->wcolor.container.button1)
				ui_toggle_minimize_prefab_box(&data->wcolor.container);
			else if (widget == data->tool_settings_container.button1)
				ui_toggle_minimize_prefab_box(&data->tool_settings_container);
			else if (widget == data->wcolor.wheel)
				get_color_from_wheel(data, x, y);
			else if (widget == data->wheader.file_menu_buttons[EXPORT_FILE_BUTTON].button)
			{
				data->wwindows.export.window = ui_create_window(data->core, (t_rect){0, 0, 400, 200}, MODAL_ACCEPT_WINDOW, "EXPORT");
				data->wwindows.export.input = ui_create_prefab_input(data->wwindows.export.window, (t_rect){20, 70, 300, 38}, "Path:");
				data->wwindows.export.png_jpg_button = ui_create_prefab_onoff_button(data->wwindows.export.window, (t_rect){130, 110, 80, 30}, "Export to PNG", FIT);
			}
			else if (widget == data->wheader.file_menu_buttons[IMPORT_FILE_BUTTON].button)
			{
				data->wwindows.import.window = ui_create_window(data->core, (t_rect){0, 0, 400, 200}, MODAL_ACCEPT_WINDOW, "IMPORT");
				data->wwindows.import.input = ui_create_prefab_input(data->wwindows.import.window, (t_rect){20, 70, 300, 38}, "Path:");
			}
			else if (widget == data->param[RECT].onoff_buttons[FILL_MODE].button)
			{
				if (ui_button_get_onoff_state(data->param[RECT].onoff_buttons[FILL_MODE].button) == true)
					ui_set_text_str(data->param[RECT].onoff_buttons[FILL_MODE].text, "Fill Mode : Filled");
				else
					ui_set_text_str(data->param[RECT].onoff_buttons[FILL_MODE].text, "Fill Mode : Hollow");
			}
			else if (widget == data->param[RECT].onoff_buttons[DRAW_MODE].button)
			{
				if (ui_button_get_onoff_state(data->param[RECT].onoff_buttons[DRAW_MODE].button) == true)
					ui_set_text_str(data->param[RECT].onoff_buttons[DRAW_MODE].text, "Draw Mode : Mouse Selection Based");
				else
					ui_set_text_str(data->param[RECT].onoff_buttons[DRAW_MODE].text, "Draw Mode : Settings Based");
			}
			else if (widget == data->param[CIRCLE].onoff_buttons[DRAW_MODE].button)
			{
				if (ui_button_get_onoff_state(data->param[CIRCLE].onoff_buttons[DRAW_MODE].button) == true)
					ui_set_text_str(data->param[CIRCLE].onoff_buttons[DRAW_MODE].text, "Draw Mode : Mouse Selection Based");
				else
					ui_set_text_str(data->param[CIRCLE].onoff_buttons[DRAW_MODE].text, "Draw Mode : Settings Based");
			}
			else if (widget == data->wcanvas[data->active_canvas].upgrade_layer)
			{
				ui_widget_upgrade(data->wcanvas[data->active_canvas].layers_buttons[data->wcanvas[data->active_canvas].active_layer]);
				ui_widget_upgrade(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
			}
			else if (widget == data->wcanvas[data->active_canvas].downgrade_layer)
			{
				ui_widget_downgrade(data->wcanvas[data->active_canvas].layers_buttons[data->wcanvas[data->active_canvas].active_layer]);
				ui_widget_downgrade(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
			}
			else if (widget == data->wcanvas[data->active_canvas].add_layer)
				add_empty_layer(data);
			else if (widget == data->wcanvas[data->active_canvas].clear_layer)
				ui_clear_layer(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], 0x00000000);
			else if (widget == data->wcanvas[data->active_canvas].close_layer)
				remove_layer(&data->wcanvas[data->active_canvas], data->wcanvas[data->active_canvas].active_layer);
			else if (widget == data->wcolor.pipette)
			{
				ui_set_use_override_cursor(data->core, true);
				data->wcolor.pipette_active = true;
			}
			else if (widget == data->wheader.help_menu_buttons[ABOUT_BUTTON].button)
				open_about_modal(data);
			else if (widget == data->wcolor.pipette)
			{
				ui_set_use_override_cursor(data->core, true);
				data->wcolor.pipette_active = true;
			}
			else if (widget == data->wwindows.export.png_jpg_button.button)
			{
				if (ui_button_get_onoff_state(data->wwindows.export.png_jpg_button.button) == true)
					ui_set_text_str(data->wwindows.export.png_jpg_button.text, "Export to JPG");
				else
					ui_set_text_str(data->wwindows.export.png_jpg_button.text, "Export to PNG");
				return ;
			}
			else
			{
				handle_font_button_click(widget, data);
				int i;
	
				i = 0;
				while (i < data->nb_stickers)
				{
					if (widget == data->wstickers.sticker_buttons[i])
					{
						if (data->active_tool != BRUSH && data->active_tool != ERASER)
							set_active_tool(data, BRUSH);
						set_active_sticker(data, i);
						return ;
					}
					i++;
				}
				i = 0;
				while (i < data->wcanvas[data->active_canvas].nb_layer)
				{
					if (widget == data->wcanvas[data->active_canvas].layers_buttons[i])
					{
						set_active_layer(&data->wcanvas[data->active_canvas], i);
						return ;
					}
					i++;
				}
				i = 0;
				while (i < NUM_TOOL)
				{
					if (widget == data->tool_buttons[i])
					{
						set_active_tool(data, i);
						return ;
					}
					i++;
				}
				i = 0;
				while (i < data->nb_canvas)
				{
					if (widget == data->canvas_buttons[i])
					{
						set_active_canvas(data, i);
						return ;
					}
					i++;
				}
				i = 0;
				while (i < NUM_FONT)
				{
					if (widget == data->draw_text.font_list.buttons[i].button)
					{
						data->draw_text.font = data->draw_text.font_list.paths[i];
						ui_set_widget_outline(data->draw_text.font_list.buttons[data->draw_text.active_font].button, 0);
						ui_set_widget_outline(data->draw_text.font_list.buttons[i].button, 2);
						data->draw_text.active_font = i;
						return ;
					}
					i++;
				}
			}
		}
		else if (button == 3)
		{
			if (widget == data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer])
			{
				ui_set_widget_dragable(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], true);
			}
		}
		
	}
}


void	on_widget_released(t_widget *widget, int button, int x, int y, void *param)
{
	t_data *data;
	ft_printf("widget %p released with button %d at x:%d y:%d\n", widget, button, x, y);
	data = (t_data *)param;
	if (button == 1)
	{
		ui_stop_selection(data->wwindows.render);
		data->draw_infos.is_drawing = false;
		if (widget == data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer])
		{
			ui_layer_snapshot(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
			if (data->active_tool == RECT)
			{
				if (ui_button_get_onoff_state(data->param[RECT].onoff_buttons[DRAW_MODE].button) == true)
				{
					ui_stop_hide_selection(data->wwindows.render);
					int rect_x = data->draw_infos.last_x < x ? data->draw_infos.last_x : x;
					int rect_y = data->draw_infos.last_y < y ? data->draw_infos.last_y : y;
					int rect_width = abs(x - data->draw_infos.last_x);
					int rect_height = abs(y - data->draw_infos.last_y);
					if (ui_button_get_onoff_state(data->param[RECT].onoff_buttons[FILL_MODE].button) == true)
						ui_draw_filled_rect(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], (t_rect){rect_x, rect_y, rect_width, rect_height}, pack_color(data->color, data->brightness));
					else
						ui_draw_rect(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], (t_rect){rect_x, rect_y, rect_width, rect_height}, data->param[RECT].thickness , pack_color(data->color, data->brightness));
				}
			}
			else if (data->active_tool == LINE)
			{
				ui_stop_hide_selection(data->wwindows.render);
				// ui_draw_line(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], data->draw_infos.last_x, data->draw_infos.last_y, x, y, pack_color(data->color, data->brightness));
				ui_draw_line_thick(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], data->draw_infos.last_x, data->draw_infos.last_y, x, y, data->param[LINE].thickness,  pack_color(data->color, data->brightness));
				
			}
			else if (data->active_tool == CIRCLE)
			{
				if (ui_button_get_onoff_state(data->param[CIRCLE].onoff_buttons[DRAW_MODE].button) == true)
				{
					ui_stop_hide_selection(data->wwindows.render);
					int	delta_x;
					int	delta_y;
					int	center_x;
					int center_y;
					int	hypo;
					

					center_x = (x + data->draw_infos.last_x) / 2;
					center_y = (y + data->draw_infos.last_y) / 2;
					delta_x = abs(x - data->draw_infos.last_x);
					delta_y = abs(y - data->draw_infos.last_y);
					hypo = sqrt((delta_x * delta_x) + (delta_y * delta_y)) / 2;
					ui_draw_circle_moins_opti_mais_plus_beau(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], center_x, center_y, hypo, data->param[CIRCLE].thickness, pack_color(data->color, data->brightness));
				}
			}
		}
	}
	else if (button == 3)
	{
		if (widget == data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer])
		{
			ui_set_widget_dragable(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], false);
		}
	}
	// ui_set_widget_color(widget, 0xFF00FF00);
}

void	on_slider_change_value(t_widget *slider, float value, void *param)
{
	t_data *data;

	data = (t_data *)param;
	if (slider == data->param[data->active_tool].thickness_slider.slider)
	{
		data->param[data->active_tool].thickness = value * (MAX_THICKNESS - MIN_THICKNESS) + MIN_THICKNESS;
		ui_set_text_str_by_float(data->param[data->active_tool].thickness_slider.value_text.text, data->param[data->active_tool].thickness, 2);
	}
	else if (slider == data->param[data->active_tool].spacing_slider.slider)
	{
		data->param[data->active_tool].spacing = value * (MAX_SPACING - MIN_SPACING) + MIN_SPACING;
		ui_set_text_str_by_float(data->param[data->active_tool].spacing_slider.value_text.text, data->param[data->active_tool].spacing, 2);
	}
	else if (slider == data->param[data->active_tool].width_slider.slider)
	{
		data->param[data->active_tool].width = value * (MAX_WIDTH - MIN_WIDTH) + MIN_WIDTH;
		ui_set_text_str_by_float(data->param[data->active_tool].width_slider.value_text.text, data->param[data->active_tool].width, 2);
		ui_set_custom_cursor_size(data->wwindows.render, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[data->active_tool].width, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[data->active_tool].height);
	}
	else if (slider == data->param[data->active_tool].height_slider.slider)
	{
		data->param[data->active_tool].height = value * (MAX_HEIGHT - MIN_HEIGHT) + MIN_HEIGHT;
		ui_set_text_str_by_float(data->param[data->active_tool].height_slider.value_text.text, data->param[data->active_tool].height, 2);
		ui_set_custom_cursor_size(data->wwindows.render, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[data->active_tool].width, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[data->active_tool].height);
	}
	else if (slider == data->param[data->active_tool].radius_slider.slider)
	{
		data->param[data->active_tool].radius = value * (MAX_RADIUS - MIN_RADIUS) + MIN_RADIUS;
		ui_set_text_str_by_float(data->param[data->active_tool].radius_slider.value_text.text, data->param[data->active_tool].radius, 2);
	}
	else if (slider == data->wcolor.sliders[RED].slider)
	{
		data->color.r = value * 255;
		ui_set_text_str_by_float(data->wcolor.sliders[RED].value_text.text, data->brightness * data->color.r, 1);
		ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));
	}
	else if (slider == data->wcolor.sliders[GREEN].slider)
	{	
		data->color.g = value * 255;
		ui_set_text_str_by_float(data->wcolor.sliders[GREEN].value_text.text, data->brightness * data->color.g, 1);
		ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));
	}
	else if (slider == data->wcolor.sliders[BLUE].slider)
	{
		data->color.b = value * 255;
		ui_set_text_str_by_float(data->wcolor.sliders[BLUE].value_text.text, data->brightness * data->color.b, 1);
		ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));
	}
	else if (slider == data->wcolor.sliders[ALPHA].slider)
	{
		data->color.a = value * 255;
		ui_set_text_str_by_float(data->wcolor.sliders[ALPHA].value_text.text, data->color.a, 1);
		ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));
	}
	else if (slider == data->wcolor.sliders[BRIGHTNESS].slider)
	{
		data->brightness = value;
		ui_set_text_str_by_float(data->wcolor.sliders[BRIGHTNESS].value_text.text, data->brightness, 3);
		ui_set_text_str_by_float(data->wcolor.sliders[RED].value_text.text, data->brightness * data->color.r, 1);
		ui_set_text_str_by_float(data->wcolor.sliders[GREEN].value_text.text, data->brightness * data->color.g, 1);
		ui_set_text_str_by_float(data->wcolor.sliders[BLUE].value_text.text, data->brightness * data->color.b, 1);
		ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));

	}
}

void	on_widget_unhovered(t_widget *widget, void *param)
{
	t_data *data;
	// ft_printf("widget %p unhovered\n", widget);
	data = (t_data *)param;
	if (widget == data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer])
	{
		ui_stop_selection(data->wwindows.render);
		data->draw_infos.is_drawing = false;
	}
	(void)data;
}

void	on_widget_hovered(t_widget *widget, int x, int y, void *param)
{
	t_data *data;
	(void)x;
	(void)y;
	// ft_printf("widget %p hovered at x:%d y:%d\n", widget, x, y);
	data = (t_data *)param;
	if (data->wcolor.pipette_active)
		return ;
	ui_set_use_custom_cursor(data->wwindows.render, false);
	// if (is_header(data, widget) == false)
	// 	close_header(data);
	if (is_canvas(widget, data))
	{
		if (data->active_tool == BRUSH || data->active_tool == ERASER)
		{
			ui_set_use_custom_cursor(data->wwindows.render, true);
			if (data->active_tool == BRUSH)
				ui_set_custom_cursor_color(data->wwindows.render, 0xAFFFFFFF);
			else
				ui_set_custom_cursor_color(data->wwindows.render, 0x5FFFFFFF);
		}
		else
		{
			ui_set_use_custom_cursor(data->wwindows.render, false);
			if (data->active_tool == PENCIL)
				ui_set_widget_cursor_from_image(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], "assets/cursors/white.png", data->param[PENCIL].thickness * ui_layer_get_zoom(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]), data->param[PENCIL].thickness * ui_layer_get_zoom(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]), pack_color(data->color, data->brightness));
			else if (data->active_tool == RECT)
				ui_set_widget_cursor_as_arrow(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
			else if (data->active_tool == CIRCLE)
				ui_set_widget_cursor_as_arrow(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
			else if (data->active_tool == LINE)
				ui_set_widget_cursor_as_arrow(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
			else if (data->active_tool == BUCKET)
				ui_set_widget_cursor_as_arrow(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
		}
	}
	else if (widget == data->wheader.header.buttons[FILE_BUTTON].button)
	{
		if (ui_get_widget_visibility(data->wheader.header.box[EDIT_BUTTON]) == true)
			toggle_header_menus(data, FILE_BUTTON);
		else if (ui_get_widget_visibility(data->wheader.header.box[HELP_BUTTON]) == true)
			toggle_header_menus(data, FILE_BUTTON);
	}
	else if (widget == data->wheader.header.buttons[EDIT_BUTTON].button)
	{
		if (ui_get_widget_visibility(data->wheader.header.box[FILE_BUTTON]) == true)
			toggle_header_menus(data, EDIT_BUTTON);
		else if (ui_get_widget_visibility(data->wheader.header.box[HELP_BUTTON]) == true)
			toggle_header_menus(data, EDIT_BUTTON);
	}
	else if (widget == data->wheader.header.buttons[HELP_BUTTON].button)
	{
		if (ui_get_widget_visibility(data->wheader.header.box[FILE_BUTTON]) == true)
			toggle_header_menus(data, HELP_BUTTON);
		else if (ui_get_widget_visibility(data->wheader.header.box[EDIT_BUTTON]) == true)
			toggle_header_menus(data, HELP_BUTTON);
	}
	// toggle_resizable_all(data, widget, false);
}

void	on_input_validated(t_widget *input, const char *str, void *param)
{
	t_data *data;
	data = (t_data *)param;
	(void)data;
	(void)input;
	// (void)str;
	ft_printf("input validated: %s\n", str);
}

void	on_file_dropped(t_widget *widget, const char *path, void *param)
{
	t_data	*data;
	int		i;

	data = (t_data *)param;
	// ft_printf("file dropped: %s\n", path);
	if (is_canvas(widget, data))
	{
		i = 0;
		while (i < MAX_LAYERS)
		{
			if (data->wcanvas[data->active_canvas].layers[i] == NULL)
			{
				add_layer(data, path, i);
				return ;
			}
			i++;	
		}
	}
	else if (widget == data->wstickers.container.inner_box)
		add_sticker(data, path);
}

void	on_window_closed(t_widget *window, bool accepted, void *param)
{
	t_data *data;
	data = (t_data *)param;
	(void)data;
	ft_printf("window closed: %d\n", accepted);
	if (window == data->wwindows.export.window && accepted)
	{
		char	*str;

		str = ui_input_get_text(data->wwindows.export.input.input);
		if (ft_strlen(str) == 0)
		{
			ui_create_prefab_modal_info_window(data->core, 500, 300, CENTER_CHILDS, "Invalid Export Path", 40);
			return ;
		}
		if (ui_button_get_onoff_state(data->wwindows.export.png_jpg_button.button) == true)
			ui_canvas_export_layers_as_jpg(data->wcanvas[data->active_canvas].canvas, str, 100);
		else
			ui_canvas_export_layers_as_png(data->wcanvas[data->active_canvas].canvas, str);
		ui_close_window(data->core, data->wwindows.export.window);
	}
	else if (window == data->wwindows.import.window && accepted)
	{
		char	*str;

		str = ui_input_get_text(data->wwindows.import.input.input);
		if (ft_strlen(str) == 0)
		{
			ui_create_prefab_modal_info_window(data->core, 500, 300, CENTER_CHILDS, "Invalid Import Path", 40);
			return ;
		}
		
		add_layer(data, str, data->wcanvas[data->active_canvas].active_layer);
		ui_close_window(data->core, data->wwindows.import.window);
	}
	else if (window == data->modal_new.window && accepted)
	{
		int	width = ft_atoi(ui_input_get_text(data->modal_new.width.input));
		int	height = ft_atoi(ui_input_get_text(data->modal_new.height.input));
		if (width < 10)
			width = 10;
		else if (width > 4000)
			width = 4000;
		if (height < 10)
			height = 10;
		else if (height > 4000)
			height = 4000;
		new_canvas(data, width, height);
		ft_printf("width = %d height = %d\n", width, height);
		ui_close_window(data->core, data->modal_new.window);
	}
	else if (window == data->wwindows.render)
		ui_quit(data->core);
	else if (window == data->wwindows.tool)
		ui_close_window(data->core, data->wwindows.tool);
}

void	on_widget_wheeled(t_widget *widget, int value, void *param)
{
	t_data *data;
	data = (t_data *)param;
	(void)data;
	(void)widget;
	if (is_canvas(widget, data))
	{
		ui_canvas_zoom(data->wcanvas[data->active_canvas].canvas, value);
		build_canvas_infos(&data->wcanvas[data->active_canvas]);
		ui_set_custom_cursor_size(data->wwindows.render, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[data->active_tool].width, ui_canvas_get_zoom(data->wcanvas[data->active_canvas].canvas) * data->param[data->active_tool].height);

	}
	// ft_printf("widget wheeled: %p value:%d\n",widget, value);

}

void	on_input_changed(t_widget *input, char *str, void *param)
{
	t_data *data;
	data = (t_data *)param;
	(void)data;
	if (input == data->draw_text.input.input)
	{
		ui_layer_write(data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer], str, data->draw_text.font, data->draw_text.font_size, pack_color(data->color, data->brightness));
		ui_widget_clone_texture(data->wcanvas[data->active_canvas].layers_buttons[data->wcanvas[data->active_canvas].active_layer], data->wcanvas[data->active_canvas].layers[data->wcanvas[data->active_canvas].active_layer]);
	}
	if (input == data->draw_text.font_size_input.input)
	{
		int		font_size;

		if (str == NULL || !str[0])
			return ;
		font_size = ft_atoi(str);
		if (font_size >= 1 && font_size <= 500)
		data->draw_text.font_size = ft_atoi(str);
		if (data->draw_text.font_size < 1)
			data->draw_text.font_size = 1;
		else if (data->draw_text.font_size > 500)
			data->draw_text.font_size = 500;
	}
	ft_printf("input changed: %p str:%s\n", input, str);

}

void	base_init(t_data *data)
{
	//INIT //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	*data = (t_data){0};
	data->core = ui_init();
	data->color = (t_rgba){0, 0, 0, 255};
	data->param[PENCIL].thickness = MAX_THICKNESS;
	data->param[PENCIL].spacing = MAX_SPACING / 4;
	data->param[PENCIL].width = MAX_WIDTH;
	data->param[PENCIL].height = MAX_HEIGHT;
	data->param[BRUSH].thickness = MAX_THICKNESS;
	data->param[BRUSH].spacing = 50;
	data->param[BRUSH].width = 100;
	data->param[BRUSH].height = 100;
	data->param[RECT].thickness = 10;
	data->param[RECT].spacing = 100;
	data->param[RECT].width = 100;
	data->param[RECT].height = 100;
	data->param[ERASER].width = 100;
	data->param[ERASER].height = 100;
	data->param[ERASER].thickness = 100;
	data->param[CIRCLE].thickness = MAX_THICKNESS / 10;
	data->param[CIRCLE].radius = MAX_RADIUS / 2;
	data->param[CIRCLE].spacing = MAX_SPACING / 4;
	data->param[LINE].thickness = MAX_THICKNESS / 10;
	data->color.a = 255;
	data->color.r = 255;
	data->color.g = 255;
	data->color.b = 255;
	data->brightness = 1.0f;
	data->draw_text.font_size = 50;

	//BINDINGS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ui_bind_onkeypress(data->core, on_key_pressed, data);
	ui_bind_onkeyrelease(data->core, on_key_released, data);
	ui_bind_onsliderslided(data->core, on_slider_change_value, data);
	ui_bind_onwidgetclicked(data->core, on_widget_clicked, data);
	ui_bind_onwidgethovered(data->core, on_widget_hovered, data);
	ui_bind_onwidgetunhovered(data->core, on_widget_unhovered, data);
	ui_bind_onwidgetreleased(data->core, on_widget_released, data);
	ui_bind_oninputvalidated(data->core, on_input_validated, data);
	ui_bind_onfiledropped(data->core, on_file_dropped, data);
	ui_bind_onwindowclosed(data->core, on_window_closed, data);
	ui_bind_onwidgetwheeled(data->core, on_widget_wheeled, data);
	ui_bind_oninputchanged(data->core, on_input_changed, data);
	ui_set_override_cursor(data->core, "assets/cursors/pipette_cursor.png", 0, 49);

	//CREATE WINDOWS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ui_get_screen_size(&data->screen_width, &data->screen_height);
	data->wwindows.tool = ui_create_window(data->core, (t_rect){0, 0, TOOL_WINDOW_WIDTH, data->screen_height}, DEFAULT_WINDOW, "TOOLS");
	data->wwindows.render = ui_create_window(data->core, (t_rect){TOOL_WINDOW_WIDTH, 0, data->screen_width - TOOL_WINDOW_WIDTH, data->screen_height}, DEFAULT_WINDOW, "RENDER");


	data->tool_box = ui_create_box(data->wwindows.tool, (t_rect){0, 50, TOOL_WINDOW_WIDTH, data->screen_height - 100}, LIST_CHILDS_VERTICAL, (t_margin){20, 10, 0, 10});
	ui_set_widget_resizable(data->tool_box, true);
	ui_set_widget_dragable(data->tool_box, true);
	ui_set_widget_drag_zone(data->tool_box, true, true, (t_rect){0, 0, 0, 0});
	ui_set_widget_outline(data->tool_box, 0);
	//HEADER //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	data->wheader.header = ui_create_prefab_header(data->wwindows.tool, 10, 10, (const char *[]){"File", "Edit", "Help"}, 3);
	data->wheader.file_menu_buttons[NEW_BUTTON] = ui_create_prefab_button(data->wheader.header.box[FILE_BUTTON], (t_rect){0, 0, 0, 30}, "New", SINGLE_LINE);
	data->wheader.file_menu_buttons[NEW_LAYER_BUTTON] = ui_create_prefab_button(data->wheader.header.box[FILE_BUTTON], (t_rect){0, 0, 0, 30}, "New Layer", SINGLE_LINE);
	data->wheader.file_menu_buttons[IMPORT_FILE_BUTTON] = ui_create_prefab_button(data->wheader.header.box[FILE_BUTTON], (t_rect){0, 0, 0, 30}, "Import File", SINGLE_LINE);
	data->wheader.file_menu_buttons[EXPORT_FILE_BUTTON] = ui_create_prefab_button(data->wheader.header.box[FILE_BUTTON], (t_rect){0, 0, 0, 30}, "Export File", SINGLE_LINE);
	data->wheader.file_menu_buttons[EXIT_BUTTON] = ui_create_prefab_button(data->wheader.header.box[FILE_BUTTON], (t_rect){0, 90, 100, 30}, "Exit", SINGLE_LINE);
	
	data->wheader.edit_menu_buttons[COPY_BUTTON] = ui_create_prefab_button(data->wheader.header.box[EDIT_BUTTON], (t_rect){0, 0, 100, 30}, "Copy", SINGLE_LINE);
	data->wheader.edit_menu_buttons[PASTE_BUTTON] = ui_create_prefab_button(data->wheader.header.box[EDIT_BUTTON], (t_rect){0, 30, 100, 30}, "Paste", SINGLE_LINE);
	data->wheader.edit_menu_buttons[CUT_BUTTON] = ui_create_prefab_button(data->wheader.header.box[EDIT_BUTTON], (t_rect){0, 60, 100, 30}, "Cut", SINGLE_LINE);
	data->wheader.edit_menu_buttons[CLEAR_BUTTON] = ui_create_prefab_button(data->wheader.header.box[EDIT_BUTTON], (t_rect){0, 90, 100, 30}, "Clear", SINGLE_LINE);
	data->wheader.edit_menu_buttons[PARAMETER_BUTTON] = ui_create_prefab_button(data->wheader.header.box[EDIT_BUTTON], (t_rect){0, 120, 100, 30}, "Parameter", SINGLE_LINE);
	data->wheader.edit_menu_buttons[FONT_BUTTON] = ui_create_prefab_button(data->wheader.header.box[EDIT_BUTTON], (t_rect){0, 150, 120, 30}, "Font", SINGLE_LINE);
	
	data->wheader.help_menu_buttons[ABOUT_BUTTON] = ui_create_prefab_button(data->wheader.header.box[HELP_BUTTON], (t_rect){0, 0, 100, 30}, "About", SINGLE_LINE);
	
	//TOOL BUTTONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	data->tool_button_container = ui_create_prefab_box(data->tool_box, (t_rect){10, 10, TOOL_WINDOW_WIDTH - 20, 150}, WRAP_CHILDS, "TOOLS", (t_rect){0, 0, 300, 30});
	data->tool_buttons[SELECT] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 10, 40, 40});
	data->tool_buttons[BRUSH] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 10, 40, 40});
	data->tool_buttons[ERASER] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 60, 40, 40});
	data->tool_buttons[PENCIL] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 110, 40, 40});
	data->tool_buttons[RECT] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 160, 40, 40});
	data->tool_buttons[LINE] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 210, 40, 40});
	data->tool_buttons[CIRCLE] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 260, 40, 40});
	data->tool_buttons[BUCKET] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 310, 40, 40});
	data->tool_buttons[TEXT_TOOL] = ui_create_button(data->tool_button_container.inner_box, (t_rect){10, 360, 40, 40});

	//COLOR PARAMETER //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	data->wcolor.pipette_box = ui_create_box(data->tool_box, (t_rect){0, 0, 30, 30}, DEFAULT, (t_margin){0, 0, 0, 0});
	data->wcolor.pipette = ui_create_button(data->wcolor.pipette_box, (t_rect){0, 0, 30, 30});
	ui_set_widget_texture(data->wcolor.pipette,"assets/icons/pipette_button.png");
	
	data->wcolor.container = ui_create_prefab_box(data->tool_box, (t_rect){10, 130, TOOL_WINDOW_WIDTH - 20, 190}, EVEN_CHILDS_HORIZONTAL, "COLOR SETTINGS", (t_rect){0, 0, 300, 30});
	data->wcolor.sliders_box = ui_create_box(data->wcolor.container.inner_box, (t_rect){0, 0, 0, 0}, EVEN_CHILDS_VERTICAL, (t_margin){5, 2, 5, 2});
	data->wcolor.sliders[RED] = ui_create_prefab_slider(data->wcolor.sliders_box, (t_rect){30, 50, 200, 20}, "R", (data->brightness * data->color.r), 255);
	data->wcolor.sliders[GREEN] = ui_create_prefab_slider(data->wcolor.sliders_box, (t_rect){30, 90, 200, 20}, "G", (data->brightness * data->color.g), 255);
	data->wcolor.sliders[BLUE] = ui_create_prefab_slider(data->wcolor.sliders_box, (t_rect){30, 130, 200, 20}, "B", (data->brightness * data->color.b), 255);
	data->wcolor.sliders[ALPHA] = ui_create_prefab_slider(data->wcolor.sliders_box, (t_rect){30, 170, 200, 20}, "A", (data->brightness * data->color.a), 255);
	data->wcolor.sliders[BRIGHTNESS] = ui_create_prefab_slider(data->wcolor.sliders_box, (t_rect){30, 210, 200, 20}, "Brightness", data->brightness, 1.0f);
	data->wcolor.wheel = ui_create_image(data->wcolor.container.inner_box, (t_rect){10, 250, 140, 140}, "assets/images/color_wheel.png");
	ui_set_widget_outline(data->wcolor.wheel, 1);

	//STICKERS
	data->wstickers.container = ui_create_prefab_box(data->tool_box, (t_rect){0, 0, 0, 300}, WRAP_CHILDS, "STICKERS", (t_rect){0, 0, 300, 30});

	//TOOL PARAMETERS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	data->tool_settings_container = ui_create_prefab_box(data->tool_box, (t_rect){10, 490, TOOL_WINDOW_WIDTH - 20, 700}, STACK_CHILDS, "SETTINGS", (t_rect){0, 0, 300, 30});

	data->settings_boxs[SELECT] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[BRUSH] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[PENCIL] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[BUCKET] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[ERASER] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[RECT] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[CIRCLE] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[LINE] = ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	data->settings_boxs[TEXT_TOOL] =  ui_create_box(data->tool_settings_container.inner_box, (t_rect){0, 0, 0, 0}, LIST_CHILDS_VERTICAL, (t_margin){10, 2, 0, 2});
	
	
	data->param[PENCIL].thickness_slider = ui_create_prefab_slider(data->settings_boxs[PENCIL], (t_rect){10, 10, 300, 20}, "Thickness", data->param[PENCIL].thickness, MAX_THICKNESS);
	data->param[PENCIL].spacing_slider = ui_create_prefab_slider(data->settings_boxs[PENCIL], (t_rect){10, 40, 300, 20}, "Spacing", data->param[PENCIL].spacing, MAX_SPACING);
	data->param[BRUSH].width_slider = ui_create_prefab_slider(data->settings_boxs[BRUSH], (t_rect){10, 10, 300, 20}, "Width", data->param[BRUSH].width, MAX_WIDTH);
	data->param[BRUSH].height_slider = ui_create_prefab_slider(data->settings_boxs[BRUSH], (t_rect){10, 40, 300, 20}, "Height", data->param[BRUSH].height, MAX_HEIGHT);
	data->param[BRUSH].spacing_slider = ui_create_prefab_slider(data->settings_boxs[BRUSH], (t_rect){10, 70, 300, 20}, "Spacing", data->param[BRUSH].spacing, MAX_SPACING);
	data->param[LINE].thickness_slider = ui_create_prefab_slider(data->settings_boxs[LINE], (t_rect){10, 10, 300, 20}, "Thickness", data->param[LINE].thickness, MAX_THICKNESS);
	data->param[RECT].thickness_slider = ui_create_prefab_slider(data->settings_boxs[RECT], (t_rect){10, 10, 300, 20}, "Thickness", data->param[RECT].thickness, MAX_THICKNESS);
	data->param[RECT].width_slider = ui_create_prefab_slider(data->settings_boxs[RECT], (t_rect){10, 40, 300, 20}, "Width", data->param[RECT].width, MAX_WIDTH);
	data->param[RECT].height_slider = ui_create_prefab_slider(data->settings_boxs[RECT], (t_rect){10, 70, 300, 20}, "Height", data->param[RECT].height, MAX_HEIGHT);
	data->param[RECT].spacing_slider = ui_create_prefab_slider(data->settings_boxs[RECT], (t_rect){10, 100, 300, 20}, "Spacing", data->param[RECT].spacing, MAX_SPACING);
	data->param[RECT].onoff_buttons[FILL_MODE] = ui_create_prefab_onoff_button(data->settings_boxs[RECT], (t_rect){10, 130, 50, 30}, "Fill Mode : Hollow", SINGLE_LINE);
	data->param[RECT].onoff_buttons[DRAW_MODE] = ui_create_prefab_onoff_button(data->settings_boxs[RECT], (t_rect){10, 170, 50, 30}, "Draw Mode : Settings Based", SINGLE_LINE);
	data->param[ERASER].width_slider = ui_create_prefab_slider(data->settings_boxs[ERASER], (t_rect){10, 10, 300, 20}, "Width", data->param[ERASER].width, MAX_WIDTH);
	data->param[ERASER].height_slider = ui_create_prefab_slider(data->settings_boxs[ERASER], (t_rect){10, 40, 300, 20}, "Height", data->param[ERASER].height, MAX_HEIGHT);
	data->param[ERASER].thickness_slider = ui_create_prefab_slider(data->settings_boxs[ERASER], (t_rect){10, 70, 300, 20}, "Force", data->param[ERASER].thickness, MAX_THICKNESS);
	data->param[CIRCLE].thickness_slider = ui_create_prefab_slider(data->settings_boxs[CIRCLE], (t_rect){10, 10, 300, 20}, "Thickness", data->param[CIRCLE].thickness, MAX_THICKNESS);
	data->param[CIRCLE].radius_slider = ui_create_prefab_slider(data->settings_boxs[CIRCLE], (t_rect){10, 40, 300, 20}, "Radius", data->param[CIRCLE].radius, MAX_RADIUS);
	data->param[CIRCLE].spacing_slider = ui_create_prefab_slider(data->settings_boxs[CIRCLE], (t_rect){10, 70, 300, 20}, "Spacing", data->param[CIRCLE].spacing, MAX_SPACING);
	data->param[CIRCLE].onoff_buttons[DRAW_MODE] = ui_create_prefab_onoff_button(data->settings_boxs[CIRCLE], (t_rect){10, 100, 50, 30}, "Draw Mode : Settings Based", SINGLE_LINE);

	data->draw_text.input = ui_create_prefab_input(data->settings_boxs[TEXT_TOOL], (t_rect){0, 0, 380, 32}, "Text:");
	data->draw_text.font_size_input = ui_create_prefab_input(data->settings_boxs[TEXT_TOOL], (t_rect){0, 0, 150, 32}, "Font size:");
	data->draw_text.font_list = ui_create_prefab_font_list(data->settings_boxs[TEXT_TOOL], (t_rect){0, 0, 150, 100});
	ui_set_widget_outline(data->draw_text.font_list.buttons[0].button, 2);
	data->draw_text.font = data->draw_text.font_list.paths[0];
	char *str;
	
	str = ft_itoa(data->draw_text.font_size);
	ui_input_set_text(data->draw_text.font_size_input.input, str);
	free(str);
	//CANVAS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int	width;

	ui_get_widget_size(data->wwindows.render, &width, NULL);
	data->canvas_buttons_box = ui_create_box(data->wwindows.render, (t_rect){0, 0, width, 50}, LIST_CHILDS_HORIZONTAL, (t_margin){15, 2, 0, 2});

	//SOME AFTER CREATION SETTINGS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	t_rect	font_button_rect = ui_get_widget_absolute_rect(data->wheader.edit_menu_buttons[FONT_BUTTON].button);
	data->edit_font_list = ui_create_prefab_font_list(data->wwindows.tool, (t_rect){font_button_rect.x + font_button_rect.width, font_button_rect.y, 200, 200});

	
	ui_set_widget_texture(data->tool_buttons[PENCIL], "assets/icons/pencil0.png");
	ui_set_widget_texture(data->tool_buttons[RECT], "assets/icons/rectangle0.png");
	ui_set_widget_texture(data->tool_buttons[LINE], "assets/icons/line.jpg");
	ui_set_widget_texture(data->tool_buttons[CIRCLE], "assets/icons/circle0.png");
	ui_set_widget_texture(data->tool_buttons[BRUSH], "assets/icons/brush0.png");
	ui_set_widget_texture(data->tool_buttons[BUCKET], "assets/icons/bucket0.png");
	ui_set_widget_texture(data->tool_buttons[ERASER], "assets/icons/eraser0.png");
	ui_set_widget_texture(data->tool_buttons[TEXT_TOOL], "assets/icons/text.jpg");
	ui_set_widget_texture(data->tool_buttons[SELECT], "assets/icons/selection.jpg");

	add_sticker(data, "assets/stickers/brush0.png");
	add_sticker(data, "assets/stickers/brush1.png");
	add_sticker(data, "assets/stickers/brush2.png");
	add_sticker(data, "assets/stickers/brush3.png");
	add_sticker(data, "assets/stickers/brush4.png");
	add_sticker(data, "assets/stickers/spray0.png");
	add_sticker(data, "assets/stickers/spray1.png");
	add_sticker(data, "assets/stickers/spray2.png");
	add_sticker(data, "assets/stickers/spray3.png");
	add_sticker(data, "assets/stickers/spray4.png");
	add_sticker(data, "assets/stickers/Wilber.png");
	add_sticker(data, "assets/stickers/stormtrooper.png");
	add_sticker(data, "assets/stickers/murloc.png");
	add_sticker(data, "assets/stickers/intense-flame-texture.png");
	close_header(data);
	set_active_tool(data, BRUSH);
	set_active_sticker(data, 0);
	new_canvas(data, 800, 600);
	ui_set_widget_colors(data->wcolor.pipette_box, pack_color(data->color, data->brightness), pack_color(data->color, data->brightness), pack_color(data->color, data->brightness));
	ui_create_prefab_modal_info_window(data->core, 500, 300, CENTER_CHILDS, "Guimp (42 school project)\nFrom\nBruno Vaujour\nXavier Charle\nMatiya Deruelle", 25);
}

void	style_init(t_data *data, t_style *style)
{
	base_init(data);
	apply_css_style(data, style);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_style	style;

	if (argc > 2)
		return ft_printf("Usage: ./guimp [style.css]\n") * 0;
	if (argc == 2)
	{
		if (!parse_css_file(argv[1], &style))
		{
			ft_printf("guimp: could not parse '%s', using default style\n", argv[1]);
			base_init(&data);
		}
		else
			style_init(&data, &style);
	}
	else
		base_init(&data);
	ui_run(data.core);
	return (0);
}

// int	main()
// {
// 	t_data	data;

// 	data = (t_data){0};
// 	data.core = ui_init();
// 	ui_bind_onkeypress(data.core, on_key_pressed, &data);
// 	ui_get_screen_size(&data.screen_width, &data.screen_height);
// 	data.wwindows.tool = ui_create_window(data.core, (t_rect){0, 0, data.screen_width, data.screen_height}, DEFAULT_WINDOW, "TEST");
// 	data.wheader.header = ui_create_prefab_header(data.wwindows.tool, 10, 10, (const char *[]){"File", "Edit", "Help"}, 3);

// 	ui_run(data.core);
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   css_apply.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 00:00:00 by injah             #+#    #+#             */
/*   Updated: 2026/04/30 18:51:25 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"

/* ── style application ───────────────────────────────────────────────────── */

static void	apply_buttons(t_data *data, t_style *style)
{
	unsigned int	normal;
	unsigned int	hover;
	unsigned int	click;
	int				i;

	normal = (style->flags & STYLE_BUTTON_NORMAL) ? style->button_normal : 0x00000000;
	hover = (style->flags & STYLE_BUTTON_HOVER) ? style->button_hover : normal;
	click = (style->flags & STYLE_BUTTON_CLICK) ? style->button_click : normal;
	i = 0;
	while (i < NUM_TOOL)
	{
		ui_set_widget_normal_color(data->tool_buttons[i], normal);
		ui_set_widget_hovered_color(data->tool_buttons[i], hover);
		ui_set_widget_clicked_color(data->tool_buttons[i], click);
		i++;
	}
	i = 0;
	while (i < NUM_HEADER)
	{
		ui_set_widget_normal_color(data->wheader.header.buttons[i].button, normal);
		ui_set_widget_hovered_color(data->wheader.header.buttons[i].button, hover);
		ui_set_widget_clicked_color(data->wheader.header.buttons[i].button, click);
		i++;
	}
}

static void	apply_sliders(t_data *data, unsigned int color)
{
	int	i;

	i = 0;
	while (i < NUM_COLOR)
		ui_slider_set_fill_color(data->wcolor.sliders[i++].slider, color);
	i = 0;
	while (i < NUM_TOOL)
	{
		if (data->param[i].thickness_slider.slider)
			ui_slider_set_fill_color(data->param[i].thickness_slider.slider, color);
		if (data->param[i].spacing_slider.slider)
			ui_slider_set_fill_color(data->param[i].spacing_slider.slider, color);
		if (data->param[i].width_slider.slider)
			ui_slider_set_fill_color(data->param[i].width_slider.slider, color);
		if (data->param[i].height_slider.slider)
			ui_slider_set_fill_color(data->param[i].height_slider.slider, color);
		i++;
	}
}

static void	apply_draw_color(t_data *data, t_style *style)
{
	if (style->flags & STYLE_COLOR_R)
		data->color.r = style->draw_color.r;
	if (style->flags & STYLE_COLOR_G)
		data->color.g = style->draw_color.g;
	if (style->flags & STYLE_COLOR_B)
		data->color.b = style->draw_color.b;
	if (style->flags & STYLE_COLOR_A)
		data->color.a = style->draw_color.a;
	if (style->flags & STYLE_BRIGHTNESS)
		data->brightness = style->brightness;
	ui_set_slider_value(data->wcolor.sliders[RED].slider, (float)data->color.r / 255.0f);
	ui_set_slider_value(data->wcolor.sliders[GREEN].slider, (float)data->color.g / 255.0f);
	ui_set_slider_value(data->wcolor.sliders[BLUE].slider, (float)data->color.b / 255.0f);
	ui_set_slider_value(data->wcolor.sliders[ALPHA].slider, (float)data->color.a / 255.0f);
	ui_set_slider_value(data->wcolor.sliders[BRIGHTNESS].slider, data->brightness);
	ui_set_text_str_by_float(data->wcolor.sliders[RED].value_text.text, data->brightness * data->color.r, 1);
	ui_set_text_str_by_float(data->wcolor.sliders[GREEN].value_text.text, data->brightness * data->color.g, 1);
	ui_set_text_str_by_float(data->wcolor.sliders[BLUE].value_text.text, data->brightness * data->color.b, 1);
	ui_set_text_str_by_float(data->wcolor.sliders[ALPHA].value_text.text, data->color.a, 1);
	ui_set_text_str_by_float(data->wcolor.sliders[BRIGHTNESS].value_text.text, data->brightness, 3);
}

static void	apply_button_outlines(t_data *data, int thickness)
{
	int	i;

	i = 0;
	while (i < NUM_TOOL)
		ui_set_widget_outline(data->tool_buttons[i++], thickness);
	i = 0;
	while (i < NUM_HEADER)
	{
		ui_set_widget_outline(data->wheader.header.buttons[i].button, thickness);
		i++;
	}
	i = 0;
	while (i < NUM_FILE_MENU)
		ui_set_widget_outline(data->wheader.file_menu_buttons[i++].button, thickness);
	i = 0;
	while (i < NUM_EDIT_MENU)
		ui_set_widget_outline(data->wheader.edit_menu_buttons[i++].button, thickness);
	i = 0;
	while (i < NUM_HELP_MENU)
		ui_set_widget_outline(data->wheader.help_menu_buttons[i++].button, thickness);
}

static void	apply_box_outlines(t_data *data, int thickness)
{
	int	i;

	i = 0;
	while (i < NUM_TOOL)
	{
		if (data->settings_boxs[i])
			ui_set_widget_outline(data->settings_boxs[i], thickness);
		i++;
	}
	if (data->wcolor.container.box)
		ui_set_widget_outline(data->wcolor.container.box, thickness);
}

static void	apply_slider_outlines(t_data *data, int thickness)
{
	int	i;

	i = 0;
	while (i < NUM_COLOR)
		ui_set_widget_outline(data->wcolor.sliders[i++].slider, thickness);
	i = 0;
	while (i < NUM_TOOL)
	{
		if (data->param[i].thickness_slider.slider)
			ui_set_widget_outline(data->param[i].thickness_slider.slider, thickness);
		if (data->param[i].spacing_slider.slider)
			ui_set_widget_outline(data->param[i].spacing_slider.slider, thickness);
		if (data->param[i].width_slider.slider)
			ui_set_widget_outline(data->param[i].width_slider.slider, thickness);
		if (data->param[i].height_slider.slider)
			ui_set_widget_outline(data->param[i].height_slider.slider, thickness);
		i++;
	}
}

static void	apply_text_colors(t_data *data, unsigned int color)
{
	int	i;

	i = 0;
	while (i < NUM_COLOR)
	{
		if (data->wcolor.sliders[i].name_text.text)
			ui_set_text_font_color(data->wcolor.sliders[i].name_text.text, color);
		if (data->wcolor.sliders[i].value_text.text)
			ui_set_text_font_color(data->wcolor.sliders[i].value_text.text, color);
		i++;
	}
	i = 0;
	while (i < NUM_HEADER)
	{
		if (data->wheader.header.buttons[i].text)
			ui_set_text_font_color(data->wheader.header.buttons[i].text, color);
		i++;
	}
	i = 0;
	while (i < NUM_FILE_MENU)
	{
		if (data->wheader.file_menu_buttons[i].text)
			ui_set_text_font_color(data->wheader.file_menu_buttons[i].text, color);
		i++;
	}
	i = 0;
	while (i < NUM_EDIT_MENU)
	{
		if (data->wheader.edit_menu_buttons[i].text)
			ui_set_text_font_color(data->wheader.edit_menu_buttons[i].text, color);
		i++;
	}
	i = 0;
	while (i < NUM_HELP_MENU)
	{
		if (data->wheader.help_menu_buttons[i].text)
			ui_set_text_font_color(data->wheader.help_menu_buttons[i].text, color);
		i++;
	}
	i = 0;
	if (data->tool_settings_container.title.text)
		ui_set_text_font_color(data->tool_settings_container.title.text, color);
	while (i < NUM_TOOL)
	{
		if (data->param[i].thickness_slider.name_text.text)
			ui_set_text_font_color(data->param[i].thickness_slider.name_text.text, color);
		if (data->param[i].spacing_slider.name_text.text)
			ui_set_text_font_color(data->param[i].spacing_slider.name_text.text, color);
		if (data->param[i].width_slider.name_text.text)
			ui_set_text_font_color(data->param[i].width_slider.name_text.text, color);
		if (data->param[i].height_slider.name_text.text)
			ui_set_text_font_color(data->param[i].height_slider.name_text.text, color);
		i++;
	}
	if (data->wcolor.container.title.text)
		ui_set_text_font_color(data->wcolor.container.title.text, color);
}

void	apply_css_style(t_data *data, t_style *style)
{
	if (style->flags & STYLE_WINDOW_BG)
	{
		ui_set_widget_normal_color(data->wwindows.tool, style->window_bg);
		ui_set_widget_hovered_color(data->wwindows.tool, style->window_bg);
		ui_set_widget_clicked_color(data->wwindows.tool, style->window_bg);
		ui_set_widget_normal_color(data->wwindows.render, style->window_bg);
		ui_set_widget_hovered_color(data->wwindows.render, style->window_bg);
		ui_set_widget_clicked_color(data->wwindows.render, style->window_bg);
	}
	if (style->flags & (STYLE_BUTTON_NORMAL | STYLE_BUTTON_HOVER | STYLE_BUTTON_CLICK))
		apply_buttons(data, style);
	if (style->flags & STYLE_BUTTON_OUTLINE)
		apply_button_outlines(data, style->button_outline);
	if (style->flags & STYLE_BOX_OUTLINE)
		apply_box_outlines(data, style->box_outline);
	if (style->flags & STYLE_SLIDER_OUTLINE)
		apply_slider_outlines(data, style->slider_outline);
	if (style->flags & STYLE_TEXT_COLOR)
		apply_text_colors(data, style->text_color);
	if (style->flags & STYLE_SLIDER_FILL)
		apply_sliders(data, style->slider_fill);
	if (style->flags & (STYLE_COLOR_R | STYLE_COLOR_G | STYLE_COLOR_B
			| STYLE_COLOR_A | STYLE_BRIGHTNESS))
		apply_draw_color(data, style);
}

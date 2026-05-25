/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   css_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 00:00:00 by injah             #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"
#include <string.h>

/* ── helpers ────────────────────────────────────────────────────────────── */

static char	*skip_ws(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

static void	rtrim(char *s)
{
	int	len;

	len = (int)strlen(s);
	while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t'
			|| s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ';'))
		s[--len] = '\0';
}

/* Parse up to 8 hex digits (no prefix) into an unsigned int */
static unsigned int	hex_to_uint(const char *s, int n)
{
	unsigned int	result;
	char			c;

	result = 0;
	while (n--)
	{
		c = *s++;
		result <<= 4;
		if (c >= '0' && c <= '9')
			result |= (unsigned int)(c - '0');
		else if (c >= 'a' && c <= 'f')
			result |= (unsigned int)(c - 'a' + 10);
		else if (c >= 'A' && c <= 'F')
			result |= (unsigned int)(c - 'A' + 10);
	}
	return (result);
}

/*
** Colors in the CSS file use AARRGGBB order (same as libui's pack_color).
**   #RRGGBB   → 0xFFRRGGBB  (alpha defaults to FF = fully opaque)
**   #AARRGGBB → 0xAARRGGBB
*/
static unsigned int	parse_hex_color(const char *s)
{
	int	len;

	s = skip_ws((char *)s);
	if (*s == '#')
		s++;
	len = 0;
	while ((s[len] >= '0' && s[len] <= '9') || (s[len] >= 'a' && s[len] <= 'f')
		|| (s[len] >= 'A' && s[len] <= 'F'))
		len++;
	if (len == 6)
		return (0xFF000000 | hex_to_uint(s, 6));
	if (len == 8)
		return (hex_to_uint(s, 8));
	return (0);
}

/* ── per-selector property handlers ─────────────────────────────────────── */

static void	parse_window_prop(t_style *style, const char *prop, char *val)
{
	if (strcmp(prop, "background-color") == 0)
	{
		style->window_bg = parse_hex_color(val);
		style->flags |= STYLE_WINDOW_BG;
	}
}

static void	parse_button_prop(t_style *style, const char *prop, char *val)
{
	if (strcmp(prop, "background-color") == 0)
	{
		style->button_normal = parse_hex_color(val);
		style->flags |= STYLE_BUTTON_NORMAL;
	}
	else if (strcmp(prop, "hover-color") == 0)
	{
		style->button_hover = parse_hex_color(val);
		style->flags |= STYLE_BUTTON_HOVER;
	}
	else if (strcmp(prop, "click-color") == 0)
	{
		style->button_click = parse_hex_color(val);
		style->flags |= STYLE_BUTTON_CLICK;
	}
}

static void	parse_color_prop(t_style *style, const char *prop, char *val)
{
	if (strcmp(prop, "r") == 0)
	{
		style->draw_color.r = (unsigned char)ft_atoi(val);
		style->flags |= STYLE_COLOR_R;
	}
	else if (strcmp(prop, "g") == 0)
	{
		style->draw_color.g = (unsigned char)ft_atoi(val);
		style->flags |= STYLE_COLOR_G;
	}
	else if (strcmp(prop, "b") == 0)
	{
		style->draw_color.b = (unsigned char)ft_atoi(val);
		style->flags |= STYLE_COLOR_B;
	}
	else if (strcmp(prop, "a") == 0)
	{
		style->draw_color.a = (unsigned char)ft_atoi(val);
		style->flags |= STYLE_COLOR_A;
	}
	else if (strcmp(prop, "brightness") == 0)
	{
		style->brightness = (float)atof(val);
		style->flags |= STYLE_BRIGHTNESS;
	}
}

static void	dispatch_prop(t_style *style, char *sel, char *prop, char *val)
{
	rtrim(prop);
	rtrim(val);
	if (strcmp(sel, "window") == 0)
		parse_window_prop(style, prop, val);
	else if (strcmp(sel, "button") == 0)
		parse_button_prop(style, prop, val);
	else if (strcmp(sel, "canvas") == 0 && strcmp(prop, "background-color") == 0)
	{
		style->canvas_bg = parse_hex_color(val);
		style->flags |= STYLE_CANVAS_BG;
	}
	else if (strcmp(sel, "slider") == 0 && strcmp(prop, "fill-color") == 0)
	{
		style->slider_fill = parse_hex_color(val);
		style->flags |= STYLE_SLIDER_FILL;
	}
	else if (strcmp(sel, "color") == 0)
		parse_color_prop(style, prop, val);
}

/* ── file parser ─────────────────────────────────────────────────────────── */

int	parse_css_file(const char *path, t_style *style)
{
	int		f;
	char	*line;
	char	selector[64];
	char	*p;
	char	*sep;

	*style = (t_style){0};
	selector[0] = '\0';
	f = open(path, O_RDONLY);
	if (f < 0)
		return (ft_printf("Error: Failed to open CSS file\n") || 0);
	ft_printf("Parsing CSS file: %s\n", path);
	line = get_next_line(f);
	while (line)
	{
		p = skip_ws(line);
		if (*p == '\0' || (*p == '/' && (p[1] == '/' || p[1] == '*')))
		{
			free(line);
			line = get_next_line(f);
			continue ;
		}
		sep = strchr(p, '{');
		if (sep)
		{
			*sep = '\0';
			rtrim(p);
			strncpy(selector, p, sizeof(selector) - 1);
			selector[sizeof(selector) - 1] = '\0';
		}
		else if (*p == '}')
			selector[0] = '\0';
		else if ((sep = strchr(p, ':')) && selector[0])
		{
			*sep = '\0';
			dispatch_prop(style, selector, p, skip_ws(sep + 1));
		}
		free(line);
		line = get_next_line(f);
	}
	close(f);
	return (1);
}

/* ── style application ───────────────────────────────────────────────────── */

// static void	apply_buttons(t_data *data, t_style *style)
// {
// 	unsigned int	normal;
// 	unsigned int	hover;
// 	unsigned int	click;
// 	int				i;

// 	normal = (style->flags & STYLE_BUTTON_NORMAL) ? style->button_normal : 0x00000000;
// 	hover = (style->flags & STYLE_BUTTON_HOVER) ? style->button_hover : normal;
// 	click = (style->flags & STYLE_BUTTON_CLICK) ? style->button_click : normal;
// 	i = 0;
// 	while (i < NUM_TOOL)
// 	{
// 		ui_set_widget_normal_color(data->tool_buttons[i], normal);
// 		ui_set_widget_hovered_color(data->tool_buttons[i], hover);
// 		ui_set_widget_clicked_color(data->tool_buttons[i], click);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < NUM_HEADER)
// 	{
// 		ui_set_widget_normal_color(data->wheader.buttons[i].button, normal);
// 		ui_set_widget_hovered_color(data->wheader.buttons[i].button, hover);
// 		ui_set_widget_clicked_color(data->wheader.buttons[i].button, click);
// 		i++;
// 	}
// }

// static void	apply_sliders(t_data *data, unsigned int color)
// {
// 	int	i;

// 	i = 0;
// 	while (i < NUM_COLOR)
// 		ui_slider_set_fill_color(data->wcolor.sliders[i++].slider, color);
// 	i = 0;
// 	while (i < NUM_TOOL)
// 	{
// 		if (data->param[i].thickness_slider.slider)
// 			ui_slider_set_fill_color(data->param[i].thickness_slider.slider, color);
// 		if (data->param[i].spacing_slider.slider)
// 			ui_slider_set_fill_color(data->param[i].spacing_slider.slider, color);
// 		if (data->param[i].width_slider.slider)
// 			ui_slider_set_fill_color(data->param[i].width_slider.slider, color);
// 		if (data->param[i].height_slider.slider)
// 			ui_slider_set_fill_color(data->param[i].height_slider.slider, color);
// 		i++;
// 	}
// }

// static void	apply_draw_color(t_data *data, t_style *style)
// {
// 	if (style->flags & STYLE_COLOR_R)
// 		data->color.r = style->draw_color.r;
// 	if (style->flags & STYLE_COLOR_G)
// 		data->color.g = style->draw_color.g;
// 	if (style->flags & STYLE_COLOR_B)
// 		data->color.b = style->draw_color.b;
// 	if (style->flags & STYLE_COLOR_A)
// 		data->color.a = style->draw_color.a;
// 	if (style->flags & STYLE_BRIGHTNESS)
// 		data->brightness = style->brightness;
// 	ui_set_slider_value(data->wcolor.sliders[RED].slider, (float)data->color.r / 255.0f);
// 	ui_set_slider_value(data->wcolor.sliders[GREEN].slider, (float)data->color.g / 255.0f);
// 	ui_set_slider_value(data->wcolor.sliders[BLUE].slider, (float)data->color.b / 255.0f);
// 	ui_set_slider_value(data->wcolor.sliders[ALPHA].slider, (float)data->color.a / 255.0f);
// 	ui_set_slider_value(data->wcolor.sliders[BRIGHTNESS].slider, data->brightness);
// 	ui_set_widget_normal_color(data->wcolor.button, pack_color(data->color, data->brightness));
// 	ui_set_widget_hovered_color(data->wcolor.button, pack_color(data->color, data->brightness));
// 	ui_set_widget_clicked_color(data->wcolor.button, pack_color(data->color, data->brightness));
// 	ui_set_text_str_by_float(data->wcolor.sliders[RED].value_text.text, data->brightness * data->color.r, 1);
// 	ui_set_text_str_by_float(data->wcolor.sliders[GREEN].value_text.text, data->brightness * data->color.g, 1);
// 	ui_set_text_str_by_float(data->wcolor.sliders[BLUE].value_text.text, data->brightness * data->color.b, 1);
// 	ui_set_text_str_by_float(data->wcolor.sliders[ALPHA].value_text.text, data->color.a, 1);
// 	ui_set_text_str_by_float(data->wcolor.sliders[BRIGHTNESS].value_text.text, data->brightness, 3);
// }

// void	apply_css_style(t_data *data, t_style *style)
// {
// 	if (style->flags & STYLE_WINDOW_BG)
// 	{
// 		ui_set_widget_normal_color(data->wwindows.tool, style->window_bg);
// 		ui_set_widget_hovered_color(data->wwindows.tool, style->window_bg);
// 		ui_set_widget_clicked_color(data->wwindows.tool, style->window_bg);
// 		ui_set_widget_normal_color(data->wwindows.render, style->window_bg);
// 		ui_set_widget_hovered_color(data->wwindows.render, style->window_bg);
// 		ui_set_widget_clicked_color(data->wwindows.render, style->window_bg);
// 	}
// 	if (style->flags & (STYLE_BUTTON_NORMAL | STYLE_BUTTON_HOVER | STYLE_BUTTON_CLICK))
// 		apply_buttons(data, style);
// 	if (style->flags & STYLE_CANVAS_BG)
// 		ui_clear_layer(data->wcanvas.layers[0], style->canvas_bg);
// 	if (style->flags & STYLE_SLIDER_FILL)
// 		apply_sliders(data, style->slider_fill);
// 	if (style->flags & (STYLE_COLOR_R | STYLE_COLOR_G | STYLE_COLOR_B
// 			| STYLE_COLOR_A | STYLE_BRIGHTNESS))
// 		apply_draw_color(data, style);
// }

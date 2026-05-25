/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_prefabs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 01:50:32 by injah             #+#    #+#             */
/*   Updated: 2026/05/05 15:58:37 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui_int.h"

t_prefab_text	ui_create_prefab_text(t_widget *parent, t_rect relative_rect, const char *str, unsigned int font_color)
{
	t_prefab_text	prefab;

	prefab.box = ui_create_box(parent, relative_rect, DEFAULT, (t_margin){0, 0, 0, 0});
	prefab.box->hit_testable = false;
	prefab.box->outline = 0;
	ui_set_widget_colors(prefab.box, 0, 0, 0);
	prefab.text = ui_create_text(prefab.box, 0, 0, str, FIT, 20);
	ui_set_widget_colors(prefab.text, font_color, font_color, font_color);
	return (prefab);
}

t_widget *ui_create_prefab_modal_info_window(t_core *core, int width, int height, e_box_mode inner_box_mode, const char *info, int font_size)
{
	t_widget	*modal;
	t_widget	*container;
	char		**split;
	int			i;

	modal = ui_create_window(core, (t_rect){0, 0, width, height}, MODAL_INFO_WINDOW, "DUMMY");
	modal->outline = 2;
	container = ui_create_box(modal, (t_rect){0, 0, width, height - 60}, inner_box_mode, (t_margin){5, 0, 0, 10});
	container->outline = 2;
	container->hit_testable = false;
	ui_set_widget_colors(container, 0, 0, 0);
	if (inner_box_mode == DEFAULT)
	{
		ui_create_text(container, 10, 10, info, DEFAULT_TEXT, font_size);
		return (modal);
	}
	split = ft_split(info, '\n');
	i = 0;
	while (split[i])
	{
		ui_create_text(container, 10, 0, split[i], DEFAULT_TEXT, font_size);
		free(split[i]);
		i++;
	}
	free(split);
	return (modal);
}

t_prefab_button	ui_create_prefab_onoff_button(t_widget *parent, t_rect relative_rect, const char *str, e_text_mode text_mode)
{
	t_prefab_button	prefab;

	prefab.button = ui_create_onoff_button(parent, relative_rect);

	prefab.text = ui_create_text(prefab.button, 0, 0, str, text_mode, relative_rect.height);
	return (prefab);
}

t_prefab_button	ui_create_prefab_button(t_widget *parent, t_rect relative_rect, const char *str, e_text_mode text_mode)
{
	t_prefab_button	prefab;

	prefab.button = ui_create_button(parent, relative_rect);

	prefab.text = ui_create_text(prefab.button, 0, 0, str, text_mode, relative_rect.height);
	return (prefab);
}

t_prefab_slider	ui_create_prefab_slider(t_widget *parent, t_rect relative_rect, char *name, float default_value, float max_value)
{
	t_prefab_slider	prefab;
	char			*str;
	int				nbr;

	nbr = (int)default_value;
	if (default_value == nbr)
		str = ft_itoa(default_value);
	else
		str = ft_ftoa(default_value, 1);
	prefab.box = ui_create_box(parent, relative_rect, EVEN_CHILDS_HORIZONTAL, (t_margin){0, 0, 0, 0});
	prefab.name_text = ui_create_prefab_text(prefab.box, (t_rect){0, 0, 0, 0}, name, 0xFFFFFFFF);
	prefab.slider = ui_create_slider(prefab.box, (t_rect){0, 0, 0, 0});
	prefab.slider->outline = 1;
	ui_set_slider_value(prefab.slider, (float)default_value / max_value);
	prefab.value_text = ui_create_prefab_text(prefab.box, (t_rect){0, 0, 0, 0}, str, 0xFFFFFFFF);
	free(str);
	return (prefab);
}

t_prefab_box	ui_create_prefab_box(t_widget *parent, t_rect relative_rect, e_box_mode mode, const char *title, t_rect title_rect)
{
	t_prefab_box	prefab;


	prefab = (t_prefab_box){0};
	prefab.box = ui_create_box(parent, relative_rect, LIST_CHILDS_VERTICAL, (t_margin){0, 0, 0, 0});
	prefab.title = ui_create_prefab_text(prefab.box, title_rect, title, 0xFFFFFFFF);
	prefab.button1 = ui_create_button(prefab.title.box, (t_rect){title_rect.height / 4, title_rect.height / 4, title_rect.height / 2, title_rect.height / 2});
	prefab.button2 = ui_create_button(prefab.title.box, (t_rect){3 * title_rect.height / 4 + 2, title_rect.height / 4, title_rect.height / 2, title_rect.height / 2});
	prefab.inner_box = ui_create_box(prefab.box, (t_rect){0, title_rect.y + title_rect.height, relative_rect.width, relative_rect.height - (title_rect.y + title_rect.height)}, mode, (t_margin){5, 5, 5, 5});
	ui_set_widget_resizable(prefab.box, true);
	ui_set_widget_dragable(prefab.box, true);
	ui_set_widget_drag_zone(prefab.box, true, false, title_rect);
	return (prefab);
}

void		ui_toggle_minimize_prefab_box(t_prefab_box *prefab)
{
	if (prefab->is_minimized == false)
	{
		prefab->is_minimized = true;
		prefab->initial_height = prefab->box->height;
		ui_set_widget_size(prefab->box, prefab->box->width, prefab->title.box->height);
	}
	else
	{
		prefab->is_minimized = false;
		ui_set_widget_size(prefab->box, prefab->box->width, prefab->initial_height);
	}
}

t_prefab_header	ui_create_prefab_header(t_widget *parent, int pos_x, int pos_y, const char **headers, int num_header)
{
	t_prefab_header	prefab;
	t_widget		*box;

	int				i;

	prefab = (t_prefab_header){0};
	prefab.container = ui_create_box(parent, (t_rect){pos_x, pos_y, 500, 500}, DEFAULT, (t_margin){0, 0, 0, 0});
	prefab.container->hit_testable = false;
	prefab.container->outline = 0;
	ui_set_widget_colors(prefab.container, 0, 0, 0);
	box = ui_create_box(prefab.container, (t_rect){0, 0, 500, 30}, LIST_CHILDS_HORIZONTAL, (t_margin){0, 0, 0, 0});
	box->outline = 0;
	ui_set_widget_colors(box, 0, 0, 0);
	i = 0;
	while (i < num_header)
	{
		prefab.buttons[i] = ui_create_prefab_button(box, (t_rect){60 * i, 0, 60, 30}, headers[i], FIT);
		prefab.box[i] = ui_create_box(prefab.container, (t_rect){60 * i, 30, 120, 250}, LIST_CHILDS_VERTICAL, (t_margin){0, 0, 0, 0});
		prefab.box[i]->is_visible = false;
		i++;
	}
	return (prefab);
}

t_prefab_input	ui_create_prefab_input(t_widget *parent, t_rect rect, const char *label)
{
	t_prefab_input	prefab;
	t_widget		*text;

	prefab.box = ui_create_box(parent, rect, DEFAULT, (t_margin){0, 0, 0, 0});
	ui_set_widget_colors(prefab.box, 0, 0, 0);
	prefab.box->outline = 0;
	text = ui_create_text(prefab.box, 0, 0, label, SINGLE_LINE, rect.height - 10);
	prefab.input = ui_create_input(prefab.box, (t_rect){text->width + 10, 5, rect.width - text->width - 10, rect.height - 10}, rect.height - 12);
	return (prefab);
}

t_prefab_font_list	ui_create_prefab_font_list(t_widget *parent, t_rect rect)
{
	t_prefab_font_list	prefab;
	int					i;

	ft_strcpy(prefab.names[ROBOTO], "Roboto");
	ft_strcpy(prefab.paths[ROBOTO], "libui/assets/fonts/Roboto-Regular.ttf");
	ft_strcpy(prefab.names[FROZENSOLID], "Frozen Solid");
	ft_strcpy(prefab.paths[FROZENSOLID], "libui/assets/fonts/FrozenSolid.ttf");
	ft_strcpy(prefab.names[BOKYTIME], "Bokytime");
	ft_strcpy(prefab.paths[BOKYTIME], "libui/assets/fonts/WtfBokytimeRegular.ttf");
	ft_strcpy(prefab.names[AMARANTE], "Amarante");
	ft_strcpy(prefab.paths[AMARANTE], "libui/assets/fonts/Amarante-Regular.ttf");
	ft_strcpy(prefab.names[BITCOUTINK], "BitcountInk");
	ft_strcpy(prefab.paths[BITCOUTINK], "libui/assets/fonts/BitcountInk.ttf");
	ft_strcpy(prefab.names[TOMORROW], "Tomorrow");
	ft_strcpy(prefab.paths[TOMORROW], "libui/assets/fonts/Tomorrow-Regular.ttf");
	prefab.box = ui_create_box(parent, rect, LIST_CHILDS_VERTICAL, (t_margin){0, 0, 0, 0});
	i = 0;
	while (i < NUM_FONT)
	{
		prefab.buttons[i] = ui_create_prefab_button(prefab.box, (t_rect){0, 0, 0, 30}, prefab.names[i], SINGLE_LINE);
		i++;
	}
	
	return (prefab);
}

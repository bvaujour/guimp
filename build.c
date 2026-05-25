/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:24:03 by injah             #+#    #+#             */
/*   Updated: 2026/05/03 23:47:01 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"

void	build_canvas_infos(t_canvas_widgets *wcanvas)
{
	char	*str;
	float	zoom;

	ft_strcpy(wcanvas->zoom_infos_str, "Resolution: ");
	str = ft_itoa(wcanvas->resolution.x);
	ft_strcat(wcanvas->zoom_infos_str, str);
	free(str);
	ft_strcat(wcanvas->zoom_infos_str, "x");
	str = ft_itoa(wcanvas->resolution.y);
	ft_strcat(wcanvas->zoom_infos_str, str);
	free(str);
	ft_strcat(wcanvas->zoom_infos_str, ", Zoom: ");
	zoom = ui_canvas_get_zoom(wcanvas->canvas) * 100;
	str = ft_ftoa(zoom, 2);
	ft_strcat(wcanvas->zoom_infos_str, str);
	free(str);
	ft_strcat(wcanvas->zoom_infos_str, "%");
	ui_set_text_str(wcanvas->zoom_infos_text, wcanvas->zoom_infos_str);
}

void	new_canvas(t_data *data, int resolution_x, int resolution_y)
{
	int	width;
	int	height;
	
	if (data->nb_canvas >= MAX_CANVAS)
		return ;
	data->canvas_buttons[data->nb_canvas] = ui_create_button(data->canvas_buttons_box, (t_rect){0, 0, 70, 0});
	ui_get_widget_size(data->wwindows.render, &width, &height);
	data->wcanvas[data->nb_canvas].container = ui_create_box(data->wwindows.render, (t_rect){0, 50, width, height - 100}, DEFAULT, (t_margin){0, 0, 0, 0});
	ui_get_widget_size(data->wcanvas[data->nb_canvas].container, &width, &height);
	data->wcanvas[data->nb_canvas].render_box = ui_create_box(data->wcanvas[data->nb_canvas].container, (t_rect){10, 10, width - 210, height - 40}, DEFAULT, (t_margin){0, 0, 0, 0});
	data->wcanvas[data->nb_canvas].zoom_infos_text =  ui_create_text(data->wcanvas[data->nb_canvas].container, 10, height - 25, data->wcanvas[data->nb_canvas].zoom_infos_str, DEFAULT_TEXT, 14);

	data->wcanvas[data->nb_canvas].add_layer = ui_create_button(data->wcanvas[data->nb_canvas].container, (t_rect){width - 190, 10, 30, 30});
	data->wcanvas[data->nb_canvas].clear_layer = ui_create_button(data->wcanvas[data->nb_canvas].container, (t_rect){width - 160, 10, 30, 30});
	data->wcanvas[data->nb_canvas].close_layer = ui_create_button(data->wcanvas[data->nb_canvas].container, (t_rect){width - 130, 10, 30, 30});
	data->wcanvas[data->nb_canvas].downgrade_layer = ui_create_button(data->wcanvas[data->nb_canvas].container, (t_rect){width - 70, 10, 30, 30});
	data->wcanvas[data->nb_canvas].upgrade_layer = ui_create_button(data->wcanvas[data->nb_canvas].container, (t_rect){width - 40, 10, 30, 30});
	ui_set_widget_texture(data->wcanvas[data->nb_canvas].upgrade_layer, "assets/images/arrow_d.png");
	ui_set_widget_texture(data->wcanvas[data->nb_canvas].downgrade_layer, "assets/images/arrow.png");
	ui_set_widget_texture(data->wcanvas[data->nb_canvas].add_layer, "assets/images/Plus.png");
	ui_set_widget_texture(data->wcanvas[data->nb_canvas].clear_layer, "assets/images/clean.png");
	ui_set_widget_texture(data->wcanvas[data->nb_canvas].close_layer, "assets/images/delete.png");
	ui_set_widget_outline(data->wcanvas[data->nb_canvas].upgrade_layer, 1);
	ui_set_widget_outline(data->wcanvas[data->nb_canvas].downgrade_layer, 1);
	ui_set_widget_outline(data->wcanvas[data->nb_canvas].add_layer, 1);
	ui_set_widget_outline(data->wcanvas[data->nb_canvas].clear_layer, 1);
	ui_set_widget_outline(data->wcanvas[data->nb_canvas].close_layer, 1);
	
	data->wcanvas[data->nb_canvas].layers_buttons_box = ui_create_box(data->wcanvas[data->nb_canvas].container, (t_rect){width - 190, 40, 180, height - 70}, LIST_CHILDS_VERTICAL, (t_margin){10, 10, 5, 10});
	
	ui_set_widget_scroll_mode(data->wcanvas[data->nb_canvas].render_box, FREE_SCROLL);
	ui_get_widget_size(data->wcanvas[data->nb_canvas].render_box, &width, &height);
	data->wcanvas[data->nb_canvas].resolution.x = resolution_x;
	data->wcanvas[data->nb_canvas].resolution.y = resolution_y;
	data->wcanvas[data->nb_canvas].canvas = ui_create_canvas(data->wcanvas[data->nb_canvas].render_box, resolution_x, resolution_y);
	build_canvas_infos(&data->wcanvas[data->nb_canvas]);
	data->nb_canvas++;
	set_active_canvas(data, data->nb_canvas - 1);
	add_empty_layer(data);
}

void	open_modal_new(t_data *data)
{
	data->modal_new.window = ui_create_window(data->core, (t_rect){0, 0, 400, 200}, MODAL_ACCEPT_WINDOW, "NEW");

	ui_create_prefab_text(data->modal_new.window, (t_rect){120, 10, 150, 50}, "New image size", 0xFFFFFFFF);

	data->modal_new.width = ui_create_prefab_input(data->modal_new.window, (t_rect){130, 65, 120, 30}, "Width:");
	data->modal_new.height = ui_create_prefab_input(data->modal_new.window, (t_rect){130, 100, 120, 30}, "Height:");

	ui_input_set_text(data->modal_new.width.input, "1920");
	ui_input_set_text(data->modal_new.height.input, "1080");
}
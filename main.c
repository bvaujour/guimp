/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:53:38 by bvaujour          #+#    #+#             */
/*   Updated: 2025/10/28 13:27:31 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"

void	quit(t_data *data)
{
	ui_destroy(&data->core);
	exit(0);
}

void	test(t_data *data)
{
	printf("hello %d\n", data->running);
}

void	fill_menu_bar(t_data *data)
{
	data->menubar_buttons[FILE_BUTTON] = ui_create_button(data->menubar, "File");
	ui_bind_button(data->menubar_buttons[FILE_BUTTON], &test, data);
	data->menubar_buttons[EDIT_BUTTON] = ui_create_button(data->menubar, "Edit");
	data->menubar_buttons[VIEW_BUTTON] = ui_create_button(data->menubar, "View");
}

void	create_boxs(t_data *data)
{
	data->menubar = ui_create_horizontal_box(data->tools_window, 3);
	ui_box_set_inner_elements(data->menubar, 5);
	data->tool_pallet = ui_create_horizontal_box(data->tools_window, 27);
	ui_box_set_inner_elements(data->tool_pallet, 3);
	ui_box_set_wrap_elements(data->tool_pallet, 5);
	ui_box_set_padding(data->tool_pallet, 5, 10, 5, 10);
	ui_box_set_gap(data->tool_pallet, 10);
	data->tool_options = ui_create_vertical_box(data->tools_window, 70);
	
	data->render = ui_create_horizontal_box(data->rendering_window, 1);
}

void	create_windows(t_data *data)
{
	data->tools_window = ui_create_basic_window(&data->core, "tools_window", 1);
	if (!data->tools_window)
		quit(data);
	data->rendering_window = ui_create_rendering_window(&data->core, "rendering_window", 3);
	if (!data->rendering_window)
		quit(data);
}




int	main()
{
	t_data		data;

	data = (t_data){0};
	if (ui_init(&data.core) != 0)
		quit(&data);
	create_windows(&data);
	create_boxs(&data);
	fill_menu_bar(&data);
	

	ui_create_button(data.tool_pallet, "TEST");
	ui_create_button(data.tool_pallet, "TEST");
	ui_create_button(data.tool_pallet, "TEST");
	ui_create_button(data.tool_pallet, "TEST");
	ui_create_button(data.tool_pallet, "TEST");
	ui_create_button(data.tool_pallet, "TEST");
	ui_run(&data.core);
	quit(&data);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_context.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:53:38 by bvaujour          #+#    #+#             */
/*   Updated: 2025/09/23 08:16:17 by injah            ###   ########.fr       */
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

int	main()
{
	t_data		data;

	data = (t_data){0};
	if (ui_init(&data.core) != 0)
		quit(&data);
	data.tools = ui_create_basic_window(&data.core, "tools", 1);
	if (!data.tools)
		quit(&data);
	data.rendering = ui_create_rendering_window(&data.core, "rendering", 3);
	if (!data.rendering)
		quit(&data);
	data.core.flex_direction = HORIZONTAL;
	data.tools->flex_direction = VERTICAL;
	data.menubar = ui_create_box(data.tools, 3);
	data.tool_pallet = ui_create_box(data.tools, 27);
	data.tool_options = ui_create_box(data.tools, 70);
	data.menubar_buttons[FILE_BUTTON] = ui_add_button(data.menubar, "File");
	ui_bind_button(data.menubar_buttons[FILE_BUTTON], &test, &data);
	data.menubar_buttons[EDIT_BUTTON] = ui_add_button(data.menubar, "Edit");
	data.menubar_buttons[VIEW_BUTTON] = ui_add_button(data.menubar, "View");
	ui_run(&data.core);
	quit(&data);
	return (0);
}

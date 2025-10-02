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
	data.tools = ui_create_basic_window(&data.core, "tools", (SDL_Rect){0, 0, data.core.screen_w / 4, data.core.screen_h - 150});
	if (!data.tools)
		quit(&data);
	data.rendering = ui_create_rendering_window(&data.core, "rendering",(SDL_Rect){data.core.screen_w / 4, 0, 3 * data.core.screen_w / 4, data.core.screen_h - 150});
	if (!data.rendering)
		quit(&data);

	data.menubar = ui_create_box(data.tools, HORIZONTAL, (SDL_Rect){10, 10, data.tools->rect.w - 20, 50});
	// data.tool_pallet = ui_create_box(data.tools, VERTICAL, (SDL_Rect){10, 46, data.tools->rect.w - 20, data.tools->rect.h / 2 - 56});
	data.tool_options = ui_create_box(data.tools, VERTICAL, (SDL_Rect){10, data.tools->rect.h / 2, data.tools->rect.w - 20, data.tools->rect.h / 2 - 10});
	data.menubar_buttons[FILE_BUTTON] = ui_add_button(data.menubar, "File", 18, MENUBAR_BUTTON_PADDING);
	ui_bind_button(data.menubar_buttons[FILE_BUTTON], &test, &data);
	data.menubar_buttons[EDIT_BUTTON] = ui_add_button(data.menubar, "Edit", 18, MENUBAR_BUTTON_PADDING);
	data.menubar_buttons[VIEW_BUTTON] = ui_add_button(data.menubar, "View", 18, MENUBAR_BUTTON_PADDING);
	ui_run(&data.core);
	quit(&data);
	return (0);
}

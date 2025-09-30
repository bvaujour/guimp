/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_ctx.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:53:38 by bvaujour          #+#    #+#             */
/*   Updated: 2025/09/23 08:16:17 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"


// void	quit(t_data *data)
// {
// 	SDL_DestroyRenderer(data->main_ctx.renderer);
// 	SDL_DestroyWindow(data->main_ctx.window);
// 	ui_quit();
// 	exit(1);
// }

// void	button_quit(t_data *data)
// {
// 	quit(data);
// }

// void	setup(t_data *data)
// {
// 	if (ui_init() != 0)
// 		quit(data);
// 	ui_get_screen_size(&data->main_ctx.width, &data->main_ctx.height);
// 	data->main_ctx.renderer = NULL;
// 	data->main_ctx.window = NULL;
// 	SDL_CreateWindowAndRenderer(data->main_ctx.width, data->main_ctx.height, SDL_WINDOW_RESIZABLE, &data->main_ctx.window, &data->main_ctx.renderer);
// 	if (!data->main_ctx.window || !data->main_ctx.renderer)
// 		quit(data);
// }

void	quit(t_data *data)
{
	ui_destroy(&data->core);
	exit(0);
}

void	test(t_data *data, int x)
{
	printf("hello %d %d\n", data->running, x);
}

int	main()
{
	// SDL_Init(SDL_INIT_EVERYTHING);
	// SDL_Quit();
	t_data		data;

	data = (t_data){0};
	if (ui_init(&data.core) != 0)
		quit(&data);
	data.rendering = ui_create_rendering_window(&data.core, "rendering",(SDL_Rect){data.core.screen_w / 2, 0, data.core.screen_w / 2, data.core.screen_h});
	if (!data.rendering)
		quit(&data);
	data.tools = ui_create_tool_window(&data.core, "tools", (SDL_Rect){0, 0, data.core.screen_w / 3, data.core.screen_h});
	if (!data.tools)
		quit(&data);

	data.menubar = ui_create_container(data.tools, TOPTOBOTTOM, (SDL_Rect){0, 10, data.core.screen_w, 50});
	data.menubar_buttons[FILE_BUTTON] = ui_add_button(data.menubar, "File", 18, MENUBAR_BUTTON_PADDING);
	ui_bind_button(data.menubar_buttons[FILE_BUTTON], &test, &data);
	data.menubar_buttons[EDIT_BUTTON] = ui_add_button(data.menubar, "Edit", 18, MENUBAR_BUTTON_PADDING);
	data.menubar_buttons[VIEW_BUTTON] = ui_add_button(data.menubar, "View", 18, MENUBAR_BUTTON_PADDING);
	ui_run(&data.core);
	quit(&data);
	return (0);
}

// int	main()
// {
// 	t_data	data;

// 	data = (t_data){0};

// 	if (ui_init() != 0)
// 	quit(&data);
// 	ui_get_screen_size(&data.main_ctx.width, &data.main_ctx.height);
// 	data.main_ctx.renderer = NULL;
// 	data.main_ctx.window = NULL;
// 	SDL_CreateWindowAndRenderer(data.main_ctx.width, data.main_ctx.height, SDL_WINDOW_RESIZABLE, &data.main_ctx.window, &data.main_ctx.renderer);
// 	if (!data.main_ctx.window || !data.main_ctx.renderer)
// 		quit(&data);
// 	data.running = true;
// 	Uint32 frame_start = 0;
// 	const int fps = 30;
// 	Uint32	target_frame_duration = 1000 / fps;
// 	Uint32	frame_duration;
// 	// float	deltatime;
// 	t_widget container = ui_create_container((SDL_Rect){50, 0, 100, 500}, (SDL_Color){55, 55, 55, 255});

// 	ui_add_children(&container, (SDL_Rect){0, 0, 100, 50}, (SDL_Color){255, 0, 0, 255});
// 	ui_add_children(&container, (SDL_Rect){0, 55, 100, 50}, (SDL_Color){255, 255, 0, 255});
// 	ui_add_children(&container.children[1], (SDL_Rect){5, 5, 50, 50}, (SDL_Color){0, 255, 100, 255});

// 	SDL_Event	event;
// 	bool		left_click = false;
// 	while (data.running)
// 	{
// 		// deltatime = 1.0f * (SDL_GetTicks() - frame_start) / 1000;
// 		frame_start = SDL_GetTicks();
// 		while (SDL_PollEvent(&event))
// 		{
// 			if (event.type == SDL_QUIT)
// 				data.running = false;
// 			else if (event.type == SDL_MOUSEBUTTONUP)
// 			{
//                 if (event.button.button == SDL_BUTTON_LEFT)
// 				{
// 					left_click = false;
// 				}
// 			}
// 			else if (event.type == SDL_MOUSEBUTTONDOWN)
// 			{
//                 if (event.button.button == SDL_BUTTON_LEFT)
// 				{
// 					left_click = true;
// 				}
// 			}
// 			else if (event.type == SDL_MOUSEMOTION)
// 			{
				
// 				if (left_click)
// 				{
// 					printf("x:%d, y:%d\n", event.motion.x, event.motion.y);
// 					if (container.is_hovered)
// 					ui_move_widget(&container, event.motion.x, event.motion.y);
// 				}
// 				ui_widget_update(&container, event.motion.x,  event.motion.y);
// 			}
// 		}
// 		ui_render_widget_and_children(&data.main_ctx, &container);
// 		SDL_RenderPresent(data.main_ctx.renderer);
// 		SDL_SetRenderDrawColor(data.main_ctx.renderer, 60, 60, 60, 255);
// 		SDL_RenderClear(data.main_ctx.renderer);

// 		frame_duration = SDL_GetTicks() - frame_start;
// 		if (frame_duration < target_frame_duration)
// 			SDL_Delay(target_frame_duration - frame_duration);
// 	}
// 	ui_destroy_widget(&container);
// 	quit(&data);
// 	return (0);
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:23:08 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 14:54:59 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

static int	ui_base_config(t_core *core)
{
	core->config.font = TTF_OpenFont(UI_THEME1_FONT, 15);
	if (!core->config.font)
	{
		fprintf(stderr, "SDL_Init error: %s\n", TTF_GetError());
        return(-1);
	}
	core->config.box_color = UI_THEME1_BOX_COLOR;
	core->config.window_color = UI_THEME1_WINDOW_COLOR;
	core->config.button_color[DEFAULT] = UI_THEME1_BUTTON_DEFAULT_COLOR;
	core->config.button_color[HOVERED] = UI_THEME1_BUTTON_HOVERED_COLOR;
	core->config.button_color[CLICKED] = UI_THEME1_BUTTON_CLICKED_COLOR;
	return (0);
}

int	ui_init(t_core *core)
{
	int img_flags;
	
	*core = (t_core){0};
	core->is_running = true;
	img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return (1);
    }
	if (TTF_Init() != 0)
	{
		fprintf(stderr, "TTF_Init error: %s\n", TTF_GetError());
		return (2);
	}
	if ((IMG_Init(img_flags) & img_flags) != img_flags)
	{
		fprintf(stderr, "IMG_Init error: %s\n", IMG_GetError());
		return (3);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	ui_get_screen_size(&core->screen_w, &core->screen_h);
	if (ui_base_config(core) != 0)
		return (4);
	return (0);
}

void	ui_run(t_core *core)
{
	// SDL_StartTextInput();
	// SDL_SetWindowModalFor();
	// SDL_HideWindow();
	// SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "info", "hello world\n re", core->contexts[0].window);
	// SDL_HideWindow(core->contexts[2].window);
	// SDL_ShowWindow(core->contexts[2].window);
	ui_build_everything(core);
	while (core->is_running)
	{
		core->scrolled = false;
		core->wheel.x = 0;
		core->wheel.y = 0;
		if (SDL_WaitEvent(&core->event))
		{
			// if (core->event.type == SDL_KEYDOWN)
			// {
			// 	printf("scancode: %d, sym: %d\n", core->event.key.keysym.scancode, core->event.key.keysym.sym);
			// 	core->hotkeys[core->event.key.keysym.scancode] = true;
			// }
			if (core->event.type == SDL_KEYDOWN)
			{
				if (core->event.key.keysym.sym == SDLK_LSHIFT)
				{
					printf("lshift\n");
					core->lshift = true;
				}
			}
			if (core->event.type == SDL_KEYUP)
			{
				if (core->event.key.keysym.sym == SDLK_ESCAPE)
				{
					core->is_running = false;
				}
				if (core->event.key.keysym.sym == SDLK_LSHIFT)
				{
					core->lshift = false;
				}
			}
			else if (core->event.type == SDL_DROPFILE)
			{
				printf("Fichier déposé : %s\n", core->event.drop.file);
			}

			else if (core->event.type == SDL_MOUSEMOTION)
			{
				core->mouse.x = core->event.motion.x;
				core->mouse.y = core->event.motion.y;
				// printf("mouse: x: %d, y: %d\n", core->event.motion.x, core->event.motion.y);
				core->focused_window_id = core->event.window.windowID;
			}
			else if (core->event.type == SDL_TEXTINPUT)
			{
				
				printf("Input: %s\n", core->event.text.text);
			}
			else if (core->event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (core->event.button.button == SDL_BUTTON_LEFT)
					core->left_click = true;
			}
			else if (core->event.type == SDL_MOUSEBUTTONUP)
			{
				if (core->event.button.button == SDL_BUTTON_LEFT)
					core->left_click = false;
			}
			else if (core->event.type == SDL_MOUSEWHEEL)
			{
				if (core->lshift == true)
					core->wheel.x = core->event.wheel.y;
				else
					core->wheel.y = core->event.wheel.y;
				core->scrolled = true;
			}
			else if (core->event.type == SDL_QUIT)
				core->is_running = false;

		}
		ui_update(core);
		ui_render(core);
	}
}
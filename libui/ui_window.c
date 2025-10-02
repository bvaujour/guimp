/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_window.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 12:53:46 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 16:28:22 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	init_context(t_core *core, t_context *context, int flex)
{
	*context = (t_context){0};
	context->core = core;
	context->is_durty = true;
	context->flex = flex;
	context->padding.left = 10;
	context->padding.right = 10;
	context->padding.top = 10;
	context->padding.bottom = 10;
	context->gap = 10;
}

t_context	*ui_create_basic_window(t_core *core, const char *title, int flex)
{
	t_context	*context;
	int			index;

	index = core->nb_contexts;
	context = &core->contexts[index];
	init_context(core, context, flex);
	context->window = SDL_CreateWindow(title, 0, 0, 0, 0, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!context->window)
	{
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return (NULL);
    }
    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED);
    if (!context->renderer)
	{
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(context->window);
        return (NULL);
    }
	core->nb_contexts++;
    return (context);
}

t_context	*ui_create_rendering_window(t_core *core, const char *title, int flex)
{
	t_context	*context;
	int		index;

	index = core->nb_contexts;
	context = &core->contexts[index];
	init_context(core, context, flex);
	context->window = SDL_CreateWindow(title, 0, 0, 0, 0, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!context->window)
	{
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return (NULL);
    }
    context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!context->renderer)
	{
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(context->window);
        return (NULL);
    }
	core->nb_contexts++;
    return (context);
}
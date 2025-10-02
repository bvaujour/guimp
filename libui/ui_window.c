/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_window.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 12:53:46 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 06:22:21 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	init_context(t_core *core, t_context *context)
{
	*context = (t_context){0};
	context->core = core;
	context->is_durty = true;
}

t_context	*ui_create_basic_window(t_core *core, const char *title, SDL_Rect rect)
{
	t_context	*context;
	int			index;

	index = core->nb_contexts;
	context = &core->contexts[index];
	init_context(core, context);
	context->rect = rect;
	context->window = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_SHOWN);
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

t_context	*ui_create_rendering_window(t_core *core, const char *title, SDL_Rect rect)
{
	t_context	*context;
	int		index;

	index = core->nb_contexts;
	context = &core->contexts[index];
	init_context(core, context);
	context->rect = rect;
	context->window = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_RESIZABLE);
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
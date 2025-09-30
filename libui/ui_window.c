/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_window.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 12:53:46 by injah             #+#    #+#             */
/*   Updated: 2025/09/28 13:59:47 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	init_ctx(t_core *core, t_ctx *ctx)
{
	*ctx = (t_ctx){0};
	ctx->core = core;
	ctx->config = &core->config;
	ctx->is_durty = true;
}

t_ctx	*ui_create_tool_window(t_core *core, const char *title, SDL_Rect rect)
{
	t_ctx	*ctx;
	int		index;

	index = core->nb_ctxs;
	ctx = &core->ctxs[index];
	init_ctx(core, ctx);
	ctx->window = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_RESIZABLE);
    if (!ctx->window)
	{
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return (NULL);
    }
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
    if (!ctx->renderer)
	{
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->window);
        return (NULL);
    }
	core->nb_ctxs++;
    return (ctx);
}

t_ctx	*ui_create_rendering_window(t_core *core, const char *title, SDL_Rect rect)
{
	t_ctx	*ctx;
	int		index;

	index = core->nb_ctxs;
	ctx = &core->ctxs[index];
	init_ctx(core, ctx);
	ctx->window = SDL_CreateWindow(title, rect.x, rect.y, rect.w, rect.h, SDL_WINDOW_RESIZABLE);
    if (!ctx->window)
	{
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        return (NULL);
    }
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!ctx->renderer)
	{
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->window);
        return (NULL);
    }
	core->nb_ctxs++;
    return (ctx);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_surface.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 08:24:54 by injah             #+#    #+#             */
/*   Updated: 2025/09/30 19:02:15 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"


void	ui_fill_surf(SDL_Surface *surface, SDL_Color color)
{
	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
}

SDL_Surface	*ui_copy_surface(SDL_Surface *surface)
{
	SDL_Surface *copy;
	copy  = SDL_ConvertSurface(surface, surface->format, 0);
	return (copy);
}

void	ui_blit_centered(SDL_Surface *src, SDL_Surface *dst)
{
	int	pos_x;
	int	pos_y;

	pos_x = dst->clip_rect.w / 2 - src->clip_rect.w / 2;
	pos_y = dst->clip_rect.h / 2 - src->clip_rect.h / 2;
	SDL_BlitSurface(src, NULL, dst, &(SDL_Rect){pos_x, pos_y, 0, 0});
}

void	ui_blit(SDL_Surface *src, SDL_Surface *dst, int x, int y)
{
	SDL_BlitSurface(src, NULL, dst, &(SDL_Rect){x, y, 0, 0});
}

SDL_Surface* ui_new_surf(int width, int height, SDL_Color *color)
{
	SDL_Surface*	surface;

	surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	if (color != NULL)
		ui_fill_surf(surface, *color);
	return (surface);
}
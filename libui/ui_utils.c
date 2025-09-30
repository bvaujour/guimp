/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 12:55:51 by injah             #+#    #+#             */
/*   Updated: 2025/09/23 10:10:44 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_get_screen_size(int *width, int *height)
{
	SDL_DisplayMode mode;

	if (SDL_GetCurrentDisplayMode(0, &mode) == 0)
		printf("Résolution courante: %dx%d\n", mode.w, mode.h);
	if (width)
		*width = mode.w;
	if (height)
		*height = mode.h;
}

void	ui_get_sdl_version()
{
	SDL_version vs;
	SDL_VERSION(&vs);
	printf("%d.%d.%d\n", vs.major, vs.minor, vs.patch);
}

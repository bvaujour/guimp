/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_widget.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:17:08 by injah             #+#    #+#             */
/*   Updated: 2025/09/30 19:12:26 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_build_button(t_widget *button, char *label, int font_size, t_offset padding)
{
	SDL_Surface	*txt_surf;
	SDL_Surface	*default_surf;
	SDL_Surface	*hovered_surf;
	SDL_Surface	*clicked_surf;

	TTF_SetFontSize(button->config->font, font_size);
	txt_surf = TTF_RenderText_Blended(button->config->font, label, UI_WHITE);

	default_surf = ui_new_surf(txt_surf->clip_rect.w + padding.left + padding.right, txt_surf->clip_rect.h + padding.top + padding.bottom, &button->config->button_color[DEFAULT]);
	ui_blit(txt_surf, default_surf, padding.left, padding.top);
	button->button_data.surfaces[DEFAULT] = default_surf;

	hovered_surf = ui_new_surf(txt_surf->clip_rect.w + padding.left + padding.right, txt_surf->clip_rect.h + padding.top + padding.bottom, &button->config->button_color[HOVERED]);
	ui_blit(txt_surf, hovered_surf, padding.left, padding.top);
	button->button_data.surfaces[HOVERED] = hovered_surf;

	clicked_surf = ui_new_surf(txt_surf->clip_rect.w + padding.left + padding.right, txt_surf->clip_rect.h + padding.top + padding.bottom, &button->config->button_color[CLICKED]);
	ui_blit(txt_surf, clicked_surf, padding.left, padding.top);
	button->button_data.surfaces[CLICKED] = clicked_surf;

	SDL_FreeSurface(txt_surf);
}

void	ui_bind_button(t_widget *button, void(*f)(), void *param)
{
	button->button_data.binded_function = f;
	button->button_data.param = param;
}

t_widget	*ui_add_button(t_container *container, char *label, int font_size, t_offset padding)
{
	t_widget	*button;
	int			index;

	index = container->nb_widget;
	button = &container->widgets[index];
	*button = (t_widget){0};
	button->type = BUTTON;
	
	button->button_data.state = DEFAULT;
	button->config = container->config;
	button->update = &ui_update_button;
	button->destroy = &ui_destroy_button;
	button->core = container->core;
	button->container = container;
	button->ctx = container->ctx;
	ui_build_button(button, label, font_size, padding);
	button->surface = button->button_data.surfaces[DEFAULT];
	button->rect = button->surface->clip_rect;
	if (container->direction == TOPTOBOTTOM)
	{
		button->relative.x = 0;
		button->relative.y = container->total_widget_rect.h;
		container->total_widget_rect.h += button->rect.h;
		if (button->rect.w > container->total_widget_rect.w)
			container->total_widget_rect.w = button->rect.w;
	}
	else if (container->direction == LEFTTORIGHT)
	{
		button->relative.x = container->total_widget_rect.w;
		button->relative.y = 0;
		container->total_widget_rect.w += button->rect.w;
		if (button->rect.h > container->total_widget_rect.h)
			container->total_widget_rect.h = button->rect.h;
	}
	container->nb_widget++;
	return (button);
}

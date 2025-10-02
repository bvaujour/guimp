/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_widget.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:17:08 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 16:37:43 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_build_button(t_widget *button, char *label, int font_size, t_offset padding)
{
	SDL_Surface	*txt_surf;
	SDL_Surface	*default_surf;
	SDL_Surface	*hovered_surf;
	SDL_Surface	*clicked_surf;

	TTF_SetFontSize(button->core->config.font, font_size);
	txt_surf = TTF_RenderText_Blended(button->core->config.font, label, UI_WHITE);

	default_surf = ui_new_surf(txt_surf->clip_rect.w + padding.left + padding.right, txt_surf->clip_rect.h + padding.top + padding.bottom, &button->core->config.button_color[DEFAULT]);
	ui_blit(txt_surf, default_surf, padding.left, padding.top);
	button->button_data.surfaces[DEFAULT] = default_surf;

	hovered_surf = ui_new_surf(txt_surf->clip_rect.w + padding.left + padding.right, txt_surf->clip_rect.h + padding.top + padding.bottom, &button->core->config.button_color[HOVERED]);
	ui_blit(txt_surf, hovered_surf, padding.left, padding.top);
	button->button_data.surfaces[HOVERED] = hovered_surf;

	clicked_surf = ui_new_surf(txt_surf->clip_rect.w + padding.left + padding.right, txt_surf->clip_rect.h + padding.top + padding.bottom, &button->core->config.button_color[CLICKED]);
	ui_blit(txt_surf, clicked_surf, padding.left, padding.top);
	button->button_data.surfaces[CLICKED] = clicked_surf;

	button->surface = button->button_data.surfaces[DEFAULT];
	button->rect = button->surface->clip_rect;
	SDL_FreeSurface(txt_surf);
}

void	ui_bind_button(t_widget *button, void(*f)(), void *param)
{
	button->button_data.on_click = f;
	button->button_data.param = param;
}



t_widget	*ui_add_button(t_box *box, char *label, int font_size, t_offset padding)
{
	t_widget	*button;
	int			index;

	index = box->nb_widget;
	button = &box->widgets[index];
	*button = (t_widget){0};
	button->type = BUTTON;
	
	button->button_data.state = DEFAULT;
	button->update = &ui_update_button;
	button->destroy = &ui_destroy_button;
	button->core = box->core;
	button->box = box;
	button->context = box->context;
	button->padding.left = 5;
	button->padding.right = 5;
	button->padding.top = 2;
	button->padding.bottom = 2;
	ui_build_button(button, label, font_size, padding);
	if (box->flex_direction == VERTICAL)
	{
		button->relative.x = 0;
		button->relative.y = box->total_widget_rect.h;
		box->total_widget_rect.h += button->rect.h;
		if (button->rect.w > box->total_widget_rect.w)
			box->total_widget_rect.w = button->rect.w;
		box->rect.w = box->total_widget_rect.w;
	}
	else if (box->flex_direction == HORIZONTAL)
	{
		button->relative.x = box->total_widget_rect.w;
		button->relative.y = 0;
		box->total_widget_rect.w += button->rect.w;
		if (button->rect.h > box->total_widget_rect.h)
			box->total_widget_rect.h = button->rect.h;
		box->rect.h = box->total_widget_rect.h;
	}
	
	box->max_scroll.x = box->rect.w - box->total_widget_rect.w;
	if (box->max_scroll.x > 0)
		box->max_scroll.x = 0;
	box->max_scroll.y = box->rect.h - box->total_widget_rect.h;
	if (box->max_scroll.y > 0)
		box->max_scroll.y = 0;
	box->nb_widget++;
	return (button);
}

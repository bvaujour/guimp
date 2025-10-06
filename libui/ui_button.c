/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_button.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:17:08 by injah             #+#    #+#             */
/*   Updated: 2025/10/06 17:03:43 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void		ui_adjust_font_size(char *label, TTF_Font *font, SDL_Rect rect)
{
	int	txt_width;
	int	txt_height;
	int	font_size;

	font_size = 1;
	TTF_SetFontSize(font, font_size);
	TTF_SizeText(font, label, &txt_width, &txt_height);
	while (txt_width < rect.w && txt_height < rect.h)
	{
		font_size++;
		TTF_SetFontSize(font, font_size);
		TTF_SizeText(font, label, &txt_width, &txt_height);
	}
	font_size--;
	TTF_SetFontSize(font, font_size);
}

void	ui_build_button(t_widget *button)
{
	SDL_Surface	*txt_surf;

	ui_adjust_font_size(button->button_data.label, button->core->config.font, button->relative);
	txt_surf = TTF_RenderText_Blended(button->core->config.font, button->button_data.label, UI_WHITE);
	if (button->surface)
		SDL_FreeSurface(button->surface);
	button->surface = ui_new_surf(button->relative.w, button->relative.h, &button->core->config.button_color[DEFAULT]);
	ui_blit_centered(txt_surf, button->surface);
	SDL_FreeSurface(txt_surf);
}

void	ui_refresh_button_surface(t_widget *button)
{
	SDL_Surface		*txt_surf;

	txt_surf = TTF_RenderText_Blended(button->core->config.font, button->button_data.label, UI_WHITE);
	ui_fill_surf(button->surface, button->core->config.button_color[button->button_data.state]);
	ui_blit_centered(txt_surf, button->surface);
	SDL_FreeSurface(txt_surf);
}

void	ui_bind_button(t_widget *button, void(*f)(), void *param)
{
	button->button_data.on_click = f;
	button->button_data.param = param;
}

t_widget	*ui_add_button(t_box *box, char *label)
{
	t_widget	*button;
	int			index;

	index = box->nb_widget;
	button = &box->widgets[index];
	*button = (t_widget){0};
	button->type = BUTTON;
	
	button->button_data.state = DEFAULT;
	button->button_data.label = label;
	button->build = &ui_build_button;
	button->update = &ui_update_button;
	button->destroy = &ui_destroy_button;
	button->core = box->core;
	button->box = box;
	button->context = box->context;
	box->nb_widget++;
	return (button);
}

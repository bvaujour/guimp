/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_box_settings.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:42:27 by bvaujour          #+#    #+#             */
/*   Updated: 2025/10/09 16:30:58 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_box_set_wrap_elements(t_box *box, int wrap_at)
{
	box->wrap_at = wrap_at;
}

void	ui_box_set_inner_elements(t_box *box, int inner_elements)
{
	box->inner_elements = inner_elements;
}

void	ui_box_set_padding(t_box *box, int padding_left, int padding_top, int padding_right, int padding_bottom)
{
	box->padding.left = padding_left;
	box->padding.top = padding_top;
	box->padding.right = padding_right;
	box->padding.bottom = padding_bottom;
}

void	ui_box_set_gap(t_box *box, int gap)
{
	box->gap = gap;
}

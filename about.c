/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   about.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 00:00:00 by injah             #+#    #+#             */
/*   Updated: 2026/05/05 16:15:18 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "guimp.h"

void	open_about_modal(t_data *data)
{
	static const char	*about_text =
		"GUIMP - Image Editor\n"
		"42 School Project\n"
		" \n"
		"Created by:\n"
		"  BVAUJOUR  |  XCHARLES  |  MADERUEL\n"
		" \n"
		"TOOLS:\n"
		"  Select, Pencil, Rectangle, Line,\n"
		"  Circle, Eraser, Text\n"
		" \n"
		"FEATURES:\n"
		"  - Multi-layer editing (up to 20 layers per canvas)\n"
		"  - Multi-canvas support (up to 10 canvases)\n"
		"  - Sticker system\n"
		"  - Color picker (pipette)\n"
		"  - Import / Export (JPG, PNG)\n"
		"  - Undo / Redo history\n"
		" \n"
		"KEYBOARD SHORTCUTS:\n"
		"  [ESC]:      QUIT\n"
		"  [CTRL + Z]: UNDO\n"
		"  [CTRL + Y]: REDO\n"
		"  [CTRL + C]: COPY a selection\n"
		"  [CTRL + X]: CUT a selection\n"
		"  [CTRL + V]: PASTE a selection as new layer\n"
		"  [E]:        EXPORT canvas as JPG\n"
		"  [R]:        EXPORT canvas as PNG\n"
		"  [N]:        DOWNGRADE active layer\n"
		"  [M]:        UPGRADE active layer\n"
		"  [B]:        BRUSH tool\n"
		"  [P]:        PENCIL tool\n"
		"  [L]:        LINE tool\n"
		"  [O]:        CIRCLE tool\n"
		"  [U]:        RECTANGLE tool\n"
		"  [I]:        BUCKET tool\n"
		"  [T]:        TEXT tool\n"
		" \n"
		"TECH STACK:\n"
		"  Language:   C (compiled with cc -Wall -Wextra -Werror)\n"
		"  Rendering:  SDL2\n"
		"  Text:       SDL2_ttf + FreeType\n"
		"  Images:     SDL2_image (JPG / PNG)\n"
		"  UI Engine:  libui (custom, built on SDL2)\n"
		"  Utilities:  libft (custom libc replacement)\n"
		"  Styling:    CSS-like config file (style.css)\n"
		" \n"
		"ARCHITECTURE:\n"
		"  main.c       Entry point, event callbacks\n"
		"  build.c      Widget tree construction\n"
		"  draws.c      Drawing tools logic\n"
		"  utils.c      Color, math helpers\n"
		"  css_parser.c CSS style file parser\n"
		"  css_apply.c  Applies styles to widgets\n"
		"  about.c      This modal\n"
		"  libui/       Custom widget engine (core,\n"
		"               box, text, input, engine)\n"
		"  t_data       Central app state struct\n";

	ui_create_prefab_modal_info_window(data->core, data->screen_width - 100,
		data->screen_height - 100, LIST_CHILDS_VERTICAL, about_text, 18);
}

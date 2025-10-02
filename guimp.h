/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guimp.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:25:14 by injah             #+#    #+#             */
/*   Updated: 2025/10/02 06:18:52 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUIMP_H
# define GUIMP_H

# include "libui/libui.h"
# include <unistd.h>
# include <stdio.h>

# define	MENUBAR_BUTTON_PADDING	(t_offset){5, 2, 5, 2}

enum e_top_buttons
{
	FILE_BUTTON,
	EDIT_BUTTON,
	VIEW_BUTTON,
};

typedef struct	s_data
{
	t_context		main_context;
	int			running;

	t_core		core;
	t_context		*rendering;
	t_context		*tools;
	t_box	*menubar;
	t_box	*tool_pallet;
	t_box	*tool_options;
	t_widget	*menubar_buttons[3];
}				t_data;


#endif
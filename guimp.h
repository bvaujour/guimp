/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guimp.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 10:25:14 by injah             #+#    #+#             */
/*   Updated: 2025/09/29 10:34:26 by injah            ###   ########.fr       */
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
	t_ctx		main_ctx;
	int			running;

	t_core		core;
	t_ctx		*rendering;
	t_ctx		*tools;
	t_container	*menubar;
	t_widget	*menubar_buttons[3];
}				t_data;


#endif
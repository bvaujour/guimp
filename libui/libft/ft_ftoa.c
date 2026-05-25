/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 13:00:52 by injah             #+#    #+#             */
/*   Updated: 2026/03/23 13:25:22 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_ftoa(float nbr, int precision)
{
	int		left_part;
	float	right_part;
	char	*str;
	int		i;

	i = 0;
	left_part = (int)nbr;
	right_part = nbr - left_part;
	while (i < precision)
	{
		right_part *= 10;
		i++;
	}
	str = ft_itoa(left_part);
	str = ft_strjoin(str, ".", 1);
	str = ft_strjoin(str, ft_itoa(right_part), 3);
	return (str);
}
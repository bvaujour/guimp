/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 12:58:47 by bvaujour          #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dest;

	dest = malloc(sizeof(char) * (ft_strlen((char *)s) + 1));
	if (dest == 0)
		return (0);
	ft_strcpy(dest, (char *)s);
	return (dest);
}

/*int	main()
{
	char src[] = "testestestestestste";
	char *dest;
	
	dest = ft_strdup(src);
	ft_printf ("%s", dest);
	free(dest);
	return (0);
}*/

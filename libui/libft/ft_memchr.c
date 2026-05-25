/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 14:00:39 by bvaujour          #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const char	*tmps;
	char		tmpc;
	size_t		i;

	tmps = s;
	tmpc = c;
	i = 0;
	while (i < n)
	{
		if (tmps[i] == tmpc)
			return ((char *)s + i);
		i++;
	}
	return (0);
}

/*int	main()
{
	char	*str = "bonjour tout le monde";
	char	c = 'z';
	
	ft_printf("%p\n", memchr(str, c, 150));
	ft_printf("%p\n", ft_memchr(str, c, 150));
}*/

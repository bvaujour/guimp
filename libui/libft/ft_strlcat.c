/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 12:48:15 by bvaujour          #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	unsigned int	size_dest;
	unsigned int	size_src;
	unsigned int	i;

	if (size == 0)
		return (ft_strlen(src));
	i = 0;
	size_dest = ft_strlen(dst);
	size_src = ft_strlen((char *)src);
	if (size <= size_dest)
		return (size + size_src);
	while (src[i] != '\0' && size_dest + i < size - 1)
	{
		dst[size_dest + i] = src[i];
		i++;
	}
	dst[size_dest + i] = '\0';
	return (size_dest + size_src);
}

/*#include <stdio.h>
#include <bsd/string.h>

int	main()
{
	char	src[50] = "hello world";
	char	dest[50] = ", salut le monde";
	ft_printf("%s\n", src);
	ft_printf("%s\n", dest);
	ft_printf("%ld\n", ft_strlcat(dest, src, 30));
	ft_printf("%ld\n", strlcat(dest, src, 30));
	ft_printf("%s\n", src);
	ft_printf("%s\n", dest);
	return (0);
}*/

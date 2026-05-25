/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvaujour <bvaujour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 14:45:42 by bvaujour          #+#    #+#             */
/*   Updated: 2026/05/06 12:24:53 by bvaujour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2, int option)
{
	char	*join;

	if (!s1 && s2)
		return (s2);
	if (!s2 && s1)
		return (s1);
	if (!s1 && !s2)
		return (NULL);
	join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (join == 0)
		return (0);
	join[0] = '\0';
	ft_strcat(join, (char *)s1);
	ft_strcat(join, (char *)s2);
	if (option == 1)
		free(s1);
	else if (option == 2)
		free(s2);
	else if (option == 3)
	{
		free(s1);
		free(s2);
	}
	return (join);
}

/*int	main()
{
	char	*s1 = "hello";
	char	*s2 = NULL;
	
	ft_printf ("%s\n", ft_strjoin(s1, s2, 0));
	ft_printf ("%d\n", ft_strlen(ft_strjoin(s1, s2, 0)));
}*/

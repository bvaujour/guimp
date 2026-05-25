/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 16:34:31 by injah             #+#    #+#             */
/*   Updated: 2026/03/26 17:00:15 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_insert(char *buffer, char *to_insert, int insert_at, int max_size)
{
	int	i;
	int	len_insert;
	int	len_buffer;

	len_insert = ft_strlen(to_insert);
	len_buffer = ft_strlen(buffer);

	// Vérifications simples
	if (insert_at < 0 || insert_at > len_buffer || len_buffer + len_insert >= max_size)
		return;

	// Décaler vers la droite (de la fin vers le début)
	i = len_buffer;
	while (i >= insert_at)
	{
		buffer[i + len_insert] = buffer[i];
		i--;
	}

	// Copier la chaîne à insérer
	i = 0;
	while (to_insert[i])
	{
		buffer[insert_at + i] = to_insert[i];
		i++;
	}
}
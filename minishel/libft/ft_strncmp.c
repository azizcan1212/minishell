/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:24:04 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:28:59 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
	{
		return (0);
	}
	while (s1[i] && s1[i] == s2[i] && i < size)
	{
		i ++;
	}
	if (i == size)
		return (0);
	return (((unsigned char *) s1)[i] - ((unsigned char *)s2)[i]);
}

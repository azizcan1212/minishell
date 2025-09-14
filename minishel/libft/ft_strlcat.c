/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:23:44 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:28:36 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (size == 0)
	{
		while (src[i])
			i++;
		return (i);
	}
	while (dst[i] && (i < size))
		i ++;
	while (src[j] && (i + j + 1 < size))
	{
		dst[i + j] = src[j];
		j ++;
	}
	if (i + j < size)
		dst[i + j] = '\0';
	while (src[j])
		j ++;
	return (i + j);
}

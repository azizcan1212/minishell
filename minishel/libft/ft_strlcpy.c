/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 23:16:37 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:28:44 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
	{
		while (src[i])
			i ++;
		return (i);
	}
	while (src[i] && (i < size - 1))
	{
		dst[i] = src[i];
		i ++;
	}
	dst[i] = '\0' ;
	while (src[i])
		i++;
	return (i);
}

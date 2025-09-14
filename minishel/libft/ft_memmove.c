/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:26:37 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:27:37 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	size_t	length;

	if (!dst && !src)
		return (NULL);
	if (!(dst < src))
	{
		length = n;
		while (length > 0)
		{
			length --;
			((unsigned char *)dst)[length] = ((unsigned char *)src)[length];
		}
	}
	else
	{
		length = 0 ;
		while (length < n)
		{
			((unsigned char *)dst)[length] = ((unsigned char *)src)[length];
			length++;
		}
	}
	return (dst);
}

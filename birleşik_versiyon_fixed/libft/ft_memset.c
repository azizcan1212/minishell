/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:23:06 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:27:43 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *arr, int c, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		((unsigned char *)arr)[i] = (unsigned char)c;
		i ++;
	}
	return (arr);
}

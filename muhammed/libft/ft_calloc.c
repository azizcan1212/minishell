/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:22:13 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:37:29 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_calloc(size_t unit, size_t size)
{
	size_t	new_size;
	void	*b;

	new_size = unit * size;
	b = malloc(new_size);
	if (b == NULL)
		return (NULL);
	ft_bzero(b, new_size);
	return (b);
}

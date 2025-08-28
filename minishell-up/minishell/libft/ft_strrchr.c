/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:24:09 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:36:54 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = (char)c;
	while (s[i])
	{
		i++;
	}
	if (c == '\0')
		return ((char *)&s[i]);
	while (i > 0)
	{
		if (s[i - 1] == c)
		{
			return ((char *)&s[i - 1]);
		}
		i--;
	}
	return (NULL);
}

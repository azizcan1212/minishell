/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:23:30 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:28:16 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	c = (char)c;
	while (s[i])
	{
		if (c == s[i])
		{
			return ((char *)&s[i]);
		}
		i ++;
	}
	if (c == '\0')
	{
		return ((char *) &s[i]);
	}
	return (NULL);
}

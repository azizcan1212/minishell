/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:23:34 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:28:19 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	char	*str;
	int		len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	str = (char *)malloc(len +1);
	if (str == NULL)
	{
		return (NULL);
	}
	ft_strlcpy(str, s1, len + 1);
	return (str);
}

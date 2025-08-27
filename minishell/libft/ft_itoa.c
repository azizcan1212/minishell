/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:22:46 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:27:15 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static unsigned int	lenght(int b)
{
	unsigned int	i;

	i = 0;
	if (b < 0)
		i++;
	if (b == 0)
		return (1);
	while (b != 0)
	{
		b /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char			*value;
	int				i;
	long			t;
	unsigned int	len;

	i = 0;
	t = n;
	len = lenght(n);
	value = (char *)malloc(len + 1);
	if (value == NULL)
		return (NULL);
	if (n < 0)
	{
		value[i++] = '-';
		t *= -1;
	}
	if (n == 0)
		value[0] = '0';
	value[len] = '\0';
	while (t != 0)
	{
		value[--len] = (t % 10) + '0';
		t = t / 10;
	}
	return (value);
}

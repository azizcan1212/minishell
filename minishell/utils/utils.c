/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:16 by muharsla          #+#    #+#             */
/*   Updated: 2025/07/28 12:52:17 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft/libft.h"

char *ft_strndup(const char *s, unsigned int n)
{
	unsigned int len;
	char *dup;

	if (!s)
		return NULL;

	len = ft_strlen(s);
	if (len > n)
		len = n;

	dup = (char *)malloc(len + 1);
	if (!dup)
		return NULL;

	ft_strlcpy(dup, s, len + 1);
	return dup;
}

int ft_isspace(char c)
{
	return (
		c == ' '  ||  // space
		c == '\t' ||  // tab
		c == '\n' ||  // newline
		c == '\v' ||  // vertical tab
		c == '\f' ||  // form feed
		c == '\r' ||    // carriage return
		c == '\\' ||  // backslash
		c == ';' // semicolon

	);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_four.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:00:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 05:01:43 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdlib.h>

char	*gc_strdup(const char *s)
{
	char	*dup;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strcpy(dup, s);
	return (dup);
}

static size_t	get_min_len(size_t s_len, size_t n)
{
	if (s_len > n)
		return (n);
	return (s_len);
}

char	*gc_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = get_min_len(ft_strlen(s), n);
	dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

static size_t	calculate_substr_len(const char *s, unsigned int start,
	size_t len)
{
	size_t	s_len;

	s_len = ft_strlen(s);
	if (start >= s_len)
		return (0);
	if (len > s_len - start)
		return (s_len - start);
	return (len);
}

char	*gc_substr(const char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	actual_len;
	size_t	i;

	if (!s)
		return (NULL);
	actual_len = calculate_substr_len(s, start, len);
	if (actual_len == 0)
		return (gc_strdup(""));
	substr = gc_malloc(actual_len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < actual_len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[actual_len] = '\0';
	return (substr);
}

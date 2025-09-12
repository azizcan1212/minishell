/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_five.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:00:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 04:53:27 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdlib.h>

static void	copy_strings(char *joined, const char *s1, const char *s2)
{
	size_t	len1;
	size_t	i;
	size_t	j;

	len1 = ft_strlen(s1);
	i = 0;
	while (i < len1)
	{
		joined[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		joined[i + j] = s2[j];
		j++;
	}
	joined[i + j] = '\0';
}

char	*gc_strjoin(const char *s1, const char *s2)
{
	char	*joined;
	size_t	total_len;

	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	joined = gc_malloc(total_len + 1);
	if (!joined)
		return (NULL);
	copy_strings(joined, s1, s2);
	return (joined);
}

static size_t	count_words(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static void	fill_result_array(char **result, const char *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	result_i;

	i = 0;
	result_i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			result[result_i++] = gc_substr(s, start, i - start);
		}
		else
			i++;
	}
	result[result_i] = NULL;
}

char	**gc_split(const char *s, char c)
{
	char	**result;
	size_t	count;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	result = gc_malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	fill_result_array(result, s, c);
	return (result);
}

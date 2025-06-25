/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:52:23 by atam              #+#    #+#             */
/*   Updated: 2024/11/06 15:28:10 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void	free_arr(char **arr, int i)
{
	while (i > 0)
		free(arr[--i]);
	free(arr);
}

static int	word_count(char const *word, char c)
{
	int	i;
	int	number_of_word;

	i = 0;
	number_of_word = 0;
	while (word[i])
	{
		if (word[i] == c)
			i++;
		else
		{
			number_of_word ++;
			while (word[i] && word[i] != c)
				i++;
		}
	}
	return (number_of_word);
}

static int	word_size(char const *word, char c)
{
	int	size_of_word;

	size_of_word = 0;
	while (word[size_of_word] && word[size_of_word] != c)
		size_of_word++;
	return (size_of_word);
}

static char	**column_arr(char **arr, char const *word, char c, int count_word)
{
	int	i;
	int	j;
	int	word_len;

	i = 0;
	while (i < count_word)
	{
		while (*word == c)
			word++;
		word_len = word_size(word, c);
		arr[i] = (char *)malloc(sizeof(char) * (word_len + 1));
		if (!arr[i])
		{
			free_arr(arr, i);
			return (NULL);
		}
		j = 0;
		while (word_len-- > 0 && *word && *word != c)
			arr[i][j++] = *word++;
		arr[i][j] = '\0';
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(char const *word, char c)
{
	char			**arr;
	unsigned int	num_words;

	if (!word)
		return (NULL);
	num_words = word_count(word, c);
	arr = (char **)malloc(sizeof(char *) * (num_words + 1));
	if (!arr)
		return (NULL);
	arr = column_arr(arr, word, c, num_words);
	return (arr);
}

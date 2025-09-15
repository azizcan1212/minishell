/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_env_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:22:38 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/15 18:03:38 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gc.h"
#include "libft/libft.h"
#include <stdio.h>

char	*create_env_entry(char *key, char *value)
{
	char	*entry;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	entry = gc_malloc(key_len + 1 + value_len + 1);
	if (!entry)
	{
		perror("minishell: update_env_value malloc failed");
		gc_cleanup();
		exit(EXIT_FAILURE);
	}
	ft_strcpy(entry, key);
	entry[key_len] = '=';
	ft_strcpy(entry + key_len + 1, value);
	return (entry);
}

int	key_matches(char *entry, char *key)
{
	char	*eq;
	size_t	key_len;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (0);
	key_len = eq - entry;
	if (ft_strlen(key) != key_len)
		return (0);
	if (ft_strncmp(entry, key, key_len) != 0)
		return (0);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:14:53 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 05:16:54 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <string.h>

void	reset_updated_flag(t_expansion *expansion)
{
	t_expansion	*cur;

	cur = expansion;
	while (cur)
	{
		cur->up = 0;
		cur = cur->next;
	}
}

static char	*create_env_entry(char *key, char *value)
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

int	update_env_value(char *key, char *value, char **envp)
{
	int		i;
	char	*eq;
	size_t	key_len;

	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			key_len = eq - envp[i];
			if (ft_strlen(key) == key_len && !ft_strncmp(envp[i], key, key_len))
			{
				envp[i] = create_env_entry(key, value);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	update_dup_key(t_expansion *expansion, char **envp)
{
	t_expansion	*current;
	int			count;
	int			result;

	current = expansion;
	count = 0;
	while (current)
	{
		if (current->export == 1)
		{
			result = update_env_value(current->key, current->value, envp);
			if (result)
				current->up = 1;
			count++;
		}
		current = current->next;
	}
	return (count);
}

int	count_exported_entries_with_equal(t_expansion *expansion)
{
	t_expansion	*current;
	int			count;

	current = expansion;
	count = 0;
	while (current)
	{
		if (current->export == 1 && current->value)
			count++;
		current = current->next;
	}
	return (count);
}

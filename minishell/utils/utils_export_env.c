/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:14:53 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 20:26:44 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"

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

int	update_env_value(char *key, char *value, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (key_matches(envp[i], key))
		{
			envp[i] = create_env_entry(key, value);
			return (1);
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

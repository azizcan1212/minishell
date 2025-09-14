/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:40:29 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 17:52:31 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"

int	count_exported_entries(t_expansion *expansion)
{
	t_expansion	*current;
	int			count;

	current = expansion;
	count = 0;
	while (current)
	{
		if (current->export == 1)
			count++;
		current = current->next;
	}
	return (count);
}

char	**create_exported_keys_array(t_expansion *expansion, int count)
{
	char		**keys;
	t_expansion	*current;
	int			i;

	keys = gc_malloc(sizeof(char *) * count);
	if (!keys)
		return (NULL);
	current = expansion;
	i = 0;
	while (current)
	{
		if (current->export == 1)
			keys[i++] = current->key;
		current = current->next;
	}
	return (keys);
}

int	count_new_entries(t_expansion *expansion)
{
	t_expansion	*cur;
	int			new_count;

	cur = expansion;
	new_count = 0;
	while (cur)
	{
		if (cur->export == 1 && cur->value && cur->up == 0)
			new_count++;
		cur = cur->next;
	}
	return (new_count);
}

void	add_new_entries_to_env_end(t_expansion *expansion, char **envp)
{
	t_expansion	*cur;
	int			i;
	char		*tmp;

	i = 0;
	while (envp[i])
		i++;
	cur = expansion;
	while (cur)
	{
		if (cur->export == 1 && cur->value && cur->up == 0)
		{
			tmp = gc_strjoin(cur->key, "=");
			envp[i++] = gc_strjoin(tmp, cur->value);
			cur->up = 1;
		}
		cur = cur->next;
	}
	envp[i] = NULL;
}

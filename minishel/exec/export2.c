/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:50:11 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 17:51:04 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_to_env_end(t_expansion *expansion, char **envp)
{
	update_dup_key(expansion, envp);
	if (count_new_entries(expansion) > 0)
		add_new_entries_to_env_end(expansion, envp);
	reset_updated_flag(expansion);
}

void	sort_str_array(char **arr, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_expansion_by_key(t_expansion *expansion, char *key)
{
	t_expansion	*current;

	current = expansion;
	while (current)
	{
		if (current->key && !ft_strcmp(current->key, key))
		{
			if (current->value)
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
			else
				printf("declare -x %s\n", current->key);
			break ;
		}
		current = current->next;
	}
}

void	print_export(t_expansion *expansion)
{
	char	**keys;
	int		count;
	int		i;

	count = count_exported_entries(expansion);
	if (count == 0)
		return ;
	keys = create_exported_keys_array(expansion, count);
	if (!keys)
		return ;
	sort_str_array(keys, count);
	i = 0;
	while (i < count)
	{
		print_expansion_by_key(expansion, keys[i]);
		i++;
	}
}

int	builtin_export(char **args, t_expansion **expansion, char **envp)
{
	int	status;

	if (!args[1])
	{
		print_export(*expansion);
		return (0);
	}
	status = set_export(args, expansion);
	copy_to_env_end(*expansion, envp);
	return (status);
}

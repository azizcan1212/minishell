/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:40:29 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
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

static int	count_new_entries(t_expansion *expansion)
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

static void	add_new_entries_to_env_end(t_expansion *expansion, char **envp)
{
	t_expansion	*cur;
	int			i;
	char		*tmp;

	/* env uzunluğunu bul */
	i = 0;
	while (envp[i])
		i++;
	/* sona ekle */
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
	if (!args[1])
	{
		print_export(*expansion);
		return (0);
	}
	if (set_export(args, expansion) != 0)
		return (1);
	copy_to_env_end(*expansion, envp);
	return (0);
}
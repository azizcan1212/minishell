/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:40:29 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 15:56:57 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	count_expansion_entries(t_expansion *expansion)
{
	t_expansion	*current;
	int			count;

	count = 0;
	current = expansion;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	**create_keys_array(t_expansion *expansion, int count)
{
	char		**keys;
	t_expansion	*current;
	int			i;

	keys = malloc(sizeof(char *) * count);
	if (!keys)
		return (NULL);
	i = 0;
	current = expansion;
	while (current)
	{
		keys[i] = current->key;
		current = current->next;
		i++;
	}
	return (keys);
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
			break;
		}
		current = current->next;
	}
}

void	print_sorted_expansion(t_expansion *expansion)
{
	char	**keys;
	int		count;
	int		i;

	count = count_expansion_entries(expansion);
	if (count == 0)
		return;
	keys = create_keys_array(expansion, count);
	if (!keys)
		return;
	sort_str_array(keys, count);
	i = 0;
	while (i < count)
	{
		print_expansion_by_key(expansion, keys[i]);
		i++;
	}
	free(keys);
}

int	builtin_export(char **args, t_expansion *expansion)
{
	int	i;

	if (!args[1])
	{
		print_sorted_expansion(expansion);
		return (0);
	}
	
	// Validate all arguments
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			write(2, "export: `", 9);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': not a valid identifier\n", 26);
			return (1);
		}
		i++;
	}
	return (0);
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

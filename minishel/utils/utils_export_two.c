/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:12:28 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 20:30:12 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include "../gc.h"

void	equal_dup_export(t_expansion *expansion, char *key, char *value)
{
	t_expansion	*current;

	current = expansion;
	while (current)
	{
		if (current->key && !ft_strcmp(current->key, key)
			&& current->export == 1)
		{
			current->value = gc_strdup(value);
			if (!current->value)
			{
				perror("minishell: export: memory allocation failed");
				gc_cleanup();
				exit(EXIT_FAILURE);
			}
			return ;
		}
		current = current->next;
	}
}

static int	process_export_arg(char **arg, int i, t_expansion **expansion)
{
	int	j;

	if (!is_valid_export(arg[i]))
	{
		print_invalid_identifier(arg[i]);
		return (1);
	}
	j = 0;
	if (check_include_equal(arg, &j, &i))
		return (process_export_equal(arg[i], expansion));
	return (process_export_no_equal(arg[i], expansion));
}

int	set_export(char **arg, t_expansion **expansion)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (arg[i])
	{
		if (process_export_arg(arg, i, expansion))
			status = 1;
		i++;
	}
	return (status);
}

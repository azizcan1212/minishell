/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:12:28 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 06:44:44 by atam             ###   ########.fr       */
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

static int	handle_no_equal(char *arg, t_expansion **expansion)
{
	t_expansion	*n_expansion;
	char		*key;

	key = gc_strdup(arg);
	if (!key)
		return (1);
	if (!control_duplicate_export(*expansion, key))
	{
		n_expansion = new_expansion(NULL, key);
		if (!n_expansion)
			return (1);
		n_expansion->export = 1;
		add_expansion_back(expansion, n_expansion);
	}
	return (0);
}

static int	process_export_equal(char *arg, t_expansion **expansion)
{
	int		j;
	char	*key;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	key = gc_substr(arg, 0, j);
	if (!key)
		return (1);
	if (!ft_strcmp(key, "_"))
		return (0);
	if (!control_duplicate_export(*expansion, key))
		return (export_manage_equal(arg, expansion, &j));
	else
		equal_dup_export(*expansion, key, arg + j + 1);
	return (0);
}

static int	process_export_no_equal(char *arg, t_expansion **expansion)
{
	if (!ft_strcmp(arg, "_"))
		return (0);
	return (handle_no_equal(arg, expansion));
}

int	set_export(char **arg, t_expansion **expansion)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (arg[i])
	{
		if (!is_valid_export(arg[i]))
		{
			write(2, "minishell: export: `", 20);
			write(2, arg[i], ft_strlen(arg[i]));
			write(2, "': not a valid identifier\n", 26);
			status = 1;
		}
		else if (check_include_equal(arg, &(int){0}, &i))
		{
			if (process_export_equal(arg[i], expansion))
				status = 1;
		}
		else if (process_export_no_equal(arg[i], expansion))
			status = 1;
		i++;
	}
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:13:46 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 05:14:35 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include "../gc.h"

int	check_include_equal(char **str, int *j, int *i)
{
	while (str[*i][*j])
	{
		if (str[*i][*j] == '=')
			return (1);
		(*j)++;
	}
	return (0);
}

int	is_valid_export(char *str)
{
	int	k;

	if (!str || !str[0])
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	k = 0;
	while (str[k] && str[k] != '=')
	{
		if (!(ft_isalnum(str[k]) || str[k] == '_'))
			return (0);
		k++;
	}
	return (1);
}

int	control_duplicate_export(t_expansion *expansion, char *key)
{
	t_expansion	*current;

	current = expansion;
	while (current)
	{
		if (current->key && !ft_strcmp(current->key, key)
			&& current->export == 1)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	create_and_add_expansion(char *key, char *value,
		t_expansion **expansion)
{
	t_expansion	*n_expansion;

	n_expansion = new_expansion(value, key);
	if (!n_expansion)
		return (1);
	n_expansion->export = 1;
	add_expansion_back(expansion, n_expansion);
	if (value)
		setenv(key, value, 1);
	return (0);
}

int	export_manage_equal(char *arg, t_expansion **expansion, int *j)
{
	char	*key;
	char	*value;

	key = gc_substr(arg, 0, *j);
	if (!key)
		return (1);
	value = gc_strdup(arg + *j + 1);
	if (!value)
		return (1);
	return (create_and_add_expansion(key, value, expansion));
}

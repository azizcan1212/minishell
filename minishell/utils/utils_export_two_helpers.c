/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export_two_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:22:14 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 20:23:21 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include "../gc.h"

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

int	process_export_equal(char *arg, t_expansion **expansion)
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

int	process_export_no_equal(char *arg, t_expansion **expansion)
{
	if (!ft_strcmp(arg, "_"))
		return (0);
	return (handle_no_equal(arg, expansion));
}

void	print_invalid_identifier(char *s)
{
	write(2, "minishell: export: `", 20);
	write(2, s, ft_strlen(s));
	write(2, "': not a valid identifier\n", 26);
}

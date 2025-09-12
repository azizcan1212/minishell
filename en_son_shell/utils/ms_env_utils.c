/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:21:13 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 20:25:29 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"

t_expansion	*find_env_var(t_expansion *expansion, const char *name)
{
	t_expansion	*current;

	current = expansion;
	while (current)
	{
		if (current->key && !ft_strcmp(current->key, name))
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	update_existing_var(t_expansion *var, const char *value,
	int overwrite, int export_flag)
{
	if (overwrite)
	{
		var->value = gc_strdup(value);
		if (!var->value)
			return (-1);
		var->export = export_flag;
	}
	return (0);
}

int	create_new_var(t_shell_val *shell, const char *name,
	const char *value, int export_flag)
{
	t_expansion	*new_exp;
	char		*key;
	char		*val;

	key = gc_strdup(name);
	val = gc_strdup(value);
	if (!key || !val)
		return (-1);
	new_exp = new_expansion(val, key);
	if (!new_exp)
		return (-1);
	new_exp->export = export_flag;
	add_expansion_back(&(shell->expansion), new_exp);
	return (0);
}

/* moved envp builders to utils/ms_env_build.c */

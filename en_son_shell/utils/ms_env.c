/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:00:00 by azizcan           #+#    #+#             */
/*   Updated: 2025/09/12 20:27:17 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"

static t_shell_val	**get_shell_instance(void)
{
	static t_shell_val	*instance = NULL;

	return (&instance);
}

void	ms_setenv_init(t_shell_val *shell_val)
{
	t_shell_val	**instance;

	instance = get_shell_instance();
	*instance = shell_val;
}

int	ms_setenv(const char *name, const char *value, int overwrite)
{
	t_shell_val	**instance;
	t_expansion	*existing;

	if (!name || !value)
		return (-1);
	instance = get_shell_instance();
	if (!*instance)
		return (-1);
	existing = find_env_var((*instance)->expansion, name);
	if (existing)
		return (update_existing_var(existing, value, overwrite, 1));
	return (create_new_var(*instance, name, value, 1));
}

int	ms_set_var(const char *name, const char *value, int overwrite)
{
	t_shell_val	**instance;
	t_expansion	*existing;

	if (!name || !value)
		return (-1);
	instance = get_shell_instance();
	if (!*instance)
		return (-1);
	existing = find_env_var((*instance)->expansion, name);
	if (existing)
		return (update_existing_var(existing, value, overwrite, 0));
	return (create_new_var(*instance, name, value, 0));
}

char	*ms_getenv(const char *name)
{
	t_shell_val	**instance;
	t_expansion	*found;

	if (!name)
		return (NULL);
	instance = get_shell_instance();
	if (!*instance)
		return (getenv(name));
	found = find_env_var((*instance)->expansion, name);
	if (found)
		return (found->value);
	return (getenv(name));
}

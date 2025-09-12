/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:00:00 by azizcan           #+#    #+#             */
/*   Updated: 2025/09/12 06:45:07 by atam             ###   ########.fr       */
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

static t_expansion	*find_env_var(t_expansion *expansion, const char *name)
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

static int	update_existing_var(t_expansion *var, const char *value,
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

static int	create_new_var(t_shell_val *shell, const char *name,
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



static char	*create_env_string(t_expansion *exp)
{
	char	*temp;
	char	*result;

	temp = gc_strjoin(exp->key, "=");
	if (!temp)
		return (NULL);
	result = gc_strjoin(temp, exp->value);
	return (result);
}

static void	fill_envp_array(char **envp, t_expansion *expansion, int count)
{
	t_expansion	*current;
	int			i;

	i = 0;
	current = expansion;
	while (current && i < count)
	{
		if (current->export == 1 && current->value)
		{
			envp[i] = create_env_string(current);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
}

char	**build_envp_from_expansion(t_expansion *expansion)
{
	char	**envp;
	int		count;

	count = count_exported_entries_with_equal(expansion);
	envp = gc_malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	fill_envp_array(envp, expansion, count);
	return (envp);
}

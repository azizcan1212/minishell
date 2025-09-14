/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:35:00 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 20:36:38 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc.h"

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

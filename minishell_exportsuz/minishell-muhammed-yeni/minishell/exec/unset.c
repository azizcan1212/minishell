/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:00:00 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 17:05:05 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	is_valid_identifier(const char *str)
{
	int		i;

	if (!str || !(str[0]))
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	unset_env(const char *key, t_expansion **env)
{
	t_expansion		*tmp;
	t_expansion		*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (tmp->key && !ft_strcmp(tmp->key, key))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			if (tmp->key)
				free(tmp->key);
			if (tmp->value)
				free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	builtin_unset(char **args, t_expansion **env)
{
	int		i;

	i = 1;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			write(2, "unset: not a valid identifier\n", 30);
			return (1);
		}
		if (env)
			unset_env(args[i], env);
		i++;
	}
	return (0);
}

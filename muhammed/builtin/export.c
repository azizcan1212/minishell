/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:18:15 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/04 09:50:35 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft.h"

static int	is_valid_identifier(const char *str)
{
	int i;

	if (!str || !(str[0]))
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **args, t_expansıon **env)
{
	int i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			write(2, "export: not a valid identifier\n", 29);
			return (1);
		}
		set_env(new_token(args[i]), env, 0);
		i++;
	}
	return (0);
}


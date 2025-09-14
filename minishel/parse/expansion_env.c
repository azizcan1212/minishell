/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:05:42 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 20:13:45 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"

int	set_env(t_token *new_token, t_expansion **expansion, int *i)
{
	char	*key;
	char	*value;
	int		end;
	int		result;

	(void)expansion;
	end = ft_strlen(new_token->value) - *i - 1;
	key = gc_substr(new_token->value, 0, *i);
	value = gc_substr(new_token->value, *i + 1, end);
	result = ms_set_var(key, value, 1);
	if (result != 0)
		return (1);
	return (0);
}

int	set_environment(t_token *token,
		t_expansion **expansion, t_equal equal_status)
{
	t_token	*new_token;
	int		i;
	int		result;

	new_token = token;
	while (new_token && equal_status == VALID)
	{
		i = 0;
		while (new_token->value[i] && new_token->value[i] != '=')
			i++;
		if (new_token->equal_status == VALID)
		{
			result = set_env(new_token, expansion, &i);
			if (result != 0)
				return (1);
		}
		new_token = new_token->next;
	}
	return (0);
}

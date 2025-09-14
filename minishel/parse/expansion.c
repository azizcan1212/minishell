/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 03:29:40 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 20:14:24 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <sys/types.h>

/* moved to expansion_env.c: set_env, set_environment */

char	*get_new_value(char *value, int i, t_shell_val *val)
{
	char	*new_value;
	char	*ex;
	char	*key;
	char	*temp;

	ex = gc_substr(value, 0, i);
	key = ft_strdup_dollar(value + i + 1);
	new_value = dollar_value(key, val);
	if (!new_value)
		new_value = gc_strdup("");
	i += ft_strlen(key) + 1;
	temp = gc_strjoin(ex, new_value);
	ex = gc_substr(value, i, ft_strlen(value) - i);
	if (!ex)
		return (temp);
	new_value = gc_strjoin(temp, ex);
	return (new_value);
}

char	*dollar_value(char *key, t_shell_val *val)
{
	char	*temp_value;
	int		last_exit_status;

	if (key[0] == '?')
	{
		last_exit_status = val->last_exit_status;
		return (gc_itoa(last_exit_status));
	}
	else if (ft_isdigit(key[0]))
		return (gc_strdup(&key[1]));
	else
	{
		temp_value = ms_getenv(key);
		if (temp_value == NULL)
			return (gc_strdup(""));
		return (gc_strdup(temp_value));
	}
}

static void	expand_token_value(t_token *tok, t_shell_val *val)
{
	int	i;

	i = 0;
	if (tok->value && tok->type != SINGLE_QUOTE)
	{
		while (tok->value[i])
		{
			if (tok->value[i] == '$' && is_valid_var_start(&tok->value[i]))
			{
				tok->value = get_new_value(tok->value, i, val);
				i = 0;
			}
			else
				i++;
		}
	}
}

void	expand_dollar(t_token *token, t_shell_val *val)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (cur->prev && cur->prev->type == META
			&& cur->prev->value && !ft_strcmp(cur->prev->value, "<<"))
		{
			cur = cur->next;
			continue ;
		}
		expand_token_value(cur, val);
		cur = cur->next;
	}
}

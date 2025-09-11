/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 03:29:40 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 03:29:47 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <sys/types.h>

int	set_env(t_token *new_token, t_expansion **expansion, int *i)
{
	char		*key;
	char		*value;
	int			end;
	int			result;

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
	t_expansion **expansion, t_equal_status equal_status)
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
	pid_t	pid;
	char	*temp_value;
	int		last_exit_status;

	pid = getpid();
	if (key[0] == '$')
		return (gc_itoa(pid));
	else if (key[0] == '?')
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

void	expand_dollar(t_token *token, t_shell_val *val)
{
	t_token		*new_token;
	int			i;

	new_token = token;
	while (new_token)
	{
		i = 0;
		if (new_token->value && new_token->type != SINGLE_QUOTE)
		{
			while (new_token->value[i])
			{
				if (new_token->value[i] == '$'
					&& is_valid_var_start(&new_token->value[i]))
				{
					new_token->value = get_new_value(new_token->value, i, val);
					i = 0;
				}
				else
					i++;
			}
		}
		new_token = new_token->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:44:45 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/10 04:20:32 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

const char	**def_metas(void)
{
	static const char	*meta[] = {
		"<<",
		">>",
		"&&",
		"||",
		"|",
		"<",
		">",
		"=",
		NULL
	};

	return (meta);
}

int	has_whitespace(const char *str)
{
	if (!str)
		return (0);
	if (ft_strchr(str, ' ') || ft_strchr(str, '\t')
		|| ft_strchr(str, '\n') || ft_strchr(str, '\r')
		|| ft_strchr(str, '\f') || ft_strchr(str, '\v'))
		return (1);
	return (0);
}

int	is_redirect_token(t_token *token)
{
	if (!token || !token->value)
		return (0);
	return (token->type == META && (!ft_strcmp(token->value, "<")
			|| !ft_strcmp(token->value, ">") || !ft_strcmp(token->value, ">>")
			|| !ft_strcmp(token->value, "<<")));
}

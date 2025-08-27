/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:44:45 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 17:53:59 by muharsla         ###   ########.fr       */
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
	if (ft_strchr(str, ' ') || ft_strchr(str, '\t') || 
		ft_strchr(str, '\n') || ft_strchr(str, '\r') || 
		ft_strchr(str, '\f') || ft_strchr(str, '\v'))
		return (1);
	return (0);
}
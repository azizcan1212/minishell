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

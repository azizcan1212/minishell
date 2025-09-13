/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:56:24 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:43:34 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_std(int *sv_in, int *sv_out)
{
	int	a;
	int	b;

	a = dup(STDIN_FILENO);
	b = dup(STDOUT_FILENO);
	if (a < 0 || b < 0)
	{
		if (a >= 0)
			close(a);
		if (b >= 0)
			close(b);
		return (1);
	}
	*sv_in = a;
	*sv_out = b;
	return (0);
}

void	restore_std(int sv_in, int sv_out)
{
	if (sv_in >= 0)
	{
		dup2(sv_in, STDIN_FILENO);
		close(sv_in);
	}
	if (sv_out >= 0)
	{
		dup2(sv_out, STDOUT_FILENO);
		close(sv_out);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:56:24 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/15 17:42:47 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

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

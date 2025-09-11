/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:32:50 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/11 21:49:28 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"

static void	exit_with_msg(const char *pfx,
	const char *arg, const char *sfx, int code)
{
	if (pfx)
		write(2, pfx, ft_strlen(pfx));
	if (arg)
		write(2, arg, ft_strlen(arg));
	if (sfx)
		write(2, sfx, ft_strlen(sfx));
	gc_cleanup();
	exit(code);
}

// Mevcut builtin_exit fonksiyonunu değiştirin  
int	builtin_exit(char **args, t_command *cmd)
{
	long	result;
	int		exit_code;

	if (!args[1])
	{
		close_all_heredocs(cmd);
		fd_gc_cleanup(); // Manuel fd kapatma yerine
		gc_cleanup();
		exit(0);
	}
	if (args[2])
		exit_with_msg("bash: exit: too many arguments\n", NULL, NULL, 1);
	if (!is_numeric(args[1]))
		exit_with_msg("bash: exit: ", args[1],
			": numeric argument required\n", 2);
	result = ft_atoi(args[1]);
	exit_code = (unsigned char)(result % 256);
	if (result < 0)
		exit_code = 256 + (result % 256);
	close_all_heredocs(cmd);
	fd_gc_cleanup(); // Manuel fd kapatma yerine
	gc_cleanup();
	exit(exit_code);
}

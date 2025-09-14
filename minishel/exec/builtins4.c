/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:32:50 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/13 05:21:46 by muharsla         ###   ########.fr       */
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

static void	exit_core(char **args, t_command *cmd, int default_status)
{
	long	result;
	int		exit_code;

	if (!args[1])
	{
		close_all_heredocs(cmd);
		fd_gc_cleanup();
		gc_cleanup();
		exit(default_status);
	}
	if (args[2] && is_numeric(args[1]))
		exit_with_msg("bash: exit: too many arguments\n",
			NULL, NULL, 1);
	if (!is_numeric(args[1]))
		exit_with_msg("bash: exit: ", args[1],
			": numeric argument required\n", 2);
	result = ft_atoi(args[1]);
	exit_code = (unsigned char)(result % 256);
	if (result < 0)
		exit_code = 256 + (result % 256);
	close_all_heredocs(cmd);
	fd_gc_cleanup();
	gc_cleanup();
	exit(exit_code);
}

int	builtin_exit(char **args, t_command *cmd)
{
	exit_core(args, cmd, 0);
	return (0);
}

int	builtin_exit_parent(char **args, t_command *cmd, t_shell_val *val)
{
	int	def;

	if (args && args[1] && args[2] && is_numeric(args[1]))
	{
		(void)!write(2, "bash: exit: too many arguments\n", 31);
		if (val)
			val->last_exit_status = 1;
		return (1);
	}
	def = 0;
	if (val)
		def = val->last_exit_status;
	exit_core(args, cmd, def);
	return (0);
}

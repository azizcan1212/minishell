/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command12.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:36:30 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:30:15 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"
#include "gc.h"

void	child_setup_signals_and_redirs(t_command *cmd)
{
	g_signal_num = 0;
	signal(SIGINT, child_sigint_handler);
	signal(SIGPIPE, sigpipe_close_all);
	signal(SIGQUIT, SIG_DFL);
	child_redirections(cmd);
}

void	child_try_run_echo_env(t_command *cmd, char **envp)
{
	int	st;

	if (!cmd->cmd)
		return ;
	if (!ft_strcmp(cmd->cmd, "echo"))
	{
		st = builtin_echo(cmd->args);
		fd_gc_cleanup();
		gc_cleanup();
		_exit(st);
	}
	if (!ft_strcmp(cmd->cmd, "env"))
	{
		st = builtin_env(envp);
		fd_gc_cleanup();
		gc_cleanup();
		_exit(st);
	}
}

void	child_try_run_export(t_command *cmd, char **envp)
{
	int			st;
	t_shell_val	tmp;

	if (!cmd->cmd)
		return ;
	if (!ft_strcmp(cmd->cmd, "export"))
	{
		tmp.last_exit_status = 0;
		tmp.expansion = NULL;
		env_bootstrap_once(&tmp, envp);
		if (!cmd->args[1])
		{
			print_export(tmp.expansion);
			fd_gc_cleanup();
			gc_cleanup();
			_exit(0);
		}
		st = set_export(cmd->args, &tmp.expansion);
		fd_gc_cleanup();
		gc_cleanup();
		_exit(st);
	}
}

void	child_try_run_unset_or_cd(t_command *cmd, char **envp)
{
	int			st;
	t_shell_val	tmp;

	if (!cmd->cmd)
		return ;
	if (!ft_strcmp(cmd->cmd, "unset"))
	{
		tmp.last_exit_status = 0;
		tmp.expansion = NULL;
		env_bootstrap_once(&tmp, envp);
		st = builtin_unset(cmd->args, &tmp.expansion);
		fd_gc_cleanup();
		gc_cleanup();
		_exit(st);
	}
	if (!ft_strcmp(cmd->cmd, "cd"))
	{
		st = builtin_cd(cmd->args);
		fd_gc_cleanup();
		gc_cleanup();
		_exit(st);
	}
}

void	child_finish_with_exit_pwd_or_exec(t_command *cmd, char **envp)
{
	char	*path;

	if (cmd->cmd && !ft_strcmp(cmd->cmd, "exit"))
	{
		fd_gc_cleanup();
		builtin_exit(cmd->args, cmd);
	}
	if (cmd->cmd && !ft_strcmp(cmd->cmd, "pwd"))
	{
		fd_gc_cleanup();
		gc_cleanup();
		builtin_pwd_and_exit();
	}
	path = find_command_path(cmd->cmd, envp);
	child_directory(cmd, path, envp);
}

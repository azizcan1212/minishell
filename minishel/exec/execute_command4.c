/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:44:26 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:35:15 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"

void	child_sigint_handler(int sig)
{
	(void)sig;
	_exit(130);
}

void	exec_child(t_command *cmd, char **envp)
{
	child_setup_signals_and_redirs(cmd);
	child_try_run_echo_env(cmd, envp);
	child_try_run_export(cmd, envp);
	child_try_run_unset_or_cd(cmd, envp);
	child_finish_with_exit_pwd_or_exec(cmd, envp);
}

void	ms_child_io_setup(int role, int i, int total, int *fds)
{
	if (total <= 1)
		return ;
	if (role == 0)
	{
		if (dup2(fds[2 * (total - 2)], STDIN_FILENO) == -1)
		{
			gc_cleanup();
			_exit(1);
		}
		return ;
	}
	if (i > 0 && dup2(fds[2 * (i - 1)], STDIN_FILENO) == -1)
	{
		gc_cleanup();
		_exit(1);
	}
	if (dup2(fds[2 * i + 1], STDOUT_FILENO) == -1)
	{
		gc_cleanup();
		_exit(1);
	}
}

int	fork_prev_children(t_command *first,
	int total, int *fds, char **envp)
{
	int			i;
	t_command	*node;
	pid_t		c;

	i = 0;
	node = first;
	while (i < total - 1)
	{
		c = fork();
		if (c == 0)
		{
			ms_child_io_setup(1, i, total, fds);
			if (total > 1)
				close_pipes(fds, 2 * (total - 1));
			exec_child(node, envp);
		}
		else if (c < 0)
			return (-1);
		node = node->next;
		i++;
	}
	return (0);
}

pid_t	spawn_pipeline(t_command *first,
	int total, int *fds, char **envp)
{
	int			i;
	t_command	*node;
	pid_t		pid;

	i = 0;
	node = first;
	while (i++ < total - 1)
		node = node->next;
	pid = fork();
	if (pid == 0)
	{
		ms_child_io_setup(0, 0, total, fds);
		if (total > 1)
			close_pipes(fds, 2 * (total - 1));
		exec_child(node, envp);
	}
	else if (pid < 0)
		return (-1);
	if (fork_prev_children(first, total, fds, envp) < 0)
		return (-1);
	return (pid);
}

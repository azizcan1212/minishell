/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command8.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 06:12:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 09:36:41 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"
#include "gc.h"
#include <signal.h>
#include <unistd.h>

static int	setup_pipeline(int total, int **fds, t_shell_val *shell)
{
	if (total <= 0)
		return (0);
	if (build_pipes(total, fds) < 0)
	{
		shell->last_exit_status = 1;
		fd_gc_cleanup();
		return (-1);
	}
	return (1);
}

static void	cleanup_pipeline_error(int **fds, int total, t_shell_val *shell)
{
	if (total > 1)
		close_pipes(*fds, 2 * (total - 1));
	shell->last_exit_status = 1;
	fd_gc_cleanup();
}

static void	wait_for_processes(pid_t last, t_shell_val *shell)
{
	int	st;
	int	tmp;
	int	sig;

	sig = 0;
	if (waitpid(last, &st, 0) == -1)
		shell->last_exit_status = 1;
	else if (WIFEXITED(st))
		shell->last_exit_status = WEXITSTATUS(st);
	else if (WIFSIGNALED(st))
	{
		sig = WTERMSIG(st);
		shell->last_exit_status = 128 + sig;
	}
	while (waitpid(-1, &tmp, WNOHANG) > 0)
		;
	if (sig == SIGINT)
		write(1, "\n", 1);
	else if (sig == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
}

int	fork_and_exec(t_command *first_cmd, t_shell_val *shell, char **envp)
{
	int		total;
	int		*fds;
	pid_t	last;
	int		setup_result;

	total = count_cmds(first_cmd);
	setup_result = setup_pipeline(total, &fds, shell);
	if (setup_result <= 0)
		return (setup_result);
	last = spawn_pipeline(first_cmd, total, fds, envp);
	if (total > 1)
		close_pipes(fds, 2 * (total - 1));
	if (last < 0)
	{
		cleanup_pipeline_error(&fds, total, shell);
		return (1);
	}
	wait_for_processes(last, shell);
	fd_gc_cleanup();
	return (shell->last_exit_status);
}

void	handle_path_execution(t_command *cur, char *f, char **envp)
{
	int	err;

	if (is_directory(f))
	{
		fd_gc_cleanup();
		print_and_exit(cur->cmd, ": is a directory", 126);
	}
	execve(f, cur->args, envp);
	err = errno;
	fd_gc_cleanup();
	handle_exec_error(cur, err);
}

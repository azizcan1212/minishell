/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_comman9.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 06:25:17 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 06:29:51 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include "fd_gc.h"

static int	setup_heredoc_pipe(int *p, const char *delim, t_shell_val *val,
			int expandable_fd)
{
	pid_t	pid;

	if (delim == NULL)
		return (-1);
	if (pipe(p) == -1)
		return (-1);
	fd_gc_add(p[0]);
	fd_gc_add(p[1]);
	pid = fork();
	if (pid == 0)
		heredoc_child(p, delim, val, expandable_fd);
	close(p[1]);
	return (pid);
}

static int	handle_heredoc_status(int st, t_shell_val *val, int read_fd)
{
	if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
		return (read_fd);
	if ((WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
		|| (WIFEXITED(st) && WEXITSTATUS(st) == 130))
	{
		val->last_exit_status = 130;
	}
	close(read_fd);
	return (-1);
}

int	get_heredoc_fd(const char *delim, t_shell_val *val, int expandable_fd)
{
	int		p[2];
	pid_t	pid;
	int		st;

	pid = setup_heredoc_pipe(p, delim, val, expandable_fd);
	if (pid < 0)
	{
		close(p[0]);
		return (-1);
	}
	if (waitpid(pid, &st, 0) == -1)
	{
		close(p[0]);
		return (-1);
	}
	return (handle_heredoc_status(st, val, p[0]));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command7.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:12:17 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 18:50:41 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include "fd_gc.h"

static int	run_empty_redirect_only(t_command *h, t_shell_val *val)
{
	int	r;

	if (h == NULL)
		return (-1);
	if (h->next != NULL)
		return (-1);
	if (h->cmd != NULL && h->cmd[0] != '\0')
		return (-1);
	if (h->input_file == NULL && h->output_file == NULL && h->heredoc_fd == -1)
		return (-1);
	r = open_redirs_check(h, val);
	if (r != 0)
		return (1);
	if (h->heredoc_fd != -1)
	{
		close(h->heredoc_fd);
		h->heredoc_fd = -1;
	}
	val->last_exit_status = 0;
	return (0);
}

static int	maybe_run_parent_builtin(t_command *head,
	t_shell_val *val, char **envp)
{
	int	ret;

	if (head == NULL)
		return (-1);
	if (head->next != NULL)
		return (-1);
	ret = handle_builtin_commands(head, val, envp);
	if (ret != -1)
		val->last_exit_status = ret;
	return (ret);
}

void	sigpipe_close_all(int sig)
{
	(void)sig;
	fd_gc_cleanup();
	gc_cleanup();
	_exit(141);
}

void	heredoc_child(int *p, const char *delim,
				t_shell_val *val, int expandable_fd)
{
	close(p[0]);
	fd_gc_add(p[1]);
	signal(SIGPIPE, sigpipe_close_all);
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	read_heredoc_lines(p[1], delim, val, expandable_fd);
	if (val != NULL && val->last_exit_status == 130)
	{
		fd_gc_cleanup();
		gc_cleanup();
		_exit(130);
	}
	fd_gc_cleanup();
	gc_cleanup();
	_exit(0);
}

int	execute_command(t_command *head, char **envp, t_shell_val *val)
{
	int	ret;

	if (prepare_heredocs_all(head, val) != 0)
	{
		close_all_heredocs(head);
		fd_gc_cleanup();
		return (130);
	}
	ret = run_empty_redirect_only(head, val);
	if (ret != -1)
	{
		fd_gc_cleanup();
		return (ret);
	}
	ret = maybe_run_parent_builtin(head, val, envp);
	if (ret != -1)
	{
		close_all_heredocs(head);
		fd_gc_cleanup();
		return (ret);
	}
	ret = fork_and_exec(head, val, envp);
	close_all_heredocs(head);
	fd_gc_cleanup();
	return (ret);
}

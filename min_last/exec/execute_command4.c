/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:44:26 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 04:55:54 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"

void child_sigint_handler(int sig)
{
    (void)sig;
    _exit(130);
}

// Mevcut exec_child fonksiyonunu değiştirin
void	exec_child(t_command *cmd, char **envp)
{
	char	*path;

	g_signal_num = 0;
	signal(SIGINT, child_sigint_handler);
	signal(SIGPIPE, sigpipe_close_all); // SIGPIPE handler'ı child process'te kesin olarak ata
	signal(SIGQUIT, SIG_DFL);
	child_redirections(cmd);
	
	if (cmd->cmd && !ft_strcmp(cmd->cmd, "exit")) {
		fd_gc_cleanup();
		builtin_exit(cmd->args, cmd);
	}
	if (cmd->cmd && !ft_strcmp(cmd->cmd, "pwd")) {
		fd_gc_cleanup();
		gc_cleanup();
		builtin_pwd_and_exit();
	}
	path = find_command_path(cmd->cmd, envp);
	child_directory(cmd, path, envp);
}

static void	ms_child_io_setup(int role, int i, int total, int *fds)
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

static int	fork_prev_children(t_command *first,
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

static pid_t	spawn_pipeline(t_command *first,
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


// Mevcut fork_and_exec fonksiyonunu değiştirin
int	fork_and_exec(t_command *first_cmd, t_shell_val *shell, char **envp)
{
	int		total;
	int		st;
	int		*fds;
	int		tmp;
	pid_t	last;

	total = count_cmds(first_cmd);
	if (total <= 0)
		return (0);
	if (build_pipes(total, &fds) < 0)
	{
		shell->last_exit_status = 1;
		fd_gc_cleanup();
		return (1);
	}
	last = spawn_pipeline(first_cmd, total, fds, envp);
	if (total > 1)
		close_pipes(fds, 2 * (total - 1));
	if (last < 0)
	{
		shell->last_exit_status = 1;
		fd_gc_cleanup();
		return (1);
	}
	if (waitpid(last, &st, 0) == -1)
		shell->last_exit_status = 1;
	else if (WIFEXITED(st))
		shell->last_exit_status = WEXITSTATUS(st);
	else if (WIFSIGNALED(st))
		shell->last_exit_status = 128 + WTERMSIG(st);
	while (waitpid(-1, &tmp, WNOHANG) > 0)
		;
	if(shell -> last_exit_status == 130)
		write(1, "\n", 1);
	fd_gc_cleanup();
	return (shell->last_exit_status);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:15:24 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/07 18:04:05 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "minishell.h"
#include "gc.h"

static void	put2(const char *s)
{
	size_t	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
		i++;
	(void)!write(2, s, i);
}

static void	print_and_exit(const char *name, const char *msg, int code)
{
	(void)!write(2, "minishell: ", 11);
	put2(name);
	put2(msg);
	(void)!write(2, "\n", 1);
	gc_cleanup();
	_exit(code);
}

int	redirect_input(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		return (-1);
	}
	if (dup2(fd, 0) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

int	redirect_output(const char *file, int append)
{
	int	fd;

	if (append)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (dup2(fd, 1) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

static void	handle_exec_error(t_command *cur, int err)
{
	if (err == EACCES)
		print_and_exit(cur->cmd, ": Permission denied", 126);
	if (err == ENOENT
		&& (cur->cmd && (cur->cmd[0] == '.' || ft_strchr(cur->cmd, '/'))))
		print_and_exit(cur->cmd, ": No such file or directory", 127);
	print_and_exit(cur->cmd, ": command not found", 127);
}

static void	check_direct_call(t_command *cur)
{
	if (is_directory(cur->cmd))
		print_and_exit(cur->cmd, ": is a directory", 126);
	if (access(cur->cmd, F_OK) == 0 && access(cur->cmd, X_OK) != 0)
		print_and_exit(cur->cmd, ": Permission denied", 126);
	print_and_exit(cur->cmd, ": No such file or directory", 127);
}

void	child_directory(t_command *cur, char *f, char **envp)
{
	int	err;

	if (cur->cmd && cur->cmd[0] == '.' && cur->cmd[1] == '\0')
		print_and_exit(".",
			": filename argument required\n.: usage: . filename [arguments]",
			2);
	if (f)
	{
		if (is_directory(f))
			print_and_exit(cur->cmd, ": is a directory", 126);
		execve(f, cur->args, envp);
		err = errno;
		free(f);
		handle_exec_error(cur, err);
	}
	if (!cur->cmd || cur->cmd[0] == '\0')
		print_and_exit("", ": command not found", 127);
	if (cur->cmd[0] == '.' || ft_strchr(cur->cmd, '/'))
		check_direct_call(cur);
	print_and_exit(cur->cmd, ": command not found", 127);
}

static int	count_cmds(t_command *n)
{
	int	c;

	c = 0;
	while (n)
	{
		c++;
		n = n->next;
	}
	return (c);
}

static int	build_pipes(int total, int **fds)
{
	int	i;

	*fds = NULL;
	if (total <= 1)
		return (0);
	*fds = (int *)malloc(sizeof(int) * 2 * (total - 1));
	if (!*fds)
		return (-1);
	i = 0;
	while (i < total - 1)
	{
		if (pipe(&(*fds)[2 * i]) == -1)
		{
			close_pipes(*fds, 2 * i);
			free(*fds);
			*fds = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

static void	child_redirections(t_command *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, 0) == -1)
		{
			gc_cleanup();
			_exit(1);
		}
		close(cmd->heredoc_fd);
	}
	if (cmd->input_file && redirect_input(cmd->input_file) < 0)
	{
		gc_cleanup();
		_exit(1);
	}
	if (cmd->output_file && redirect_output(cmd->output_file, cmd->append) < 0)
	{
		gc_cleanup();
		_exit(1);
	}
}

void	exec_child(t_command *cmd, char **envp)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	child_redirections(cmd);
	if (cmd->cmd && !ft_strcmp(cmd->cmd, "exit"))
		builtin_exit(cmd->args);
	path = find_command_path(cmd->cmd, envp);
	child_directory(cmd, path, envp);
}

static pid_t	spawn_pipeline(t_command *first, int total, int *fds, char **envp)
{
	int			i;
	t_command	*node;
	t_command	*last_node;
	pid_t		pid;

	i = 0;
	node = first;
	while (i++ < total - 1) node = node->next;
	last_node = node;
	pid = fork();
	if (pid == 0)
	{
		if (total > 1)
		{
			if (dup2(fds[2 * (total - 2)], STDIN_FILENO) == -1)
			{
				gc_cleanup();
				_exit(1);
			}
			close_pipes(fds, 2 * (total - 1));
		}
		exec_child(last_node, envp);
	}
	else if (pid < 0)
		return (-1);
	i = 0;
	node = first;
	while (i < total - 1)
	{
		pid_t	c = fork();
		if (c == 0)
		{
			if (total > 1)
			{
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
				close_pipes(fds, 2 * (total - 1));
			}
			exec_child(node, envp);
		}
		else if (c < 0)
			return (-1);
		node = node->next;
		i++;
	}
	return (pid);
}

int	fork_and_exec(t_command *first_cmd, t_shell_val *shell, char **envp)
{
	int		total;
	int		st;
	int		*fds;
	pid_t	last;

	total = count_cmds(first_cmd);
	if (total <= 0)
		return (0);
	if (build_pipes(total, &fds) < 0)
		return (shell->last_exit_status = 1, 1);
	last = spawn_pipeline(first_cmd, total, fds, envp);
	if (total > 1)
	{
		close_pipes(fds, 2 * (total - 1));
		free(fds);
	}
	if (last < 0)
		return (shell->last_exit_status = 1, 1);
	if (waitpid(last, &st, 0) == -1)
		shell->last_exit_status = 1;
	else if (WIFEXITED(st))
		shell->last_exit_status = WEXITSTATUS(st);
	else if (WIFSIGNALED(st))
		shell->last_exit_status = 128 + WTERMSIG(st);
	while (wait(NULL) > 0)
		;
	return (shell->last_exit_status);
}

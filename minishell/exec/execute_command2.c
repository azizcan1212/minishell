/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:15:24 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/28 15:20:25 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "minishell.h"

int	redirect_input(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		return (-1);
	}
	dup2(fd, 0);
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
	dup2(fd, 1);
	close(fd);
	return (0);
}

void	child_directory(t_command *cur, char *f, char **envp)
{
	if (f)
	{
		if (is_directory(f))
		{
			printf("minishell: %s: Is a directory\n", cur->cmd);
			free(f);
			exit(126);
		}
		if (access(f, X_OK) != 0)
		{
			printf("minishell: %s: Permission denied\n", cur->cmd);
			free(f);
			exit(126);
		}
		execve(f, cur->args, envp);
		free(f);
		exit(127);
	}
}



static void	exec_child(t_command *cur, char **envp)
{
	char	*f;

	if (cur->heredoc_fd != -1)
	{
		dup2(cur->heredoc_fd, 0);
		close(cur->heredoc_fd);
	}
	if (cur->input_file && redirect_input(cur->input_file) < 0)
		exit(1);
	if (cur->output_file && redirect_output(cur->output_file, cur->append) < 0)
		exit(1);
	f = find_command_path(cur->cmd, envp);
	child_directory(cur, f, envp);
	if (!cur->cmd || cur->cmd[0] == '\0')
	{
		printf("minishell: '': command not found\n");
	}
	else
		printf("minishell: %s: command not found\n", cur->cmd);
	exit(127);
}

void	fork_and_exec(t_command *cmd,
	char **envp, int *pipes, pid_t *pids, int n)
{
	int				i;
	t_command		*cur;

	i = 0;
	cur = cmd;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			setup_child_io(i, n, pipes);
			close_pipes(pipes, 2 * (n - 1));
			exec_child(cur, envp);
		}
		cur = cur->next;
		i++;
	}
}

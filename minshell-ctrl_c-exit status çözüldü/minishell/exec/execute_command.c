/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:47 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>

static void	read_heredoc_lines(int write_fd,
	const char *delim, t_shell_val *val, int expandable_fd)
{
	char	*line;
	int		line_number;

	line_number = 1;
	while (1)
	{
		line = readline("> ");
		if (should_stop_heredoc(line, delim, line_number))
		{
			if (line)
				free(line); // readline allocates with system malloc
			break ;
		}
		process_heredoc_line(line, write_fd, val, expandable_fd);
		free(line); // readline allocates with system malloc
		line_number++;
	}
}

static int	get_heredoc_fd(const char *delim, t_shell_val *val, int expandable_fd)
{
	int		fd[2];

	if (pipe(fd) == -1)
		return (-1);
	read_heredoc_lines(fd[1], delim, val, expandable_fd);
	close(fd[1]);
	return (fd[0]);
}

int	is_directory(const char *path)
{
	DIR		*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

void check_directory(t_command *cur, t_shell_val *val)
{
	if (cur->cmd[0] == '.' && cur->cmd[1] == '\0')
	{
		printf("minishell: .: filename argument required\n");
		val->last_exit_status = 2;
		gc_cleanup();
	}
	if(cur->cmd[0] == '.' && cur->cmd[1] == '.' && cur->cmd[2] == '\0')
	{
		printf ("minishell: %s: Command not found\n", cur->cmd);
		val->last_exit_status = 127;
		gc_cleanup();
	}
	if (access(cur->cmd, F_OK) != 0)
	{
		printf("minishell: %s: No such file or directory\n", cur->cmd);
		val->last_exit_status = 127;
		gc_cleanup();
	}
	return;
}

int	execute_command(t_command *cmd, char **envp, t_shell_val *val)
{
	int			n;
	int			i;
	int			pipes[512];
	pid_t		pids[256];
	int			status;
	t_command	*cur;
	int			builtin_result;

	if(cmd -> cmd)
	{
		if((ft_strchr(cmd->cmd, '/') || cmd->cmd[0] == '.') )
			check_directory(cmd, val);
	}
	builtin_result = handle_builtin_commands(cmd, val, envp);
	if (builtin_result != -1)
		return (builtin_result);
	n = 0;
	cur = cmd;
	while (cur)
	{
		if (cur->heredoc_delim)
			cur->heredoc_fd = get_heredoc_fd(cur->heredoc_delim, val,cur->expandable_fd);
		else
			cur->heredoc_fd = -1;
		n++;
		cur = cur->next;
	}
	i = 0;
	while (i < n - 1)
	{
		pipe(pipes + i * 2);
		i++;
	}
	fork_and_exec(cmd, envp, pipes, pids, n);
	close_pipes(pipes, 2 * (n - 1));
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	cur = cmd;
	while (cur)
	{
		if (cur->heredoc_fd != -1)
			close(cur->heredoc_fd);
		cur = cur->next;
	}
	if (WIFEXITED(status))
		val->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		val->last_exit_status = 128 + WTERMSIG(status);
	return (0);
}
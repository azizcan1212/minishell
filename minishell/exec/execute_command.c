/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:47 by muharsla          #+#    #+#             */
/*   Updated: 2025/07/23 16:43:21 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int	execute_command(t_command *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	char	*full;

	pid = fork();
	if (pid == 0)
	{
		if (cmd->input_file && redirect_input(cmd->input_file) < 0)
			return (perror(cmd->input_file), _exit(1), 1);
		if (cmd->output_file && redirect_output(cmd->output_file, cmd->append) < 0)
			return (perror(cmd->output_file), _exit(1), 1);
		if (cmd->heredoc_delim && setup_heredoc(cmd->heredoc_delim) < 0)
			return (_exit(1), 1);
		full = find_command_path(cmd->cmd, envp);
		if (full)
		{
			execve(full, cmd->args, envp);
			free(full);
		}
		write(2, "minishell: command not found\n", 29);
		_exit(127);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	redirect_input(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-1);
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

int	setup_heredoc(const char *delim)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delim))
			break;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(fd[1]);
	dup2(fd[0], 0);
	close(fd[0]);
	return (0);
}

void print_command_debug(t_command *cmd)
{
	int i = 0;
	printf("cmd->cmd: '%s'\n", cmd->cmd);
	while (cmd->args && cmd->args[i])
	{
		printf("cmd->args[%d]: '%s'\n", i, cmd->args[i]);
		i++;
	}
	if (cmd->output_file)
		printf("cmd->output_file: '%s' (append: %d)\n", cmd->output_file, cmd->append);
	if (cmd->input_file)
		printf("cmd->input_file: '%s'\n", cmd->input_file);
	if (cmd->heredoc_delim)
		printf("cmd->heredoc_delim: '%s'\n", cmd->heredoc_delim);
}

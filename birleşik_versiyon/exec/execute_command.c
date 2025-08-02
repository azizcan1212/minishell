/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:47 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/02 13:07:23 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int execute_command(t_command *cmd, char **envp, t_shell_val *val)
{
	int n = 0, i, s = 0; t_command *t = cmd;
	while (t) { n++; t = t->next; }
	int p[2 * (n - 1)]; pid_t ids[n];
	for (i = 0; i < n - 1; i++) pipe(p + i * 2);
	t = cmd;
	for (i = 0; i < n; i++) {
		ids[i] = fork();
		if (!ids[i]) {
			if (i > 0) dup2(p[(i - 1) * 2], 0);
			if (i < n - 1) dup2(p[i * 2 + 1], 1);
			for (int j = 0; j < 2 * (n - 1); j++) close(p[j]);
			if (t->input_file && redirect_input(t->input_file) < 0) exit(1);
			if (t->output_file && redirect_output(t->output_file, t->append) < 0) exit(1);
			if (t->heredoc_delim && setup_heredoc(t->heredoc_delim) < 0) exit(1);
			char *f = find_command_path(t->cmd, envp);
			if (f) { execve(f, t->args, envp); free(f); }
			write(2, "minishell: command not found\n", 29); exit(127);
		}
		t = t->next;
	}
	for (i = 0; i < 2 * (n - 1); i++) close(p[i]);
	for (i = 0; i < n; i++) waitpid(ids[i], &s, 0);
	val->last_exit_status = WIFEXITED(s) ? WEXITSTATUS(s) : 128 + WTERMSIG(s);
	return 0;
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

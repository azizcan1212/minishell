/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:47 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/04 14:39:25 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "libft.h"
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

// Built-in implementations
int	builtin_echo(char **args)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	// Check for -n option
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	
	if (newline)
		write(1, "\n", 1);
	return (0);
}

int	builtin_cd(char **args)
{
	char *path;

	if (!args[1])
	{
		// cd without arguments goes to HOME
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else
		path = args[1];
	
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}

int	builtin_exit(char **args)
{
	(void)args;
	write(1, "exit\n", 5);
	exit(0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || 
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"))
		return (1);
	return (0);
}

int	execute_builtin(char *cmd, char **args)
{
	if (!ft_strcmp(cmd, "echo"))
		return (builtin_echo(args));
	else if (!ft_strcmp(cmd, "cd"))
		return (builtin_cd(args));
	else if (!ft_strcmp(cmd, "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd, "exit"))
		return (builtin_exit(args));
	else if (!ft_strcmp(cmd, "unset"))
		return (builtin_unset(args, NULL));
	return (1);
}

static void	close_pipes(int *pipes, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		close(pipes[i]);
		i++;
	}
}

static void	setup_child_io(int i, int n, int *pipes)
{
	if (i > 0)
		dup2(pipes[(i - 1) * 2], 0);
	if (i < n - 1)
		dup2(pipes[i * 2 + 1], 1);
}

static int	get_heredoc_fd(const char *delim, t_shell_val *shell_val)
{
	int		fd[2];
	char	*line;
	char	*expanded;
	int		delimiter_quoted;

	if (pipe(fd) == -1)
		return (-1);
	delimiter_quoted = is_quoted(delim);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
			break;
		if (!delimiter_quoted)
			expanded = heredoc_expand(line, shell_val);
		else
			expanded = ft_strdup(line);
		write(fd[1], expanded, ft_strlen(expanded));
		write(fd[1], "\n", 1);
		free(expanded);
		free(line);
	}
	free(line);
	close(fd[1]);
	return (fd[0]);
}

static void	exec_child(t_command *cur, char **envp)
{
	int exit_code;

	if (cur->heredoc_fd != -1)
	{
		dup2(cur->heredoc_fd, 0);
		close(cur->heredoc_fd);
	}
	if (cur->input_file && redirect_input(cur->input_file) < 0)
		exit(1);
	if (cur->output_file && redirect_output(cur->output_file, cur->append) < 0)
		exit(1);
	
	// Check for built-in commands
	if (is_builtin(cur->cmd))
	{
		exit_code = execute_builtin(cur->cmd, cur->args);
		exit(exit_code);
	}
	
	// External commands
	char *f = find_command_path(cur->cmd, envp);
	if (f)
	{
		execve(f, cur->args, envp);
		free(f);
	}
	write(2, "minishell: command not found\n", 29);
	exit(127);
}

static void	fork_and_exec(t_command *cmd, char **envp, int *pipes, pid_t *pids, int n)
{
	int i;
	t_command *cur;

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

int	execute_command(t_command *cmd, char **envp, t_shell_val *val)
{
	int n;
	int i;
	int pipes[512];
	pid_t pids[256];
	int status;
	t_command *cur;

	if (cmd && cmd->cmd && !cmd->next)
	{
		if (!ft_strcmp(cmd->cmd, "cd"))
		{
			val->last_exit_status = builtin_cd(cmd->args);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "exit"))
		{
			builtin_exit(cmd->args);
			return (0);
		}
	}

	n = 0;
	cur = cmd;
	while (cur)
	{
		if (cur->heredoc_delim)
			cur->heredoc_fd = get_heredoc_fd(cur->heredoc_delim, val);
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

int	is_quoted(const char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if ((str[0] == '\'' && str[len - 1] == '\'') ||
		(str[0] == '"' && str[len - 1] == '"'))
		return (1);
	return (0);
}

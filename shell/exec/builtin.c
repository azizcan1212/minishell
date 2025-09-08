/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:05:27 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/07 17:11:21 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"
#include "gc.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;
	int	j;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		newline = 0;
		i++;
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
	char	*path;

	if (args[2] || has_whitespace(args[1]))
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	if (!args[1])
	{
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
	char	*cwd;

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

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int		exit_code;
	long	result;

	if (!args[1])
	{
		gc_cleanup();
		exit(0);
	}
	if (args[2])
	{
		write(2, "bash: exit: too many arguments\n", 32);
		gc_cleanup();
		exit(1);
	}
	if (!is_numeric(args[1]))
	{
		write(2, "bash: exit: ", 12);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		gc_cleanup();
		exit(2);
	}
	result = ft_atoi(args[1]);
	exit_code = (unsigned char)(result % 256);
	if (result < 0)
		exit_code = 256 + (result % 256);
	gc_cleanup();
	exit(exit_code);
}

int	handle_builtin_commands(t_command *cmd, t_shell_val *val, char **envp)
{
	int	sv_in;
	int	sv_out;
	int	st;

	if (!(cmd && cmd->cmd && !cmd->next) || (!is_parent_builtin(cmd -> cmd)))
		return (-1);
	if (save_std(&sv_in, &sv_out))
		return (1);
	if (!ft_strcmp(cmd->cmd, "exit"))
	{
		if (!cmd->next && isatty(STDOUT_FILENO))
			(void)!write(STDOUT_FILENO, "exit\n", 5);
		close(sv_in);
		close(sv_out);
		builtin_exit(cmd->args);
		return (0);
	}
	if (apply_in_parent(cmd) || apply_out_parent(cmd))
	{
		restore_std(sv_in, sv_out);
		val->last_exit_status = 1;
		return (1);
	}
	st = exec_builtin_single(cmd, val, envp);
	restore_std(sv_in, sv_out);
	return (st);
}

int	is_parent_builtin(const char *s)
{
	if (!s)
		return (0);
	if (!ft_strcmp(s, "cd"))
		return (1);
	if (!ft_strcmp(s, "echo"))
		return (1);
	if (!ft_strcmp(s, "pwd"))
		return (1);
	if (!ft_strcmp(s, "env"))
		return (1);
	if (!ft_strcmp(s, "export"))
		return (1);
	if (!ft_strcmp(s, "unset"))
		return (1);
	if (!ft_strcmp(s, "exit"))
		return (1);
	return (0);
}

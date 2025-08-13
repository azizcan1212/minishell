/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:05:27 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 16:05:29 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static int	builtin_echo(char **args)
{
	int		i;
	int		newline;

	i = 1;
	newline = 1;
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

static int	builtin_cd(char **args)
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

static int	builtin_pwd(void)
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

static int	is_numeric(const char *str)
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

static int	builtin_exit(char **args)
{
	int		exit_code;
	long	result;

	write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	if (args[2])
	{
		write(2, "bash: exit: too many arguments\n", 32);
		exit(1);
	}
	if (!is_numeric(args[1]))
	{
		write(2, "bash: exit: ", 12);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		exit(2);
	}
	result = ft_atoi(args[1]);
	exit_code = (unsigned char)(result % 256);
	if (result < 0)
		exit_code = 256 + (result % 256);
	exit(exit_code);
}

int	handle_builtin_commands(t_command *cmd, t_shell_val *val, char **envp)
{
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
		if (!ft_strcmp(cmd->cmd, "echo"))
		{
			val->last_exit_status = builtin_echo(cmd->args);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "pwd"))
		{
			val->last_exit_status = builtin_pwd();
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "env"))
		{
			val->last_exit_status = builtin_env(envp);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "export"))
		{
			val->last_exit_status = builtin_export(cmd->args, val->expansion);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "unset"))
		{
			val->last_exit_status = builtin_unset(cmd->args, &(val->expansion));
			return (val->last_exit_status);
		}
	}
	return (-1);
}

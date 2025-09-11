/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:13:03 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 19:20:57 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static int	print_env_from_exp(t_expansion *exp)
{
	while (exp)
	{
		if (exp->key && exp->value && exp->export == 1)
		{
			write(1, exp->key, ft_strlen(exp->key));
			write(1, "=", 1);
			write(1, exp->value, ft_strlen(exp->value));
			write(1, "\n", 1);
		}
		exp = exp->next;
	}
	return (0);
}

int	apply_out_parent(t_command *cmd)
{
	int	fd;
	int	flags;

	if (cmd->output_file == NULL)
		return (0);
	flags = O_CREAT | O_WRONLY;
	if (cmd->append != 0)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd < 0)
	{
		perror(cmd->output_file);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	exec_builtin_single(t_command *cmd, t_shell_val *val, char **envp)
{
	int	st;

	env_bootstrap_once(val, envp);
	if (!ft_strcmp(cmd->cmd, "cd"))
		st = builtin_cd(cmd->args);
	else if (!ft_strcmp(cmd->cmd, "echo"))
		st = builtin_echo(cmd->args);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		st = builtin_pwd();
	else if (!ft_strcmp(cmd->cmd, "env"))
		st = bi_env_single(cmd, val, envp);
	else if (!ft_strcmp(cmd->cmd, "export"))
		st = builtin_export(cmd->args, &(val->expansion), envp);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		st = builtin_unset(cmd->args, &(val->expansion));
	else
		return (-1);
	val->last_exit_status = st;
	return (st);
}

int	bi_env_single(t_command *cmd, t_shell_val *val, char **envp)
{
	if (cmd->args[1])
	{
		ft_putendl_fd("minishell: env: too many arguments", 2);
		return (1);
	}
	env_bootstrap_once(val, envp);
	return (print_env_from_exp(val->expansion));
}

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum((unsigned char)s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

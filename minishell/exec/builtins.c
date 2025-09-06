/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:13:03 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/06 16:47:57 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:13:03 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static void	exp_append(t_expansion **head, t_expansion *node);
static void	env_bootstrap_once(t_shell_val *val, char **envp);

static int	print_env_from_exp(t_expansion *exp)
{
	while (exp)
	{
		if (exp->key && exp->value)
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

int	save_std(int *sv_in, int *sv_out)
{
	int	a;
	int	b;

	a = dup(STDIN_FILENO);
	b = dup(STDOUT_FILENO);
	if (a < 0 || b < 0)
	{
		if (a >= 0)
			close(a);
		if (b >= 0)
			close(b);
		return (1);
	}
	*sv_in = a;
	*sv_out = b;
	return (0);
}

void	restore_std(int sv_in, int sv_out)
{
	if (sv_in >= 0)
	{
		dup2(sv_in, STDIN_FILENO);
		close(sv_in);
	}
	if (sv_out >= 0)
	{
		dup2(sv_out, STDOUT_FILENO);
		close(sv_out);
	}
}

int	apply_in_parent(t_command *cmd)
{
	int	fd;

	if (cmd->heredoc_fd >= 0)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
		{
			close(cmd->heredoc_fd);
			return (1);
		}
		close(cmd->heredoc_fd);
		return (0);
	}
	if (!cmd->input_file)
		return (0);
	fd = open(cmd->input_file, O_RDONLY);
	if (fd < 0)
	{
		perror(cmd->input_file);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_out_parent(t_command *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->output_file)
		return (0);
	flags = O_CREAT | O_WRONLY | (cmd->append ? O_APPEND : O_TRUNC);
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

static void	exp_append(t_expansion **head, t_expansion *node)
{
	t_expansion	*cur;

	if (!node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static void	env_bootstrap_once(t_shell_val *val, char **envp)
{
	int			i;
	const char	*s;
	t_expansion	*e;
	char		*eq;

	if (val->expansion || !envp)
		return ;
	i = 0;
	while ((s = envp[i++]))
	{
		e = (t_expansion *)gc_malloc(sizeof(*e));
		if (!e)
			return ;
		e->key = NULL;
		e->value = NULL;
		e->export = 1;
		e->next = NULL;
		eq = ft_strchr(s, '=');
		if (eq)
		{
			e->key = gc_substr(s, 0, eq - s);
			e->value = gc_strdup(eq + 1);
		}
		else
			e->key = gc_strdup(s);
		exp_append(&val->expansion, e);
	}
}

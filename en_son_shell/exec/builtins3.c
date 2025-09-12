/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:27:17 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:45:01 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin_commands(t_command *cmd, t_shell_val *val, char **envp)
{
	int		sv_in;
	int		sv_out;
	int		st;

	if (!cmd || !cmd->cmd || cmd->next || !is_parent_builtin(cmd->cmd))
		return (-1);
	if (save_std(&sv_in, &sv_out))
		return (1);
	if (!ft_strcmp(cmd->cmd, "exit"))
	{
		if (isatty(STDOUT_FILENO))
			(void)!write(STDOUT_FILENO, "exit\n", 5);
		close(sv_in);
		close(sv_out);
		builtin_exit_parent(cmd->args, cmd, val);
		return (0);
	}
	if (apply_in_parent(cmd))
		return (restore_std(sv_in, sv_out), val->last_exit_status = 1, 1);
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

void	exp_append(t_expansion **head, t_expansion *node)
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

t_expansion	*make_exp_from_str(const char *s)
{
	char		*eq;
	int			klen;
	t_expansion	*e;

	eq = ft_strchr(s, '=');
	if (eq)
		klen = (int)(eq - s);
	else
		klen = (int)ft_strlen(s);
	e = (t_expansion *)gc_malloc(sizeof(*e));
	if (!e)
		return (NULL);
	if (eq)
	{
		e->key = gc_substr(s, 0, klen);
		e->value = gc_strdup(eq + 1);
	}
	else
		e->key = gc_strdup(s);
	e->export = 1;
	e->next = NULL;
	return (e);
}

void	env_bootstrap_once(t_shell_val *val, char **envp)
{
	int				i;
	const char		*s;
	t_expansion		*e;
	extern char		**environ;

	if (val->expansion)
		return ;
	if (!envp || !envp[0])
		envp = environ;
	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		s = envp[i];
		e = make_exp_from_str(s);
		if (!e)
			return ;
		e->export = 1;
		add_expansion_back(&val->expansion, e);
		i++;
	}
}

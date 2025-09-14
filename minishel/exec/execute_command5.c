/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:45:26 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/13 05:52:18 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"
#include "gc.h"
#include "libft.h"

void	child_directory(t_command *cur, char *f, char **envp)
{
	if (cur->cmd && cur->cmd[0] == '.' && cur->cmd[1] == '\0')
	{
		fd_gc_cleanup();
		print_and_exit(".",
			": filename argument required\n.: usage: . filename [arguments]",
			2);
	}
	if (f)
	{
		handle_path_execution(cur, f, envp);
		return ;
	}
	if (!cur->cmd || cur->cmd[0] == '\0')
	{
		fd_gc_cleanup();
		print_and_exit("''", ": command not found", 127);
	}
	if (ft_strchr(cur->cmd, '/'))
	{
		fd_gc_cleanup();
		check_direct_call(cur);
	}
	fd_gc_cleanup();
	print_and_exit(cur->cmd, ": command not found", 127);
}

int	build_pipes(int total, int **fds)
{
	int	i;

	*fds = NULL;
	if (total <= 1)
		return (0);
	*fds = (int *)gc_malloc(sizeof(int) * 2 * (total - 1));
	if (!*fds)
		return (-1);
	i = 0;
	while (i < total - 1)
	{
		if (pipe(&(*fds)[2 * i]) == -1)
		{
			fd_gc_cleanup();
			*fds = NULL;
			return (-1);
		}
		fd_gc_add((*fds)[2 * i]);
		fd_gc_add((*fds)[2 * i + 1]);
		i++;
	}
	return (0);
}

int	count_cmds(t_command *n)
{
	int	c;

	c = 0;
	while (n)
	{
		c++;
		n = n->next;
	}
	return (c);
}

void	builtin_pwd_and_exit(void)
{
	int	st;

	st = builtin_pwd();
	fd_gc_cleanup();
	gc_cleanup();
	exit(st);
}

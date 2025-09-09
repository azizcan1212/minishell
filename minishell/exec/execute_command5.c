/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:45:26 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 18:49:41 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_directory(t_command *cur, char *f, char **envp)
{
	int	err;

	if (cur->cmd && cur->cmd[0] == '.' && cur->cmd[1] == '\0')
		print_and_exit(".",
			": filename argument required\n.: usage: . filename [arguments]",
			2);
	if (f)
	{
		if (is_directory(f))
			print_and_exit(cur->cmd, ": is a directory", 126);
		execve(f, cur->args, envp);
		err = errno;
		free(f);
		handle_exec_error(cur, err);
	}
	if (!cur->cmd || cur->cmd[0] == '\0')
		print_and_exit("", ": command not found", 127);
	if (cur->cmd[0] == '.' || ft_strchr(cur->cmd, '/'))
		check_direct_call(cur);
	print_and_exit(cur->cmd, ": command not found", 127);
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
			close_pipes(*fds, 2 * i);
			*fds = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

void	child_redirections(t_command *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, 0) == -1)
		{
			gc_cleanup();
			_exit(1);
		}
		close(cmd->heredoc_fd);
	}
	if (cmd->input_file && redirect_input(cmd->input_file) < 0)
	{
		gc_cleanup();
		_exit(1);
	}
	if (cmd->output_file && redirect_output(cmd->output_file, cmd->append) < 0)
	{
		gc_cleanup();
		_exit(1);
	}
}

void	builtin_pwd_and_exit(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		gc_cleanup();
		exit(0);
	}
	gc_cleanup();
	exit(1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command11.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:22:55 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:42:49 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"
#include "gc.h"

static void	child_fail_exit(void)
{
	fd_gc_cleanup();
	gc_cleanup();
	_exit(1);
}

static void	apply_heredoc_before_sequence(t_command *cmd)
{
	if (cmd->heredoc_fd != -1 && cmd->redir_count > 0)
	{
		if (dup2(cmd->heredoc_fd, 0) == -1)
			child_fail_exit();
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

static void	apply_redir_sequence(t_command *cmd)
{
	int				i;
	int				type;
	const char		*file;
	int				rc;

	if (cmd->redir_count <= 0)
		return ;
	i = 0;
	while (i < cmd->redir_count)
	{
		type = cmd->redir_types[i];
		file = cmd->redir_files[i];
		if (type == 0)
			rc = redirect_input(file);
		else
			rc = redirect_output(file, type == 2);
		if (rc < 0)
			child_fail_exit();
		i++;
	}
}

static void	apply_single_heredoc_and_files(t_command *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, 0) == -1)
			child_fail_exit();
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->input_file && redirect_input(cmd->input_file) < 0)
		child_fail_exit();
	else if (cmd->output_file
		&& redirect_output(cmd->output_file, cmd->append) < 0)
		child_fail_exit();
}

void	child_redirections(t_command *cmd)
{
	apply_heredoc_before_sequence(cmd);
	if (cmd->redir_count > 0)
	{
		apply_redir_sequence(cmd);
		return ;
	}
	apply_single_heredoc_and_files(cmd);
}

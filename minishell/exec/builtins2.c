/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:20:12 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:55:29 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_parent_heredoc(t_command *cmd)
{
	if (dup2(cmd->heredoc_fd, STDIN_FILENO) < 0)
	{
		close(cmd->heredoc_fd);
		return (1);
	}
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
	return (0);
}

static int	apply_parent_input_file(t_command *cmd)
{
	int	fd;

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

static int	parent_dup_in(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file), 1);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (close(fd), 1);
	close(fd);
	return (0);
}

static int	parent_dup_out(const char *file, int type)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (type == 2)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
		return (perror(file), 1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (close(fd), 1);
	close(fd);
	return (0);
}

int	apply_in_parent(t_command *cmd)
{
	int	i;

	i = -1;
	if (cmd->redir_count > 0)
	{
		while (++i < cmd->redir_count)
		{
			if (cmd->redir_types[i] == 0)
			{
				if (parent_dup_in(cmd->redir_files[i]) != 0)
					return (1);
			}
			else
			{
				if (parent_dup_out(cmd->redir_files[i],
						cmd->redir_types[i]) != 0)
					return (1);
			}
		}
		return (0);
	}
	if (cmd->heredoc_fd >= 0)
		return (apply_parent_heredoc(cmd));
	return (apply_parent_input_file(cmd));
}

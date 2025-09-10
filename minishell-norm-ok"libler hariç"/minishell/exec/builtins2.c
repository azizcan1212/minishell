/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:20:12 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 17:26:00 by muharsla         ###   ########.fr       */
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

int	apply_in_parent(t_command *cmd)
{
	if (cmd->heredoc_fd >= 0)
		return (apply_parent_heredoc(cmd));
	return (apply_parent_input_file(cmd));
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

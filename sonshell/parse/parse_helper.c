/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:51 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 06:31:28 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gc.h"
#include <stdlib.h>
#include "libft.h"
#include "fd_gc.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = gc_malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	init_command_fields(cmd);
	return (cmd);
}

void	init_command_fields(t_command *cmd)
{
	cmd->cmd = NULL;
	cmd->heredoc_list = NULL;
	cmd->args = gc_malloc(sizeof(char *) * 100);
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->redir_files = NULL;
	cmd->redir_types = NULL;
	cmd->redir_count = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_fd = -1;
	cmd->append = 0;
	cmd->next = NULL;
	cmd->expandable_fd = 0;
}

int	token_is_pipe(t_token *tk)
{
	if (!tk || !tk->value)
		return (0);
	return (!ft_strcmp(tk->value, "|"));
}

int	should_touch_file(t_command *cmd)
{
	int	fd;

	if (cmd->cmd && cmd->cmd[0] != '\0')
		return (0);
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		fd_gc_add(fd);
		if (fd < 0)
			return (0);
		close(fd);
	}
	return (1);
}

void	touch_output_file(char *filename, int append)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 420);
	fd_gc_add(fd);
	if (fd >= 0)
		close(fd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:46:32 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 19:15:29 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_direct_call(t_command *cur)
{
	if (is_directory(cur->cmd))
		print_and_exit(cur->cmd, ": is a directory", 126);
	if (access(cur->cmd, F_OK) == 0 && access(cur->cmd, X_OK) != 0)
		print_and_exit(cur->cmd, ": Permission denied", 126);
	print_and_exit(cur->cmd, ": No such file or directory", 127);
}

int	handle_node_heredoc(t_command *node, t_shell_val *val)
{
	t_hdoc	*h;
	int		fd_tmp;

	node->heredoc_fd = -1;
	if (node->heredoc_list != NULL)
	{
		h = node->heredoc_list;
		while (h != NULL)
		{
			fd_tmp = get_heredoc_fd(h->delim, val, h->expandable);
			if (fd_tmp == -1 && val->last_exit_status == 130)
				return (1);
			if (h->next == NULL)
				node->heredoc_fd = fd_tmp;
			else if (fd_tmp != -1)
				close(fd_tmp);
			h = h->next;
		}
	}
	else if (node->heredoc_delim != NULL)
		node->heredoc_fd = get_heredoc_fd(node->heredoc_delim, val,
				node->expandable_fd);
	return (0);
}

int	open_input_check(t_command *h, t_shell_val *val)
{
	int	fd;

	if (h->input_file == NULL)
		return (0);
	fd = open(h->input_file, O_RDONLY);
	if (fd < 0)
	{
		perror(h->input_file);
		val->last_exit_status = 1;
		return (1);
	}
	close(fd);
	return (0);
}

int	open_output_check(t_command *h, t_shell_val *val)
{
	int	fd;
	int	flags;

	if (h->output_file == NULL)
		return (0);
	flags = O_CREAT | O_WRONLY;
	if (h->append != 0)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	fd = open(h->output_file, flags, 0644);
	if (fd < 0)
	{
		perror(h->output_file);
		val->last_exit_status = 1;
		return (1);
	}
	close(fd);
	return (0);
}

void	close_all_heredocs(t_command *head)
{
	t_command	*n;

	n = head;
	while (n != NULL)
	{
		if (n->heredoc_fd != -1)
		{
			close(n->heredoc_fd);
			n->heredoc_fd = -1;
		}
		n = n->next;
	}
}

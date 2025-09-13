/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:46:32 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:14:09 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"

void	check_direct_call(t_command *cur)
{
	if (is_directory(cur->cmd))
	{
		fd_gc_cleanup();
		print_and_exit(cur->cmd, ": is a directory", 126);
	}
	if (access(cur->cmd, F_OK) == 0 && access(cur->cmd, X_OK) != 0)
	{
		fd_gc_cleanup();
		print_and_exit(cur->cmd, ": Permission denied", 126);
	}
	fd_gc_cleanup();
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

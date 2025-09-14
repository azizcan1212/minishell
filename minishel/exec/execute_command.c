/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:47 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/13 05:43:36 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fd_gc.h>

void	heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	fd_gc_cleanup();
	gc_cleanup();
	_exit(130);
}

void	read_heredoc_lines(int write_fd,
	const char *delim, t_shell_val *val, int expandable_fd)
{
	char	*line;

	(void)val;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			(void)!write(2,
				"warning: here-document delimited by end-of-file (wanted '",
				58);
			(void)!write(2, delim, ft_strlen(delim));
			(void)!write(2, "')\n", 3);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, write_fd, val, expandable_fd);
		free(line);
	}
}

int	is_directory(const char *path)
{
	struct stat	st;

	if (!path || !*path)
		return (0);
	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode) != 0);
}

int	prepare_heredocs_all(t_command *head, t_shell_val *val)
{
	t_command	*node;
	int			interrupted;

	node = head;
	interrupted = 0;
	while (node != NULL)
	{
		if (handle_node_heredoc(node, val) != 0)
			interrupted = 1;
		node = node->next;
	}
	return (interrupted);
}

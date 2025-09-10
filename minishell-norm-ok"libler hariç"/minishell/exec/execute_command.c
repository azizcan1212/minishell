/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:47 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 19:27:49 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
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

void	check_directory(t_command *cur, t_shell_val *val)
{
	if (cur->cmd[0] == '.' && cur->cmd[1] == '\0')
	{
		printf("minishell: .: filename argument required\n");
		val->last_exit_status = 2;
		return ;
	}
	if (cur->cmd[0] == '.' && cur->cmd[1] == '.' && cur->cmd[2] == '\0')
	{
		printf ("minishell: %s: Command not found\n", cur->cmd);
		val->last_exit_status = 127;
		return ;
	}
	if (access(cur->cmd, F_OK) != 0)
	{
		printf("minishell: %s: No such file or directory\n", cur->cmd);
		val->last_exit_status = 127;
		return ;
	}
	return ;
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

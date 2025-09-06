/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:18:12 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/06 17:00:34 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"

void	close_pipes(int *pipes, int count)
{
	int		i;

	i = 0;
	while (i < count)
	{
		if (pipes[i] >= 0)
			close(pipes[i]);
		i++;
	}
}

void	setup_child_io(int i, int n, int *pipes)
{
	int	rfd;
	int	wfd;

	rfd = -1;
	wfd = -1;
	if (i > 0)
	{
		rfd = pipes[2 * (i - 1)];
		if (dup2(rfd, STDIN_FILENO) == -1)
			_exit(1);
	}
	if (i < n - 1)
	{
		wfd = pipes[2 * i + 1];
		if (dup2(wfd, STDOUT_FILENO) == -1)
			_exit(1);
	}
}

static char	*expand_heredoc_line(char *line,
	t_shell_val *val, int expandable_fd)
{
	char	*expanded_line;
	int		i;

	if (!line)
		return (NULL);
	expanded_line = gc_strdup(line);
	i = 0;
	if (expandable_fd == 1)
		return (expanded_line);
	while (expanded_line[i])
	{
		if (expanded_line[i] == '$' && is_valid_var_start(&expanded_line[i]))
		{
			expanded_line = get_new_value(expanded_line, i, val);
			i = 0;
		}
		else
			i++;
	}
	return (expanded_line);
}

int	should_stop_heredoc(char *line, const char *delim, int line_number)
{
	if (!line || !ft_strcmp(line, delim))
	{
		if (!line)
			printf("minishell: warning: here-document at line %d "
				"delimited by end-of-file (wanted `%s')\n",
				line_number, delim);
		return (1);
	}
	return (0);
}

void	process_heredoc_line(char *line,
	int write_fd, t_shell_val *val, int expandable_fd)
{
	char	*expanded_line;

	expanded_line = expand_heredoc_line(line, val, expandable_fd);
	if (expanded_line)
	{
		write(write_fd, expanded_line, ft_strlen(expanded_line));
	}
	write(write_fd, "\n", 1);
}

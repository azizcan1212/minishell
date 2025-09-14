/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:18:12 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/13 05:42:01 by muharsla         ###   ########.fr       */
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

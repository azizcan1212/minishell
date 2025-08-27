/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:18:12 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 15:50:10 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	close_pipes(int *pipes, int count)
{
	int		i;

	i = 0;
	while (i < count)
	{
		close(pipes[i]);
		i++;
	}
}

void	setup_child_io(int i, int n, int *pipes)
{
	if (i > 0)
		dup2(pipes[(i - 1) * 2], 0);
	if (i < n - 1)
		dup2(pipes[i * 2 + 1], 1);
}

static char	*expand_heredoc_line(char *line, t_shell_val *val)
{
	char	*expanded_line;
	int		i;

	if (!line)
		return (NULL);
	expanded_line = ft_strdup(line);
	i = 0;
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

void	process_heredoc_line(char *line, int write_fd, t_shell_val *val)
{
	char	*expanded_line;

	expanded_line = expand_heredoc_line(line, val);
	if (expanded_line)
	{
		write(write_fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	write(write_fd, "\n", 1);
}

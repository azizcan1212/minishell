/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 03:45:38 by atam              #+#    #+#             */
/*   Updated: 2025/09/11 20:24:59 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <sys/types.h>

void	handle_input_redirection(t_token *next, t_command *cmd)
{
	cmd->input_file = gc_strdup(next->value);
}

void	handle_output_redirection(t_token *tk, t_token *next, t_command *cmd)
{
	cmd->output_file = gc_strdup(next->value);
	cmd->append = (tk->value[1] == '>');
	if (should_touch_file(cmd))
		touch_output_file(cmd->output_file, cmd->append);
}

t_hdoc	*create_heredoc_node(char *delim, int expandable)
{
	t_hdoc	*node;

	node = gc_malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->delim = gc_strdup(delim);
	node->expandable = expandable;
	node->next = NULL;
	return (node);
}

void	add_heredoc_to_list(t_command *cmd, t_hdoc *new_node)
{
	t_hdoc	*it;

	if (!cmd->heredoc_list)
		cmd->heredoc_list = new_node;
	else
	{
		it = cmd->heredoc_list;
		while (it->next)
			it = it->next;
		it->next = new_node;
	}
}

void	handle_heredoc_redirection(t_token *next, t_command *cmd)
{
	t_hdoc	*new_node;

	if (next->expandable_fd == 1)
		cmd->expandable_fd = 1;
	cmd->heredoc_delim = gc_strdup(next->value);
	new_node = create_heredoc_node(next->value, next->expandable_fd);
	if (new_node)
		add_heredoc_to_list(cmd, new_node);
}

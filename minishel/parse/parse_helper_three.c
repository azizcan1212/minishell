/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper_three.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 03:46:36 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 03:46:44 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <sys/types.h>

void	handle_normal_token(t_token *tk, t_command *cmd, int *argc)
{
	if (!cmd->cmd)
		cmd->cmd = gc_strdup(tk->value);
	cmd->args[*argc] = gc_strdup(tk->value);
	(*argc)++;
}

void	handle_pipe_token(t_command **cur, int *argc)
{
	(*cur)->args[*argc] = NULL;
	(*cur)->next = new_command();
	*cur = (*cur)->next;
	*argc = 0;
}

int	is_redirection_token(t_token *token)
{
	if (!token || !token->value || token->type != META)
		return (0);
	return (token->value[0] == '<' || token->value[0] == '>');
}

void	handle_redirection_with_touch(t_token **tokens, t_command *cur)
{
	int		is_out;
	int		is_append;
	char	*fname;

	is_out = ((*tokens)->value[0] == '>');
	is_append = (is_out && (*tokens)->value[1] == '>');
	fname = NULL;
	if ((*tokens)->next)
		fname = (*tokens)->next->value;
	handle_redirection(tokens, cur);
	if (is_out && should_touch_file(cur) && fname)
		touch_output_file(fname, is_append);
}

void	init_current_command(t_command **cur, t_command **head, int *argc)
{
	if (!*cur)
	{
		*cur = new_command();
		if (!*head)
			*head = *cur;
		*argc = 0;
	}
}

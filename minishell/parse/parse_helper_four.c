/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper_four.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 03:47:20 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 03:47:38 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <sys/types.h>

void	process_one_token(t_token **tokens, t_command **cur,
	t_command **head, int *argc)
{
	init_current_command(cur, head, argc);
	if (token_is_pipe(*tokens))
		handle_pipe_token(cur, argc);
	else if (is_redirection_token(*tokens))
		handle_redirection_with_touch(tokens, *cur);
	else
		handle_normal_token(*tokens, *cur, argc);
	*tokens = (*tokens)->next;
}

void	handle_redirection(t_token **tk, t_command *cmd)
{
	t_token	*next;

	next = (*tk)->next;
	if (!next || !next->value)
		return ;
	if (!ft_strcmp((*tk)->value, "<"))
		handle_input_redirection(next, cmd);
	else if (!ft_strcmp((*tk)->value, ">") || !ft_strcmp((*tk)->value, ">>"))
		handle_output_redirection(*tk, next, cmd);
	else if (!ft_strcmp((*tk)->value, "<<"))
		handle_heredoc_redirection(next, cmd);
	*tk = next;
}

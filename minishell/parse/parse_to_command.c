/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:54 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 20:00:34 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"

t_command	*parse_tokens_to_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*cur;
	int			argc;

	head = NULL;
	cur = NULL;
	argc = 0;
	while (tokens)
		process_one_token(&tokens, &cur, &head, &argc);
	if (cur)
		cur->args[argc] = NULL;
	return (head);
}

void	main_shell_loop(t_shell_state *state, char **envp)
{
	while (1)
	{
		process_pending_signal(state);
		state->input = read_input_with_history();
		process_pending_signal(state);
		signal(SIGINT, SIG_IGN);
		if (!process_input_line(state, envp))
			break ;
		signal(SIGINT, signal_handler);
		if (state->cmds)
			close_all_heredocs(state->cmds);
		fd_gc_cleanup();
	}
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:54 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 07:34:40 by atam             ###   ########.fr       */
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
		fd_gc_cleanup();
	}
}

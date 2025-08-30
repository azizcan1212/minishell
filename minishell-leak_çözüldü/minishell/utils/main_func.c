/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: main_func                                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 00:00:00 by gc                #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "gc.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>

char	*read_input_with_history(void)
{
	char	*input;
	char	*gc_input;

	input = readline("minishell>> ");
	if (!input)
	{
		printf("exit\n");
		return (NULL);
	}
	if (*input)
	{
		add_history(input);
		gc_input = gc_strdup(input); // Copy to GC managed memory
		free(input); // Free the readline allocated memory
		return (gc_input);
	}
	gc_input = gc_strdup(input);
	free(input); // Free the readline allocated memory
	return (gc_input);
}

void	cleanup_memory(t_token *tokens, t_shell_val *val, char *input)
{
	/* 
	 * With GC, individual cleanup is not needed
	 * All will be cleaned up by gc_cleanup() at program end
	 */
	(void)tokens;
	(void)val;
	(void)input;
}

void	init_shell_state(t_shell_state *state)
{
	/* Initialize GC if not already done */
	if (!g_gc)
		gc_init();
		
	state->input = NULL;
	state->tokens = NULL;
	state->expansion = NULL;
	state->val = new_shell_val();
	state->cmds = NULL;
	state->token_check = 0;
	state->env_status = 0;
	state->equal_status = NO_EQUAL;
}

int	process_tokens(t_shell_state *state)
{
	state->tokens = tokenize(state->input);
	if (!state->tokens)
	{
		return (0);
	}

	print_tokens(state->tokens);

	// Syntax validation
	if (!validate_syntax(state->tokens))
	{
		state->val->last_exit_status = 2;
		return (0);
	}

	print_tokens(state->tokens);
	control_equal(state->tokens);
	state->equal_status = get_equal_status(state->tokens);
	return (1);
}

void print_tokens(t_token *tokens)
{
	t_token *cur = tokens;
	while (cur)
	{
		printf("Token: %s, Type: %d, Expandable FD: %d\n", cur->value, cur->type, cur->expandable_fd);
		cur = cur->next;
	}
}
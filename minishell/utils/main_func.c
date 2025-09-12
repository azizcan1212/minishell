/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:38:57 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 07:28:26 by atam             ###   ########.fr       */
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
		gc_input = gc_strdup(input);
		free(input);
		return (gc_input);
	}
	gc_input = gc_strdup(input);
	free(input);
	return (gc_input);
}

void	init_shell_state(t_shell_state *state)
{
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
	if (!validate_syntax(state->tokens))
	{
		state->val->last_exit_status = 2;
		return (0);
	}
	control_equal(state->tokens);
	state->equal_status = get_equal_status(state->tokens);
	return (1);
}

void	handle_execution(t_shell_state *state, char **envp)
{
	state->env_status = set_environment(state->tokens,
			&state->expansion, state->equal_status);
	if (is_export_command(state->tokens))
	{
		execute_command(state->cmds, envp, state->val);
	}
	else if (state->equal_status != VALID
		&& !state->token_check && state->env_status == 0)
	{
		execute_command(state->cmds, envp, state->val);
	}
}

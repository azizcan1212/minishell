#include "../minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>

char	*read_input_with_history(void)
{
	char	*input;

	input = readline("minishell>> ");
	if (!input)
	{
		printf("exit\n");
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}

void	cleanup_memory(t_token *tokens, t_shell_val *val, char *input)
{
	(void)val;
	free_tokens(tokens);
	free(input);
}

void	init_shell_state(t_shell_state *state)
{
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
		free(state->input);
		return (0);
	}

	printf("Tokens after tokenization:\n");

	print_tokens(state->tokens);
	

	// Syntax validation
	if (!validate_syntax(state->tokens))
	{
		state->val->last_exit_status = 2;
		free_tokens(state->tokens);
		free(state->input);
		return (0);
	}

	printf("Tokens after syntax validation:\n");
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
		printf("Token: %s, Type: %d\n", cur->value, cur->type);
		cur = cur->next;
	}
}

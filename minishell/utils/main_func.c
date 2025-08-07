#include "../minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

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
	control_equal(state->tokens);
	state->equal_status = get_equal_status(state->tokens);
	return (1);
}

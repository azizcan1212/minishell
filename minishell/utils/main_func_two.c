#include "minishell.h"

static t_token	*find_invalid_token(t_token *head)
{
	t_token		*cur;

	cur = head;
	while (cur)
	{
		if (cur->equal_status == INVALID)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

t_token	*remove_tokens_before_invalid(t_token *head)
{
	t_token		*cur;
	t_token		*invalid_token;
	t_token		*temp;

	invalid_token = find_invalid_token(head);
	if (!invalid_token)
		return (head);
	cur = head;
	while (cur && cur != invalid_token)
	{
		temp = cur;
		cur = cur->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
	if (invalid_token)
		invalid_token->prev = NULL;
	return (invalid_token);
}

int	process_commands(t_shell_state *state)
{
	expand_dollar(state->tokens, state->val);
	state->token_check = check_tokens_is_null(state->tokens);
	delete_dollars(state->tokens);
	state->tokens = merge_token(state->tokens);
	if (state->equal_status == INVALID)
		state->tokens = remove_tokens_before_invalid(state->tokens);
	state->cmds = parse_tokens_to_commands(state->tokens);
	if (!state->cmds)
	{
		cleanup_memory(state->tokens, state->val, state->input);
		return (0);
	}
	return (1);
}

void	handle_execution(t_shell_state *state, char **envp)
{
	state->env_status = set_environment(state->tokens,
			&state->expansion, state->equal_status);
	if (state->equal_status != VALID
		&& !state->token_check && state->env_status == 0)
		execute_command(state->cmds, envp, state->val);
}

void	cleanup_state(t_shell_state *state)
{
	free_commands(state->cmds);
	cleanup_memory(state->tokens, state->val, state->input);
}

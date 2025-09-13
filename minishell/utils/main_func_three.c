/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_func_three.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 02:30:10 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/13 02:31:03 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	invalid_token = find_invalid_token(head);
	if (!invalid_token)
		return (head);
	cur = head;
	while (cur && cur != invalid_token)
	{
		cur = cur->next;
	}
	if (invalid_token)
		invalid_token->prev = NULL;
	return (invalid_token);
}

t_token	*remove_empty_tokens(t_token *head)
{
	t_token	*current;
	t_token	*prev;

	current = head;
	prev = NULL;
	while (current)
	{
		if ((!current->value && current->type == WORD)
			|| (ft_strlen(current->value) == 0 && current->type == WORD))
		{
			if (prev)
				prev->next = current->next;
			else
				head = current->next;
			current = current->next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	return (head);
}

int	is_export_command(t_token *head)
{
	if (head && head->value && !ft_strcmp(head->value, "export"))
		return (1);
	return (0);
}

int	process_commands(t_shell_state *state)
{
	expand_dollar(state->tokens, state->val);
	state->token_check = check_tokens_is_null(state->tokens);
	delete_dollars(state->tokens);
	state->tokens = remove_empty_tokens(state->tokens);
	state->tokens = split_unquoted_ws_tokens(state->tokens);
	state->tokens = merge_token(state->tokens);
	if (!state->tokens)
		return (0);
	if (state->equal_status == INVALID && !is_export_command(state->tokens))
		state->tokens = remove_tokens_before_invalid(state->tokens);
	state->cmds = parse_tokens_to_commands(state->tokens);
	if (!state->cmds)
		return (0);
	return (1);
}

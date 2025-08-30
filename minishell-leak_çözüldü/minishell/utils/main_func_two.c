/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_func_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: main_func_two                               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 00:00:00 by gc                #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "gc.h"
#include <stdio.h>

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
		/* With GC, no need to manually free tokens */
	}
	if (invalid_token)
		invalid_token->prev = NULL;
	return (invalid_token);
}

t_token *remove_empty_tokens(t_token *head)
{
    t_token *current = head;
    t_token *prev = NULL;
    while (current)
    {
        if (!current->value || ft_strlen(current->value) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                head = current->next;
            current = current->next;
            /* With GC, no need to manually free token and its value */
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    return head;
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
	state->tokens = merge_token(state->tokens);

	print_tokens(state->tokens);
	
	// Export komutu ise invalid token'ları kaldırma
	if (state->equal_status == INVALID && !is_export_command(state->tokens))
		state->tokens = remove_tokens_before_invalid(state->tokens);
		
	state->cmds = parse_tokens_to_commands(state->tokens);

	print_tokens(state->tokens);
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
	
	// Export komutu ise her durumda execute et (validation için)
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

void	cleanup_state(t_shell_state *state)
{
	/*
	 * With GC, individual cleanup is not needed
	 * All will be cleaned up by gc_cleanup() at program end
	 */
	(void)state;
}
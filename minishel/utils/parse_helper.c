/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:15:59 by atam              #+#    #+#             */
/*   Updated: 2025/09/13 05:42:59 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"
#include "gc.h"

void	delete_dollars(t_token *token)
{
	char	*new_value;
	t_token	*cur;
	t_token	*to_delete;

	cur = token;
	while (cur && cur->next)
	{
		if (cur->value[0] == '$'
			&& cur->value[1] == '\0'
			&& (cur->next->type == SINGLE_QUOTE
				|| cur->next->type == DOUBLE_QUOTE))
		{
			new_value = gc_strdup(cur->next->value);
			cur->value = new_value;
			to_delete = cur->next;
			cur->next = to_delete->next;
			if (to_delete->next)
				to_delete->next->prev = cur;
		}
		else
			cur = cur->next;
	}
}

int	is_token_invalid_before_equal(t_token *equal_token)
{
	t_token	*temp;

	temp = equal_token->prev;
	if (!temp)
		return (1);
	while (temp && temp->space_next_status == 0)
	{
		if (!is_valid_identifier(temp->value) || temp->type == SINGLE_QUOTE
			|| temp->type == DOUBLE_QUOTE)
			return (1);
		temp = temp->prev;
	}
	return (0);
}

void	control_equal(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur)
	{
		if (cur->value && cur->value[0] == '=')
		{
			if (is_token_invalid_before_equal(cur))
			{
				cur->equal_status = INVALID;
			}
			else
			{
				cur->equal_status = VALID;
			}
		}
		cur = cur->next;
	}
}

t_equal	get_equal_status(t_token *head)
{
	t_token	*cur;
	int		valid_equal_found;

	cur = head;
	valid_equal_found = 0;
	while (cur)
	{
		if (cur->equal_status == INVALID)
			return (INVALID);
		else if (cur->equal_status == VALID)
			valid_equal_found = 1;
		cur = cur->next;
	}
	if (valid_equal_found)
		return (VALID);
	return (NO_EQUAL);
}

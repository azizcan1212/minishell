/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:01 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/28 20:07:21 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

char	*value(const char **p)
{
	int	meta_len;

	meta_len = 0;
	if (**p == '\'')
		return (parse_single_quote(p));
	else if (**p == '"')
		return (parse_double_quote(p));
	else if (is_meta(*p, &meta_len))
		return (parse_meta(p, &meta_len));
	else
		return (parse_word(p));
}

t_token	*tokenize(const char *input)
{
	t_token		*list;
	const char	*p;
	char		*val;
	const char	*index;

	list = NULL;
	p = input;
	while (*p)
	{
		while (ft_isspace(*p))
			p++;
		if (*p)
		{
			index = p;
			val = value(&p);
			if (!val)
			{
				free_tokens(list);
				return (NULL);
			}
			add_token_with_type(&list, val, index, p);
		}
	}
	return (list);
}

static int	should_merge_tokens(t_token *current)
{
	return (current->space_next_status == 0
		&& !(current->type == META && current->value[0] != '=')
		&& !(current->next->type == META && current->next->value[0] != '='));
}

static void	merge_current_with_next(t_token *current)
{
	t_token		*next_token;
	char		*merged_value;

	next_token = current->next;
	merged_value = ft_strjoin(current->value, next_token->value);
	if(current->type == SINGLE_QUOTE || next_token->type == SINGLE_QUOTE)
		current->expandable_fd = 1;
	else if(current->type == DOUBLE_QUOTE || next_token->type == DOUBLE_QUOTE)
		current->expandable_fd = 1;
	current->type = WORD;
	free(current->value);
	current->value = merged_value;
	current->space_next_status = next_token->space_next_status;
	if (current->equal_status == INVALID || next_token->equal_status == INVALID)
		current->equal_status = INVALID;
	else if (next_token->equal_status != NO_EQUAL)
		current->equal_status = next_token->equal_status;
	current->next = next_token->next;
	if (next_token->next)
		next_token->next->prev = current;
	free(next_token->value);
	free(next_token);
}

t_token	*merge_token(t_token *head)
{
	t_token	*current;

	current = head;
	while (current && current->next)
	{
		if (should_merge_tokens(current))
			merge_current_with_next(current);
		else
			current = current->next;
	}
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:13 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 17:19:20 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../libft/libft.h"

t_token	*new_token(char *val)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = val;
	tok->next = NULL;
	tok->prev = NULL;
	tok->type = WORD;
	tok->equal_status = NO_EQUAL;
	tok->space_next_status = 0;
	return (tok);
}

void	add_token_back(t_token **head, t_token *new_token)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new_token;
		new_token->prev = NULL;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
	new_token->prev = tmp;
}

void	token_type(const char *index, t_token *token)
{
	int	len;

	len = 0;
	if (index[0] == '\'')
		token->type = SINGLE_QUOTE;
	else if (index[0] == '\"')
		token->type = DOUBLE_QUOTE;
	else if (is_meta(index, &len))
		token->type = META;
	else
		token->type = WORD;
}

void	add_token_with_type(t_token **list,
	char *value, const char *i, const char *p)
{
	t_token	*new_tok;

	if (value)
	{
		new_tok = new_token(value);
		if (!new_tok)
			return ;
		token_type(i, new_tok);
		if (*(p))
		{
			if (ft_isspace(*(p)))
				new_tok->space_next_status = 1;
		}
		add_token_back(list, new_tok);
	}
}

int	check_tokens_is_null(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur)
	{
		if (!(cur->type == WORD && cur->value[0] == '\0'))
			return (0);
		cur = cur->next;
	}
	return (1);
}

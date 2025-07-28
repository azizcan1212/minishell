/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_to_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:54 by muharsla          #+#    #+#             */
/*   Updated: 2025/07/11 16:43:55 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse_tokens_to_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*cur;
	int			argc;

	head = NULL;
	cur = NULL;
	argc = 0;
	while (tokens)
		process_one_token(&tokens, &cur, &head, &argc);
	if (cur)
		cur->args[argc] = NULL;
	return (head);
}

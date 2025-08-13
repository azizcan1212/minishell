/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:51 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 16:43:08 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "libft.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->args = malloc(sizeof(char *) * 100);
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->heredoc_delim = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

int	token_is_pipe(t_token *tk)
{
	if (!tk || !tk->value)
		return (0);
	return (!ft_strcmp(tk->value, "|"));
}

void	handle_redirection(t_token **tk, t_command *cmd)
{
	t_token	*next;

	next = (*tk)->next;
	// Note: Syntax validation is now handled in validate_syntax()
	if (!next || !next->value)
		return;
		
	if (!ft_strcmp((*tk)->value, "<"))
		cmd->input_file = ft_strdup(next->value);
	else if (!ft_strcmp((*tk)->value, ">"))
	{
		cmd->output_file = ft_strdup(next->value);
		cmd->append = 0;
	}
	else if (!ft_strcmp((*tk)->value, ">>"))
	{
		cmd->output_file = ft_strdup(next->value);
		cmd->append = 1;
	}
	else if (!ft_strcmp((*tk)->value, "<<"))
		cmd->heredoc_delim = ft_strdup(next->value);
	*tk = next;
}

void	handle_normal_token(t_token *tk, t_command *cmd, int *argc)
{
	if (!cmd->cmd)
		cmd->cmd = ft_strdup(tk->value);
	cmd->args[*argc] = ft_strdup(tk->value);
	(*argc)++;
}

void	process_one_token(
	t_token **tokens, t_command **cur, t_command **head, int *argc)
{
	if (!*cur)
	{
		*cur = new_command();
		if (!*head)
			*head = *cur;
		*argc = 0;
	}
	if (token_is_pipe(*tokens))
	{
		(*cur)->args[*argc] = NULL;
		(*cur)->next = new_command();
		*cur = (*cur)->next;
		*argc = 0;
	}
	else if (((*tokens)->value[0] == '<'
			|| (*tokens)->value[0] == '>') && (*tokens)->type == META)
		handle_redirection(tokens, *cur);
	else
		handle_normal_token(*tokens, *cur, argc);
	*tokens = (*tokens)->next;
}

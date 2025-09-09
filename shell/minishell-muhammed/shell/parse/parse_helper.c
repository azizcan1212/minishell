/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:51 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/06 20:58:12 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gc.h"
#include <stdlib.h>
#include "libft.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = gc_malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd -> heredoc_list = NULL;
	cmd->args = gc_malloc(sizeof(char *) * 100);
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_fd = -1;
	cmd->append = 0;
	cmd->next = NULL;
	cmd->expandable_fd = 0;
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
	int		fd;
	int		block_touch;

	next = (*tk)->next;
	if (!next || !next->value)
		return ;
	if (!ft_strcmp((*tk)->value, "<"))
	{
		cmd->input_file = gc_strdup(next->value);
	}
	else if (!ft_strcmp((*tk)->value, ">") || !ft_strcmp((*tk)->value, ">>"))
	{
		cmd->output_file = gc_strdup(next->value);
		cmd->append = ((*tk)->value[1] == '>');
		if (!cmd->cmd || cmd->cmd[0] == '\0')
		{
			block_touch = 0;
			if (cmd->input_file)
			{
				fd = open(cmd->input_file, O_RDONLY);
				if (fd < 0)
					block_touch = 1;
				else
					close(fd);
			}
			if (!block_touch)
			{
				fd = open(cmd->output_file,
						O_CREAT | O_WRONLY
						| (cmd->append ? O_APPEND : O_TRUNC), 0644);
				if (fd >= 0)
					close(fd);
			}
		}
	}
	else if (!ft_strcmp((*tk)->value, "<<"))
	{
		t_hdoc *n, *it;

		if (next->expandable_fd == 1)
			cmd->expandable_fd = 1;
		cmd->heredoc_delim = gc_strdup(next->value);  /* geriye uyumluluk */

		/* YENİ: bu heredoc’u listeye push-back */
		n = gc_malloc(sizeof(*n));
		n->delim = gc_strdup(next->value);
		n->expandable = next->expandable_fd;   /* heredoc’a özel expand bilgisi */
		n->next = NULL;
		if (!cmd->heredoc_list)
			cmd->heredoc_list = n;
		else
		{
			it = cmd->heredoc_list;
			while (it->next)
				it = it->next;
			it->next = n;
		}
	}

	*tk = next;
}

void	handle_normal_token(t_token *tk, t_command *cmd, int *argc)
{
	if (!cmd->cmd)
		cmd->cmd = gc_strdup(tk->value);
	cmd->args[*argc] = gc_strdup(tk->value);
	(*argc)++;
}

void	process_one_token(
	t_token **tokens, t_command **cur, t_command **head, int *argc)
{
	int		is_out;
	int		is_append;
	int		block_touch;
	char	*fname;

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
	{
		is_out = ((*tokens)->value[0] == '>');
		is_append = (is_out && (*tokens)->value[1] == '>');
		fname = ((*tokens)->next) ? (*tokens)->next->value : NULL;
		block_touch = 0;
		if (!(*cur)->cmd || (*cur)->cmd[0] == '\0')
		{
			if ((*cur)->input_file)
			{
				int t = open((*cur)->input_file, O_RDONLY);
				if (t < 0)
					block_touch = 1;
				else
					close(t);
			}
		}
		handle_redirection(tokens, *cur);
		if (is_out && !block_touch
			&& (!(*cur)->cmd || (*cur)->cmd[0] == '\0') && fname)
		{
			int	t = open(fname,
					O_CREAT | O_WRONLY
					| (is_append ? O_APPEND : O_TRUNC), 0644);
			if (t >= 0)
				close(t);
		}
	}
	else
		handle_normal_token(*tokens, *cur, argc);
	*tokens = (*tokens)->next;
}

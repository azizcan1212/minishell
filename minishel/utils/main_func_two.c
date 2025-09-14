/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_func_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:25:06 by atam              #+#    #+#             */
/*   Updated: 2025/09/13 02:33:07 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "gc.h"
#include <stdio.h>

static void	split_finish(t_split_ctx *c)
{
	if (!c->last)
		return ;
	c->last->space_next_status = c->orig_space_next;
	c->last->next = c->after;
	if (c->after)
		c->after->prev = c->last;
}

static int	split_take_next_piece(t_split_ctx *c, char **piece)
{
	while (c->str[c->i] && ft_isspace(c->str[c->i]))
		c->i++;
	c->start = c->i;
	while (c->str[c->i] && !ft_isspace(c->str[c->i]))
		c->i++;
	if (c->i <= c->start)
	{
		*piece = NULL;
		return (0);
	}
	*piece = gc_substr(c->str, c->start, c->i - c->start);
	return (*piece != NULL);
}

static void	split_append_piece(t_split_ctx *c, char *piece)
{
	t_token	*nt;

	if (!c->wrote_first)
	{
		c->tok->value = piece;
		c->tok->type = WORD;
		c->tok->expandable_fd = 0;
		c->tok->space_next_status = 1;
		c->last = c->tok;
		c->wrote_first = 1;
		return ;
	}
	nt = new_token(piece);
	if (!nt)
		return ;
	nt->type = WORD;
	nt->space_next_status = 1;
	nt->prev = c->last;
	nt->next = NULL;
	c->last->next = nt;
	c->last = nt;
}

static void	split_step(t_split_ctx *c)
{
	char	*piece;

	if (!split_take_next_piece(c, &piece) || !piece)
		return ;
	split_append_piece(c, piece);
}

void	split_token_in_place(t_token *tok)
{
	t_split_ctx	c;

	if (!tok || !tok->value)
		return ;
	c.str = tok->value;
	c.i = 0;
	c.wrote_first = 0;
	c.orig_space_next = tok->space_next_status;
	c.after = tok->next;
	c.last = tok;
	c.tok = tok;
	while (c.str[c.i])
		split_step(&c);
	split_finish(&c);
}

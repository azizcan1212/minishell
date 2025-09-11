/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_seven.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:00:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 05:02:11 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdlib.h>

void	gc_mark_ptr(void *ptr)
{
	t_gc_node	*node;

	if (!ptr)
		return ;
	node = find_gc_node(ptr);
	if (node)
		node->marked = 1;
}

void	gc_mark_all(void)
{
	t_gc		*gc;
	t_gc_node	*node;

	gc = get_gc_instance();
	node = gc->head;
	while (node)
	{
		node->marked = 1;
		node = node->next;
	}
}

static void	free_unmarked_node(t_gc *gc, t_gc_node *temp)
{
	gc->total_allocated -= temp->size;
	gc->allocation_count--;
	free(temp->ptr);
	free(temp);
}

static void	process_node(t_gc *gc, t_gc_node **node,
	t_gc_node **prev)
{
	t_gc_node	*temp;

	if (!(*node)->marked)
	{
		temp = *node;
		if (*prev)
			(*prev)->next = (*node)->next;
		else
			gc->head = (*node)->next;
		*node = (*node)->next;
		free_unmarked_node(gc, temp);
	}
	else
	{
		(*node)->marked = 0;
		*prev = *node;
		*node = (*node)->next;
	}
}

void	gc_sweep(void)
{
	t_gc		*gc;
	t_gc_node	*node;
	t_gc_node	*prev;

	gc = get_gc_instance();
	prev = NULL;
	node = gc->head;
	while (node)
		process_node(gc, &node, &prev);
}

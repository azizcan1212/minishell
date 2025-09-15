/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_last.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:54:55 by atam              #+#    #+#             */
/*   Updated: 2025/09/15 17:59:14 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include <stdlib.h>
#include <unistd.h>

static void	reset_gc_instance(t_gc *gc)
{
	gc->head = NULL;
	gc->total_allocated = 0;
	gc->allocation_count = 0;
}

void	gc_cleanup(void)
{
	t_gc		*gc;
	t_gc_node	*node;
	t_gc_node	*temp;

	gc = get_gc_instance();
	node = gc->head;
	while (node)
	{
		temp = node;
		node = node->next;
		free(temp->ptr);
		free(temp);
	}
	reset_gc_instance(gc);
}

void	free_gc_node(void)
{
	t_gc		*gc;
	t_gc_node	*prev;
	t_gc_node	*node;

	gc = get_gc_instance();
	if (!gc->head)
		return ;
	prev = NULL;
	node = gc->head;
	while (node->next)
	{
		prev = node;
		node = node->next;
	}
	if (prev)
		prev->next = NULL;
	else
		gc->head = NULL;
	if (gc->total_allocated >= node->size)
		gc->total_allocated -= node->size;
	if (gc->allocation_count > 0)
		gc->allocation_count--;
	free(node->ptr);
	free(node);
}

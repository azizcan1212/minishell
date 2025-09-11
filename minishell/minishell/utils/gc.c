/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:00:33 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 05:00:34 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

t_gc	*get_gc_instance(void)
{
	static t_gc	gc_instance = {0};
	static int	initialized = 0;

	if (!initialized)
	{
		gc_instance.head = NULL;
		gc_instance.total_allocated = 0;
		gc_instance.allocation_count = 0;
		initialized = 1;
	}
	return (&gc_instance);
}

t_gc	*gc_init(void)
{
	return (get_gc_instance());
}

t_gc_node	*gc_create_node(void *ptr, size_t size)
{
	t_gc_node	*node;

	node = malloc(sizeof(t_gc_node));
	if (!node)
		return (NULL);
	node->ptr = ptr;
	node->size = size;
	node->marked = 0;
	node->next = NULL;
	return (node);
}

void	gc_append_node(t_gc *gc, t_gc_node *node)
{
	t_gc_node	*current;

	if (!gc->head)
		gc->head = node;
	else
	{
		current = gc->head;
		while (current->next)
			current = current->next;
		current->next = node;
	}
}

void	gc_add_node(void *ptr, size_t size)
{
	t_gc		*gc;
	t_gc_node	*node;

	gc = get_gc_instance();
	node = gc_create_node(ptr, size);
	if (!node)
		return ;
	gc_append_node(gc, node);
	gc->total_allocated += size;
	gc->allocation_count++;
}

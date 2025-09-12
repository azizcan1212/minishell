/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_last.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:54:55 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 05:02:20 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdlib.h>

void	gc_collect(void)
{
	gc_sweep();
}

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

size_t	gc_get_allocated_memory(void)
{
	t_gc	*gc;

	gc = get_gc_instance();
	return (gc->total_allocated);
}

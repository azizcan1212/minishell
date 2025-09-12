/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_two.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:00:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 04:52:09 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdlib.h>

t_gc_node	*find_gc_node(void *ptr)
{
	t_gc		*gc;
	t_gc_node	*node;

	gc = get_gc_instance();
	node = gc->head;
	while (node && node->ptr != ptr)
		node = node->next;
	return (node);
}

void	*gc_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc_add_node(ptr, size);
	return (ptr);
}

void	*gc_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = gc_malloc(count * size);
	if (ptr)
		ft_memset(ptr, 0, count * size);
	return (ptr);
}

static size_t	get_copy_size(size_t old_size, size_t new_size)
{
	if (old_size < new_size)
		return (old_size);
	return (new_size);
}

void	*gc_copy_and_free(t_gc_node *node, void *ptr, size_t new_size)
{
	void	*new_ptr;
	size_t	to_copy;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (node->size)
	{
		to_copy = get_copy_size(node->size, new_size);
		ft_memcpy(new_ptr, ptr, to_copy);
	}
	free(ptr);
	return (new_ptr);
}

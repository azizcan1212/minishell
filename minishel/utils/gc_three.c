/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_three.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:00:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/13 05:39:03 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdlib.h>

static void	*handle_realloc_null(size_t new_size)
{
	return (malloc(new_size));
}

static void	*handle_realloc_zero(void *ptr)
{
	free(ptr);
	return (NULL);
}

static void	*ft_realloc(void *ptr, size_t new_size)
{
	t_gc_node	*node;

	if (!ptr)
		return (handle_realloc_null(new_size));
	if (new_size == 0)
		return (handle_realloc_zero(ptr));
	node = find_gc_node(ptr);
	if (!node)
		return (NULL);
	return (gc_copy_and_free(node, ptr, new_size));
}

static void	update_node_info(t_gc_node *node, void *new_ptr,
	size_t new_size, t_gc *gc)
{
	gc->total_allocated = gc->total_allocated - node->size + new_size;
	node->ptr = new_ptr;
	node->size = new_size;
}

void	*gc_realloc(void *ptr, size_t new_size)
{
	t_gc		*gc;
	t_gc_node	*node;
	void		*new_ptr;

	if (!ptr)
		return (gc_malloc(new_size));
	if (!new_size)
		return (NULL);
	node = find_gc_node(ptr);
	if (!node)
		return (NULL);
	new_ptr = ft_realloc(ptr, new_size);
	if (!new_ptr)
		return (NULL);
	gc = get_gc_instance();
	update_node_info(node, new_ptr, new_size, gc);
	return (new_ptr);
}

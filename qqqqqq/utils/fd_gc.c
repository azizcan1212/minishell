/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_gc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 06:39:44 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 07:33:51 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "gc.h"
#include "fd_gc.h"

static t_fd_gc	*get_fd_gc_instance(void)
{
	static t_fd_gc	instance = {NULL, 0, 0};

	return (&instance);
}

static void	copy_fds(int *new_fds, int *old_fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_fds[i] = old_fds[i];
		i++;
	}
}

static void	expand_fd_array(t_fd_gc *gc)
{
	int	new_capacity;
	int	*new_fds;

	if (gc->capacity)
		new_capacity = gc->capacity * 2;
	else
		new_capacity = 8;
	new_fds = gc_malloc(sizeof(int) * new_capacity);
	if (gc->fds)
		copy_fds(new_fds, gc->fds, gc->count);
	gc->fds = new_fds;
	gc->capacity = new_capacity;
}

void	fd_gc_add(int fd)
{
	t_fd_gc	*gc;

	gc = get_fd_gc_instance();
	/* Only track valid, non-negative FDs */
	if (fd < 0)
		return ;
	if (gc->count == gc->capacity)
		expand_fd_array(gc);
	gc->fds[gc->count++] = fd;
}

void	fd_gc_cleanup(void)
{
	t_fd_gc	*gc;
	int		i;

	gc = get_fd_gc_instance();
	i = 0;
	while (i < gc->count)
	{
		if (gc->fds[i] >= 0)
			close(gc->fds[i]);
		i++;
	}
	gc->fds = NULL;
	gc->count = 0;
	gc->capacity = 0;
}

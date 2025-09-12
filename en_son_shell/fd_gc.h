/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_gc.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 06:50:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 07:35:26 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FD_GC_H
# define FD_GC_H

# include <stdlib.h>

typedef struct s_fd_gc
{
	int	*fds;
	int	count;
	int	capacity;
}	t_fd_gc;

void	fd_gc_add(int fd);
void	fd_gc_cleanup(void);

#endif

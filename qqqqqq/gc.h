/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 06:51:23 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 06:59:02 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H

# include <stdlib.h>

typedef struct s_gc_node
{
	void				*ptr;
	size_t				size;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*head;
	size_t		total_allocated;
	size_t		allocation_count;
}	t_gc;

t_gc		*gc_init(void);
t_gc		*get_gc_instance(void);
void		*gc_malloc(size_t size);
void		*gc_calloc(size_t count, size_t size);
void		*gc_realloc(void *ptr, size_t new_size);
char		*gc_strdup(const char *s);
char		*gc_strndup(const char *s, size_t n);
char		*gc_substr(const char *s, unsigned int start, size_t len);
char		*gc_strjoin(const char *s1, const char *s2);
char		**gc_split(const char *s, char c);
char		*gc_itoa(int n);

t_gc_node	*gc_create_node(void *ptr, size_t size);
void		gc_add_node(void *ptr, size_t size);
t_gc_node	*find_gc_node(void *ptr);
void		*gc_copy_and_free(t_gc_node *node, void *ptr, size_t new_size);

void		gc_cleanup(void);


#endif

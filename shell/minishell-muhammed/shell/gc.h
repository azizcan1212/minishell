#ifndef GC_H
# define GC_H

# include <stdlib.h>

typedef struct s_gc_node
{
	void			*ptr;
	size_t			size;
	int				marked;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*head;
	size_t		total_allocated;
	size_t		allocation_count;
}	t_gc;


/* Core GC functions */
t_gc	*gc_init(void);
void	*gc_malloc(size_t size);
void	*gc_calloc(size_t count, size_t size);
void	*gc_realloc(void *ptr, size_t new_size);
char	*gc_strdup(const char *s);
char	*gc_strndup(const char *s, size_t n);
char	*gc_substr(const char *s, unsigned int start, size_t len);
char	*gc_strjoin(const char *s1, const char *s2);
char	**gc_split(const char *s, char c);
char	*gc_itoa(int n);

/* GC management */
void	gc_mark_ptr(void *ptr);
void	gc_mark_all(void);
void	gc_sweep(void);
void	gc_collect(void);
void	gc_cleanup(void);

/* Debug functions */
void	gc_print_stats(void);
size_t	gc_get_allocated_memory(void);

#endif
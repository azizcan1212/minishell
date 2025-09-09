#include "gc.h"
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static t_gc *get_gc_instance(void)
{
	static t_gc gc_instance = {0};
	static int initialized = 0;
	
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

static t_gc_node	*gc_create_node(void *ptr, size_t size)
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

static void	gc_add_node(void *ptr, size_t size)
{
	t_gc		*gc;
	t_gc_node	*node;
	t_gc_node	*current;

	gc = get_gc_instance();
	node = gc_create_node(ptr, size);
	if (!node)
		return ;
	if (!gc->head)
		gc->head = node;
	else
	{
		current = gc->head;
		while (current->next)
			current = current->next;
		current->next = node;
	}
	gc->total_allocated += size;
	gc->allocation_count++;
}

static void	*gc_copy_and_free(t_gc_node *node, void *ptr, size_t new_size)
{
	void	*new_ptr;
	size_t	to_copy;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (node->size)
	{
		if (node->size < new_size)
			to_copy = node->size;
		else
			to_copy = new_size;
		ft_memcpy(new_ptr, ptr, to_copy);
	}
	free(ptr);
	return (new_ptr);
}

static void	*ft_realloc(void *ptr, size_t new_size)
{
	t_gc		*gc;
	t_gc_node	*node;

	if (!ptr)
		return (malloc(new_size));
	else if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	else
	{
		gc = get_gc_instance();
		node = gc->head;
		while (node && node->ptr != ptr)
			node = node->next;
		if (!node)
			return (NULL);
		else
			return (gc_copy_and_free(node, ptr, new_size));
	}
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

void	*gc_realloc(void *ptr, size_t new_size)
{
	t_gc		*gc;
	t_gc_node	*node;
	void		*new_ptr;

	if (!ptr)
		return (gc_malloc(new_size));
	if (!new_size)
		return (NULL);
	gc = get_gc_instance();
	node = gc->head;
	while (node && node->ptr != ptr)
		node = node->next;
	if (!node)
		return (NULL);
	new_ptr = ft_realloc(ptr, new_size);
	if (!new_ptr)
		return (NULL);
	gc->total_allocated = gc->total_allocated - node->size + new_size;
	node->ptr = new_ptr;
	node->size = new_size;
	return (new_ptr);
}

char	*gc_strdup(const char *s)
{
	char	*dup;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strcpy(dup, s);
	return (dup);
}

char	*gc_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len > n)
		len = n;
	dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

char	*gc_substr(const char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	s_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (gc_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	substr = gc_malloc(len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[len] = '\0';
	return (substr);
}

char	*gc_strjoin(const char *s1, const char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = gc_malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		joined[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		joined[i + j] = s2[j];
		j++;
	}
	joined[i + j] = '\0';
	return (joined);
}

char	**gc_split(const char *s, char c)
{
	char	**result;
	size_t	count;
	size_t	i;
	size_t	start;
	size_t	result_i;

	if (!s)
		return (NULL);
	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	result = gc_malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	result_i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			result[result_i++] = gc_substr(s, start, i - start);
		}
		else
			i++;
	}
	result[result_i] = NULL;
	return (result);
}

char	*gc_itoa(int n)
{
	long tmp;
	size_t len;
	char *res;
	long t;

	tmp = n;
	len = 0;
	if (tmp <= 0)
		len = 1;
	t = tmp;
	while (t != 0)
	{
		t /= 10;
		len++;
	}
	res = gc_malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (n == 0)
	{
		res[0] = '0';
		return (res);
	}
	t = n;
	if (t < 0)
	{
		res[0] = '-';
		t = -t;
	}
	while (t != 0)
	{
		res[--len] = (t % 10) + '0';
		t /= 10;
	}
	return (res);
}

void	gc_mark_ptr(void *ptr)
{
	t_gc		*gc;
	t_gc_node	*node;

	if (!ptr)
		return ;
	gc = get_gc_instance();
	node = gc->head;
	while (node)
	{
		if (node->ptr == ptr)
		{
			node->marked = 1;
			return ;
		}
		node = node->next;
	}
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

void	gc_sweep(void)
{
	t_gc		*gc;
	t_gc_node	*node;
	t_gc_node	*prev;
	t_gc_node	*temp;

	gc = get_gc_instance();
	prev = NULL;
	node = gc->head;
	while (node)
	{
		if (!node->marked)
		{
			temp = node;
			if (prev)
				prev->next = node->next;
			else
				gc->head = node->next;
			node = node->next;
			gc->total_allocated -= temp->size;
			gc->allocation_count--;
			free(temp->ptr);
			free(temp);
		}
		else
		{
			node->marked = 0;
			prev = node;
			node = node->next;
		}
	}
}

void	gc_collect(void)
{
	gc_sweep();
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
	
	/* Reset the static instance */
	gc->head = NULL;
	gc->total_allocated = 0;
	gc->allocation_count = 0;
}

void	gc_print_stats(void)
{
	t_gc	*gc;

	gc = get_gc_instance();
	printf("GC Stats:\n");
	printf("  Total allocated: %zu bytes\n", gc->total_allocated);
	printf("  Active allocations: %zu\n", gc->allocation_count);
}

size_t	gc_get_allocated_memory(void)
{
	t_gc	*gc;

	gc = get_gc_instance();
	return (gc->total_allocated);
}
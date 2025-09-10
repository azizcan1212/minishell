/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_six.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:00:00 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 04:59:23 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

static size_t	get_num_length(int n)
{
	long	tmp;
	size_t	len;

	tmp = n;
	len = 0;
	if (tmp <= 0)
		len = 1;
	while (tmp != 0)
	{
		tmp /= 10;
		len++;
	}
	return (len);
}

static char	*handle_zero_case(void)
{
	char	*res;

	res = gc_malloc(2);
	if (!res)
		return (NULL);
	res[0] = '0';
	res[1] = '\0';
	return (res);
}

static void	fill_number_string(char *res, int n, size_t len)
{
	long	t;

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
}

char	*gc_itoa(int n)
{
	size_t	len;
	char	*res;

	if (n == 0)
		return (handle_zero_case());
	len = get_num_length(n);
	res = gc_malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	fill_number_string(res, n, len);
	return (res);
}

void	gc_print_stats(void)
{
	t_gc	*gc;

	gc = get_gc_instance();
	printf("GC Stats:\n");
	printf("  Total allocated: %zu bytes\n", gc->total_allocated);
	printf("  Active allocations: %zu\n", gc->allocation_count);
}

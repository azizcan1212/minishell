/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:00:00 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/11 12:15:54 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <unistd.h>
#include <stdlib.h>

static void	remove_key(t_expansion **head, const char *key)
{
	t_expansion	*cur;
	t_expansion	*prev;
	t_expansion	*next;

	if (head == NULL || *head == NULL || key == NULL)
		return;
	prev = NULL;
	cur = *head;
	while (cur != NULL)
	{
		next = cur->next;
		if (cur->key != NULL && ft_strcmp(cur->key, key) == 0)
		{
			if (prev != NULL)
				prev->next = next;
			else
				*head = next;
			cur->next = NULL;
			cur = next;
			continue;
		}
		prev = cur;
		cur = next;
	}
}

int	builtin_unset(char **args, t_expansion **head)
{
	int	i;
	int	ret;

	ret = 0;
	if (!args || !args[1])
		return (0);
	if (args[1][0] == '-' && args[1][1])
		return (write(2, "minishell: unset: invalid option\n", 33), 2);
	i = 1;
	while (args[i])
	{
		remove_key(head, args[i]);
		i++;
	}
	return (ret);
}

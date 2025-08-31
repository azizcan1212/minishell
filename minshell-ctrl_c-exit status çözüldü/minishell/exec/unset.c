/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 15:00:00 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "gc.h"
#include <unistd.h>
#include <stdlib.h>

static int	valid_ident(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum((unsigned char)s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	err_unset_ident(const char *arg)
{
	write(2, "minishell: unset: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
}

static void	remove_key(t_expansion **head, const char *key)
{
	t_expansion	*cur;
	t_expansion	*prev;

	prev = NULL;
	cur = *head;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			if (prev)
				prev->next = cur->next;
			else
				*head = cur->next;
			// No need to free key and value as they are GC managed
			// free(cur->key);
			// free(cur->value);
			// free(cur); // This will be cleaned up by GC
			return ;
		}
		prev = cur;
		cur = cur->next;
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
		if (ft_strchr(args[i], '=') || !valid_ident(args[i]))
			ret = (err_unset_ident(args[i]), 1);
		else
			remove_key(head, args[i]);
		i++;
	}
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 05:15:24 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 05:15:47 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include "gc.h"

t_shell_val	*new_shell_val(void)
{
	t_shell_val	*shell_val;

	shell_val = gc_malloc(sizeof(t_shell_val));
	if (!shell_val)
		return (NULL);
	shell_val->last_exit_status = 0;
	shell_val->expansion = NULL;
	return (shell_val);
}

t_expansion	*new_expansion(char *value, char *key)
{
	t_expansion	*expansion;

	expansion = gc_malloc(sizeof(t_expansion));
	if (!expansion)
		return (NULL);
	expansion->value = value;
	expansion->key = key;
	expansion->next = NULL;
	expansion->export = 0;
	expansion->up = 0;
	return (expansion);
}

void	add_expansion_back(t_expansion **head, t_expansion *new)
{
	t_expansion	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	is_valid_var_start(char *s)
{
	if (!s || !s[0])
		return (0);
	if (!s[1])
		return (0);
	if (ft_isspace(s[0]))
		return (0);
	return (1);
}

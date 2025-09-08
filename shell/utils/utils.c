/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:16 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft/libft.h"
#include "minishell.h"
#include "gc.h"

char	*ft_strndup(const char *s, unsigned int n)
{
	return (gc_strndup(s, n));
}

int	ft_isspace(char c)
{
	return (
		c == ' '
		|| c == '\t'
		|| c == '\n'
		|| c == '\v'
		|| c == '\f'
		|| c == '\r'
		|| c == '\\'
		|| c == ';'
	);
}

unsigned int	ft_strlen_dollar(const char *s)
{
	unsigned int	len;

	if (!s)
		return (0);
	len = 0;
	if (s[0] == '$' || s[0] == '?' )
		return (1);
	while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
		len++;
	return (len);
}

char	*ft_strdup_dollar(const char *s)
{
	unsigned int	len;
	char			*dup;

	if (!s)
		return (NULL);
	len = ft_strlen_dollar(s);
	dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}

void	print_command_debug(t_command *cmd)
{
	int		i;

	while (cmd)
	{
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("[%s] ", cmd->args[i]);
			i++;
		}
		cmd = cmd->next;
	}
}

void	free_split(char **arr)
{
	/*
	 * With GC, manual freeing is not needed
	 * This function is kept for compatibility
	 */
	(void)arr;
}
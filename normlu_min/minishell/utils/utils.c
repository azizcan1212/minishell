/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:16 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 07:32:29 by atam             ###   ########.fr       */
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

int	ft_strlen_dollar(const char *s)
{
	int	len;

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 13:40:29 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 15:56:57 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	builtin_export(char **envp)
{
	int		i;
	int		n;
	char	*eq;

	i = 0;
	n = 0;
	while (envp[n])
		n++;
	sort_str_array(envp, n);
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (!eq)
			printf("declare -x '%s'\n", envp[i]);
		else
			printf("declare -x %.*s=\"%s\"\n",
				(int)(eq - envp[i]), envp[i], eq + 1);
		i++;
	}
	return (0);
}

void	sort_str_array(char **arr, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

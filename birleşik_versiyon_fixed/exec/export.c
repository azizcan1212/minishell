/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:18:15 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/06 21:55:16 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"


#include "minishell.h"
#include "libft.h"

int	builtin_export(char **envp)
{
	int		i;
	int		n;
    char    *eq;

	i = 0;
	n = 0;
    while(envp[n])
	n++;
    sort_str_array(envp, n);
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if(!eq)
			printf("declare -x '%s'\n", envp[i]);
		else
			printf("declare -x %.*s=\"%s\"\n", (int)(eq - envp[i]), envp[i], eq + 1);
		i++;
	}
	return (0);
}

void sort_str_array(char **arr, int size)
{
	int	i, j;
	char *tmp;

	for (i = 0; i < size - 1; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

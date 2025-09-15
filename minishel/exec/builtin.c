/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:05:27 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/15 17:41:38 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "gc.h"
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include "libft/libft.h"

static int	echo_parse_flags(char **args, int *newline)
{
	int	i;
	int	j;

	i = 1;
	*newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		*newline = 0;
		i++;
	}
	return (i);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = echo_parse_flags(args, &newline);
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

static int	print_pwd_from_env(void)
{
	char	*cwd;

	cwd = ms_getenv("PWD");
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (print_pwd_from_env());
	gc_add_node(cwd, ft_strlen(cwd + 1));
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free_gc_node();
	return (0);
}

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (!str[i])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (ft_isspace(str[i]))
		{
			while (str[i] && ft_isspace(str[i]))
				i++;
			return (!str[i]);
		}
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

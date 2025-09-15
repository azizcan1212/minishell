/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:40:38 by atam              #+#    #+#             */
/*   Updated: 2025/09/15 17:40:39 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

static char	*get_cd_path(char **args, char *old_pwd, int old_pwd_allocated)
{
	char	*path;

	if (!args[1])
	{
		path = ms_getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			if (old_pwd_allocated)
				free(old_pwd);
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

static int	validate_cd_args(char **args)
{
	if (args[2])
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	return (0);
}

static int	handle_cd_validation_error(char *path, char *old_pwd,
	int old_pwd_allocated)
{
	write(2, "cd: ", 4);
	write(2, path, ft_strlen(path));
	write(2, ": No such file or directory\n", 28);
	if (old_pwd_allocated)
		free(old_pwd);
	return (1);
}

static int	handle_chdir_error(char *old_pwd, int old_pwd_allocated)
{
	perror("cd");
	if (old_pwd_allocated)
		free(old_pwd);
	return (1);
}

int	builtin_cd(char **args)
{
	char	*path;
	char	*old_pwd;
	int		old_pwd_allocated;

	if (validate_cd_args(args))
		return (1);
	old_pwd = getcwd(NULL, 0);
	old_pwd_allocated = (old_pwd != NULL);
	if (!old_pwd)
		old_pwd = ms_getenv("PWD");
	path = get_cd_path(args, old_pwd, old_pwd_allocated);
	if (!path)
		return (1);
	if (!is_directory(path))
		return (handle_cd_validation_error(path, old_pwd, old_pwd_allocated));
	if (chdir(path) == -1)
		return (handle_chdir_error(old_pwd, old_pwd_allocated));
	return (update_env_after_cd(old_pwd, old_pwd_allocated));
}

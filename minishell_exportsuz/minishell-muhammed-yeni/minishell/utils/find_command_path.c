/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:58:29 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/07 17:36:22 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

char	*join_path_dir(const char *dir, const char *cmd)
{
	int		ld;
	int		lc;
	char	*full;

	ld = ft_strlen(dir);
	lc = ft_strlen(cmd);
	full = malloc(ld + lc + 2);
	if (!full)
		return (NULL);
	ft_strcpy(full, dir);
	full[ld] = '/';
	ft_strcpy(full + ld + 1, cmd);
	return (full);
}

char	*find_command_path(const char *cmd, char **envp)
{
	char	*paths;
	char	*res;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = find_path_env(envp);
	if (!paths)
		return (NULL);
	res = try_in_paths(cmd, paths);
	free(paths);
	return (res);
}

char	*find_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_strdup(envp[i] + 5));
		i++;
	}
	return (NULL);
}

char	*try_in_paths(const char *cmd, char *paths)
{
	char	**dirs;
	int		i;
	char	*full;

	dirs = ft_split(paths, ':');
	i = 0;
	while (dirs && dirs[i])
	{
		full = join_path_dir(dirs[i], cmd);
		if (access(full, X_OK) == 0)
		{
			free_split(dirs);
			return (full);
		}
		free(full);
		i++;
	}
	free_split(dirs);
	return (NULL);
}

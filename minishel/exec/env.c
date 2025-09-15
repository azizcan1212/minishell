/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam <atam@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:57:23 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/15 17:41:02 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

int	builtin_env(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	update_env_after_cd(char *old_pwd, int old_pwd_allocated)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd: error retrieving current directory");
		if (old_pwd_allocated)
			free(old_pwd);
		return (1);
	}
	if (old_pwd)
		ms_setenv("OLDPWD", old_pwd, 1);
	if (old_pwd_allocated)
		free(old_pwd);
	ms_setenv("PWD", new_pwd, 1);
	free(new_pwd);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:21:29 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 04:21:55 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pwd_env(void)
{
	char	*pwd;

	if (!getenv("PWD"))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			ms_setenv("PWD", pwd, 1);
			free(pwd);
		}
	}
}

void	setup_shlvl_env(void)
{
	char	*shlvl_env;

	shlvl_env = getenv("SHLVL");
	if (!shlvl_env || !*shlvl_env)
		ms_setenv("SHLVL", "1", 1);
}

void	setup_underscore_env(void)
{
	if (!getenv("_"))
		ms_setenv("_", "/usr/bin/env", 1);
}

void	init_minimal_env_shlvl(void)
{
	setup_pwd_env();
	setup_shlvl_env();
	setup_underscore_env();
}

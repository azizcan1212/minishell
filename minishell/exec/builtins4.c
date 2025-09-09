/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:32:50 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 19:16:29 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_with_msg(const char *pfx,
	const char *arg, const char *sfx, int code)
{
	if (pfx)
		write(2, pfx, ft_strlen(pfx));
	if (arg)
		write(2, arg, ft_strlen(arg));
	if (sfx)
		write(2, sfx, ft_strlen(sfx));
	gc_cleanup();
	exit(code);
}

int	builtin_exit(char **args)
{
	long	result;
	int		exit_code;

	if (!args[1])
	{
		gc_cleanup();
		exit(0);
	}
	if (args[2])
		exit_with_msg("bash: exit: too many arguments\n", NULL, NULL, 1);
	if (!is_numeric(args[1]))
		exit_with_msg("bash: exit: ", args[1],
			": numeric argument required\n", 2);
	result = ft_atoi(args[1]);
	exit_code = (unsigned char)(result % 256);
	if (result < 0)
		exit_code = 256 + (result % 256);
	gc_cleanup();
	exit(exit_code);
}

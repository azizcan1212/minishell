/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:15:24 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/09 19:02:54 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "minishell.h"
#include "gc.h"

static void	put2(const char *s)
{
	size_t	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
		i++;
	(void)!write(2, s, i);
}

void	print_and_exit(const char *name, const char *msg, int code)
{
	(void)!write(2, "minishell: ", 11);
	put2(name);
	put2(msg);
	(void)!write(2, "\n", 1);
	gc_cleanup();
	_exit(code);
}

int	redirect_input(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		return (-1);
	}
	if (dup2(fd, 0) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

int	redirect_output(const char *file, int append)
{
	int	fd;

	if (append)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (dup2(fd, 1) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

void	handle_exec_error(t_command *cur, int err)
{
	if (err == EACCES)
		print_and_exit(cur->cmd, ": Permission denied", 126);
	if (err == ENOENT
		&& (cur->cmd && (cur->cmd[0] == '.' || ft_strchr(cur->cmd, '/'))))
		print_and_exit(cur->cmd, ": No such file or directory", 127);
	print_and_exit(cur->cmd, ": command not found", 127);
}

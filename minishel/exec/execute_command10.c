/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command10.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 19:03:31 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 19:08:42 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"

static int	open_input_gc(const char *file, t_shell_val *val)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		val->last_exit_status = 1;
		return (1);
	}
	fd_gc_add(fd);
	return (0);
}

static int	open_output_gc(const char *file, int append, t_shell_val *val)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		perror(file);
		val->last_exit_status = 1;
		return (1);
	}
	fd_gc_add(fd);
	return (0);
}

static int	handle_file_open(const char *file, int is_input,
				int append, t_shell_val *val)
{
	if (is_input)
		return (open_input_gc(file, val));
	return (open_output_gc(file, append, val));
}

static int	handle_no_redir_list(t_command *h, t_shell_val *val)
{
	if (h->input_file)
	{
		if (handle_file_open(h->input_file, 1, 0, val) != 0)
			return (1);
	}
	if (h->output_file)
	{
		if (handle_file_open(h->output_file, 0, h->append, val) != 0)
			return (1);
	}
	fd_gc_cleanup();
	return (0);
}

int	open_redirs_check(t_command *h, t_shell_val *val)
{
	int	i;
	int	is_input;
	int	append;

	if (h->redir_count <= 0)
		return (handle_no_redir_list(h, val));
	i = 0;
	while (i < h->redir_count)
	{
		is_input = (h->redir_types[i] == 0);
		append = (h->redir_types[i] == 2);
		if (handle_file_open(h->redir_files[i], is_input, append, val) != 0)
			return (1);
		i++;
	}
	fd_gc_cleanup();
	return (0);
}

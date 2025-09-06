/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:43:47 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/06 21:07:48 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_heredoc_lines(int write_fd,
	const char *delim, t_shell_val *val, int expandable_fd)
{
	char	*line;

	(void)val;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			(void)!write(2,
				"warning: here-document delimited by end-of-file (wanted '", 58);
			(void)!write(2, delim, ft_strlen(delim));
			(void)!write(2, "')\n", 3);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, write_fd, val, expandable_fd);
		free(line);
	}
}

static int	
	get_heredoc_fd(const char *delim, t_shell_val *val, int expandable_fd)
{
	int		p[2];
	pid_t	pid;
	int		st;

	if (!delim || pipe(p) == -1)
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		close(p[0]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		read_heredoc_lines(p[1], delim, val, expandable_fd);
		close(p[1]);
		gc_cleanup();
		_exit(0);
	}
	close(p[1]);
	if (pid < 0)
		return (close(p[0]), -1);
	if (waitpid(pid, &st, 0) == -1)
		return (close(p[0]), -1);
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	{
		close(p[0]);
		val->last_exit_status = 130;
		return (-1);
	}
	return (p[0]);
}

int	is_directory(const char *path)
{
	DIR		*dir;

	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}

void	check_directory(t_command *cur, t_shell_val *val)
{
	if (cur->cmd[0] == '.' && cur->cmd[1] == '\0')
	{
		printf("minishell: .: filename argument required\n");
		val->last_exit_status = 2;
		return ;
	}
	if (cur->cmd[0] == '.' && cur->cmd[1] == '.' && cur->cmd[2] == '\0')
	{
		printf ("minishell: %s: Command not found\n", cur->cmd);
		val->last_exit_status = 127;
		return ;
	}
	if (access(cur->cmd, F_OK) != 0)
	{
		printf("minishell: %s: No such file or directory\n", cur->cmd);
		val->last_exit_status = 127;
		return ;
	}
	return ;
}

int	execute_command(t_command *head, char **envp, t_shell_val *val)
{
	t_command	*node;
	int			ret;
	int			fd;

	node = head;
	while (node)
	{
		node->heredoc_fd = -1;
		if (node->heredoc_list)
		{
			t_hdoc *h = node->heredoc_list;
			while (h)
			{
				int fd_tmp = get_heredoc_fd(h->delim, val, h->expandable);
				if (fd_tmp == -1 && val->last_exit_status == 130)
					break;
				if (!h->next)
					node->heredoc_fd = fd_tmp;
				else if (fd_tmp != -1)
					close(fd_tmp);
				h = h->next;
			}
		}
		else if (node->heredoc_delim)
			node->heredoc_fd = get_heredoc_fd(node->heredoc_delim,
											val, node->expandable_fd);
		node = node->next;
	}
	if (val->last_exit_status == 130)
	{
		node = head;
		while (node)
		{
			if (node->heredoc_fd != -1)
			{
				close(node->heredoc_fd);
				node->heredoc_fd = -1;
			}
			node = node->next;
		}
		return (130);
	}
	if (head && !head->next && (!head->cmd || head->cmd[0] == '\0'))
	{
		if (head->input_file)
		{
			fd = open(head->input_file, O_RDONLY);
			if (fd < 0)
				return (perror(head->input_file), val->last_exit_status = 1, 1);
			close(fd);
		}
		if (head->output_file)
		{
			fd = open(head->output_file,
					O_CREAT | O_WRONLY
					| (head->append ? O_APPEND : O_TRUNC), 0644);
			if (fd < 0)
				return (perror(head->output_file),
					val->last_exit_status = 1, 1);
			close(fd);
		}
		if (head->heredoc_fd != -1)
			close(head->heredoc_fd);
		return (val->last_exit_status = 0, 0);
	}
	if (head && !head->next
		&& (ret = handle_builtin_commands(head, val, envp)) != -1)
		return (val->last_exit_status = ret, ret);
	return (fork_and_exec(head, val, envp));
}

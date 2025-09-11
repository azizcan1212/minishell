/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command7.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 19:12:17 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/11 22:33:25 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <unistd.h>
#include "fd_gc.h"

static int	run_empty_redirect_only(t_command *h, t_shell_val *val)
{
	int	r;

	if (h == NULL)
		return (-1);
	if (h->next != NULL)
		return (-1);
	if (h->cmd != NULL && h->cmd[0] != '\0')
		return (-1);
	if (h->input_file == NULL && h->output_file == NULL && h->heredoc_fd == -1)
		return (-1);
	r = open_input_check(h, val);
	if (r != 0)
		return (1);
	r = open_output_check(h, val);
	if (r != 0)
		return (1);
	if (h->heredoc_fd != -1)
		close(h->heredoc_fd);
	val->last_exit_status = 0;
	return (0);
}

static int	maybe_run_parent_builtin(t_command *head,
	t_shell_val *val, char **envp)
{
	int	ret;

	if (head == NULL)
		return (-1);
	if (head->next != NULL)
		return (-1);
	ret = handle_builtin_commands(head, val, envp);
	if (ret != -1)
		val->last_exit_status = ret;
	return (ret);
}


static void	sigpipe_close_all(int sig)
{
	int	i;


	gc_cleanup();
	(void)sig;
	i = 0;
	while (i < 1024)
	{
		close(i);
		i++;
	}
	_exit(141);
}


// Mevcut get_heredoc_fd fonksiyonunu değiştirin
int	get_heredoc_fd(const char *delim, t_shell_val *val, int expandable_fd)
{
	int		p[2];
	pid_t	pid;
	int		st;

	if (delim == NULL) return (-1);
	if (pipe(p) == -1) return (-1);
	
	fd_gc_add(p[0]); // Read end'i takip et
	fd_gc_add(p[1]); // Write end'i takip et
	
	pid = fork();
	if (pid == 0)
		heredoc_child(p, delim, val, expandable_fd);
	
	close(p[1]); // Parent'te write end'i kapat
	
	if (pid < 0) { 
		close(p[0]);
		return (-1); 
	}
	
	if (waitpid(pid, &st, 0) == -1) { 
		close(p[0]);
		return (-1); 
	}

	// Başarılı durumda read fd'yi döndür
	if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
		return (p[0]);

	// SIGINT ve diğer hata durumlarında
	if ((WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	 || (WIFEXITED(st) && WEXITSTATUS(st) == 130))
	{
		val->last_exit_status = 130;
	}

	close(p[0]); // Bu satır kritik - fd leak'i önler
	return (-1);
}

// Mevcut heredoc_child fonksiyonunu değiştirin  
void	heredoc_child(int *p, const char *delim,
				t_shell_val *val, int expandable_fd)
{
	close(p[0]);
	fd_gc_add(p[1]);  // Bu satırı ekleyin
	signal(SIGPIPE, sigpipe_close_all);
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	read_heredoc_lines(p[1], delim, val, expandable_fd);
	if (val != NULL && val->last_exit_status == 130)
	{
		fd_gc_cleanup(); 
		gc_cleanup();
		_exit(130);
	}
	fd_gc_cleanup();  // Buraya ekleyin
	gc_cleanup();
	_exit(0);
}

// Mevcut execute_command fonksiyonunu değiştirin
int	execute_command(t_command *head, char **envp, t_shell_val *val)
{
	int	ret;

	if (prepare_heredocs_all(head, val) != 0)
	{
		close_all_heredocs(head);
		fd_gc_cleanup();
		return (130);
	}
	ret = run_empty_redirect_only(head, val);
	if (ret != -1)
	{
		fd_gc_cleanup();
		return (ret);
	}
	ret = maybe_run_parent_builtin(head, val, envp);
	if (ret != -1)
	{
		fd_gc_cleanup();
		return (ret);
	}
	ret = fork_and_exec(head, val, envp);
	fd_gc_cleanup();
	return (ret);
}

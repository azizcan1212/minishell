/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:02:56 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/13 05:40:41 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "fd_gc.h"

volatile sig_atomic_t	g_signal_num = 0;

void	signal_handler(int signum)
{
	g_signal_num = signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_handler);
}

void	process_pending_signal(t_shell_state *state)
{
	if (g_signal_num == SIGINT)
	{
		state->val->last_exit_status = 128 + SIGINT;
		g_signal_num = 0;
	}
	else if (g_signal_num == SIGQUIT)
	{
		state->val->last_exit_status = 128 + SIGQUIT;
		g_signal_num = 0;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_shell_state	state;
	int				exit_status;

	(void)ac;
	(void)av;
	init_shell_environment(&state);
	main_shell_loop(&state, envp);
	exit_status = 0;
	if (state.val)
		exit_status = state.val->last_exit_status;
	fd_gc_cleanup();
	gc_cleanup();
	rl_clear_history();
	return (exit_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 17:02:56 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/07 17:32:09 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "minishell.h"
#include "gc.h"

volatile sig_atomic_t	g_signal_num = 0;

static int get_shlvl(void)
{
    char *v = getenv("SHLVL");
    if (!v || !*v)
        return 1;
    return ft_atoi(v);
}

void update_shlvl(void)
{
    int lvl = get_shlvl();
    char *next = gc_itoa(lvl + 1);
    if (next)
        setenv("SHLVL", next, 1);
}


void	signal_handler(int signum)
{
	g_signal_num = signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int signum)
{
	g_signal_num = signum;
}

static void	setup_signal_handlers(void)
{
	signal(SIGQUIT, sigquit_handler);
	signal(SIGINT, signal_handler);
}

static void	process_pending_signal(t_shell_state *state)
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

static void init_minimal_env_shlvl(void)
{
	char *pwd;

	if (!getenv("PWD"))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			setenv("PWD", pwd, 1);
			free(pwd);
		}
	}
	/* SHLVL başlangıcını 2 olarak ayarla (env -i) */
	if (!getenv("SHLVL") || !*getenv("SHLVL"))
		setenv("SHLVL", "1", 1);
	if (!getenv("_"))
		setenv("_", "/usr/bin/env", 1);
}

int	main(int ac, char **av, char **envp)
{
	t_shell_state	state;

	(void)ac;
	(void)av;
	init_shell_state(&state);
	init_minimal_env_shlvl();
	update_shlvl();
	setup_signal_handlers();
	while (1)
	{
		process_pending_signal(&state);
		state.input = read_input_with_history();
		process_pending_signal(&state);
		if (!state.input)
			break ;
		if (process_tokens(&state))
		{
			if (process_commands(&state))
			{
				handle_execution(&state, envp);
				cleanup_state(&state);
			}
		}
	}
	gc_cleanup();
	return (0);
}

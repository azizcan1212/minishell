/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:22:20 by atam              #+#    #+#             */
/*   Updated: 2025/09/11 21:15:33 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static int	get_shlvl(void)
{
	char	*v;

	v = getenv("SHLVL");
	if (!v || !*v)
		return (1);
	return (ft_atoi(v));
}

void	update_shlvl(void)
{
	int		lvl;
	char	*next;

	lvl = get_shlvl();
	next = gc_itoa(lvl + 1);
	if (next)
		ms_setenv("SHLVL", next, 1);
}

int	process_input_line(t_shell_state *state, char **envp)
{
	if (!state->input)
		return (0);
	if (!process_tokens(state))
		return (1);
	if (!process_commands(state))
		return (1);
	handle_execution(state, envp);
	return (1);
}

void	load_system_environment(t_shell_val *val, char **envp)
{
	int				i;
	const char		*s;
	t_expansion		*e;
	extern char		**environ;

	if (!envp || !envp[0])
		envp = environ;
	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		s = envp[i];
		e = make_exp_from_str(s);
		if (!e)
			return ;
		e->export = 1;  /* Sistem env değişkenleri export edilmiş */
		add_expansion_back(&val->expansion, e);
		i++;
	}
}

void	init_shell_environment(t_shell_state *state)
{
	extern char **environ;
	
	init_shell_state(state);
	ms_setenv_init(state->val);  /* Custom setenv instance'ını başlat */
	load_system_environment(state->val, environ);  /* Sistem environment'ını yükle */
	init_minimal_env_shlvl();
	update_shlvl();
	setup_signal_handlers();
}

void	main_shell_loop(t_shell_state *state, char **envp)
{
	while (1)
	{
		
		process_pending_signal(state);
		state->input = read_input_with_history();
		process_pending_signal(state);
		signal(SIGINT, SIG_IGN);
		if (!process_input_line(state, envp))
			break ;
		signal(SIGINT, signal_handler);
		fd_gc_cleanup(); // Her döngüde fd cleanup
	}
}

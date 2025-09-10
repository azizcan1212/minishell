/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:22:20 by atam              #+#    #+#             */
/*   Updated: 2025/09/10 06:06:05 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
		setenv("SHLVL", next, 1);
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

void	init_shell_environment(t_shell_state *state)
{
	init_shell_state(state);
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
		if (!process_input_line(state, envp))
			break ;
	}
}

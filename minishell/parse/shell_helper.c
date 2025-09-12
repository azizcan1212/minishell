/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 04:22:20 by atam              #+#    #+#             */
/*   Updated: 2025/09/12 06:35:57 by atam             ###   ########.fr       */
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

void	init_shell_environment(t_shell_state *state)
{
	extern char	**environ;

	init_shell_state(state);
	ms_setenv_init(state->val);
	env_bootstrap_once(state->val, environ);
	init_minimal_env_shlvl();
	update_shlvl();
	setup_signal_handlers();
}

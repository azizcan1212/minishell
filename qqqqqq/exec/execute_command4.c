/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atam < atam@student.42kocaeli.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:44:26 by muharsla          #+#    #+#             */
/*   Updated: 2025/09/12 06:11:50 by atam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "fd_gc.h"

void	child_sigint_handler(int sig)
{
	(void)sig;
	_exit(130);
}

void	exec_child(t_command *cmd, char **envp)
{
    char	*path;

    g_signal_num = 0;
    signal(SIGINT, child_sigint_handler);
    signal(SIGPIPE, sigpipe_close_all);
    signal(SIGQUIT, SIG_DFL);
    child_redirections(cmd);
    /* Run builtins in child (needed for pipelines) */
    if (cmd->cmd)
    {
        if (!ft_strcmp(cmd->cmd, "echo"))
        {
            int st = builtin_echo(cmd->args);
            fd_gc_cleanup();
            gc_cleanup();
            _exit(st);
        }
        if (!ft_strcmp(cmd->cmd, "env"))
        {
            int st = builtin_env(envp);
            fd_gc_cleanup();
            gc_cleanup();
            _exit(st);
        }
        if (!ft_strcmp(cmd->cmd, "export"))
        {
            t_shell_val tmp;
            tmp.last_exit_status = 0;
            tmp.expansion = NULL;
            env_bootstrap_once(&tmp, envp);
            if (!cmd->args[1])
            {
                print_export(tmp.expansion);
                fd_gc_cleanup();
                gc_cleanup();
                _exit(0);
            }
            else
            {
                int st = set_export(cmd->args, &tmp.expansion);
                /* Do not propagate to parent env from child */
                fd_gc_cleanup();
                gc_cleanup();
                _exit(st);
            }
        }
        if (!ft_strcmp(cmd->cmd, "unset"))
        {
            t_shell_val tmp;
            tmp.last_exit_status = 0;
            tmp.expansion = NULL;
            env_bootstrap_once(&tmp, envp);
            int st = builtin_unset(cmd->args, &tmp.expansion);
            fd_gc_cleanup();
            gc_cleanup();
            _exit(st);
        }
        if (!ft_strcmp(cmd->cmd, "cd"))
        {
            int st = builtin_cd(cmd->args);
            fd_gc_cleanup();
            gc_cleanup();
            _exit(st);
        }
    }
    if (cmd->cmd && !ft_strcmp(cmd->cmd, "exit"))
    {
        fd_gc_cleanup();
        builtin_exit(cmd->args, cmd);
    }
	if (cmd->cmd && !ft_strcmp(cmd->cmd, "pwd"))
	{
		fd_gc_cleanup();
		gc_cleanup();
		builtin_pwd_and_exit();
	}
	path = find_command_path(cmd->cmd, envp);
	child_directory(cmd, path, envp);
}

void	ms_child_io_setup(int role, int i, int total, int *fds)
{
	if (total <= 1)
		return ;
	if (role == 0)
	{
		if (dup2(fds[2 * (total - 2)], STDIN_FILENO) == -1)
		{
			gc_cleanup();
			_exit(1);
		}
		return ;
	}
	if (i > 0 && dup2(fds[2 * (i - 1)], STDIN_FILENO) == -1)
	{
		gc_cleanup();
		_exit(1);
	}
	if (dup2(fds[2 * i + 1], STDOUT_FILENO) == -1)
	{
		gc_cleanup();
		_exit(1);
	}
}

int	fork_prev_children(t_command *first,
	int total, int *fds, char **envp)
{
	int			i;
	t_command	*node;
	pid_t		c;

	i = 0;
	node = first;
	while (i < total - 1)
	{
		c = fork();
		if (c == 0)
		{
			ms_child_io_setup(1, i, total, fds);
			if (total > 1)
				close_pipes(fds, 2 * (total - 1));
			exec_child(node, envp);
		}
		else if (c < 0)
			return (-1);
		node = node->next;
		i++;
	}
	return (0);
}

pid_t	spawn_pipeline(t_command *first,
	int total, int *fds, char **envp)
{
	int			i;
	t_command	*node;
	pid_t		pid;

	i = 0;
	node = first;
	while (i++ < total - 1)
		node = node->next;
	pid = fork();
	if (pid == 0)
	{
		ms_child_io_setup(0, 0, total, fds);
		if (total > 1)
			close_pipes(fds, 2 * (total - 1));
		exec_child(node, envp);
	}
	else if (pid < 0)
		return (-1);
	if (fork_prev_children(first, total, fds, envp) < 0)
		return (-1);
	return (pid);
}

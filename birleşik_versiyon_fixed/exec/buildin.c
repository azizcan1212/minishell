#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static int	builtin_echo(char **args)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	// Check for -n option
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	
	if (newline)
		write(1, "\n", 1);
	return (0);
}

static int	builtin_cd(char **args)
{
	char *path;

	if (!args[1])
	{
		// cd without arguments goes to HOME
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else
		path = args[1];
	
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	builtin_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}

static int	builtin_exit(char **args)
{
	(void)args;
	write(1, "exit\n", 5);
	exit(0);
}

int	handle_builtin_commands(t_command *cmd, t_shell_val *val,char **envp)
{
	if (cmd && cmd->cmd && !cmd->next)
	{
		if (!ft_strcmp(cmd->cmd, "cd"))
		{
			val->last_exit_status = builtin_cd(cmd->args);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "exit"))
		{
			builtin_exit(cmd->args);
			return (0);
		}
		if (!ft_strcmp(cmd->cmd, "echo"))
		{
			val->last_exit_status = builtin_echo(cmd->args);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "pwd"))
		{
			val->last_exit_status = builtin_pwd();
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "env"))
		{
			val->last_exit_status = builtin_env(envp);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "export"))
		{
			val->last_exit_status = builtin_export(envp);
			return (val->last_exit_status);
		}
		if (!ft_strcmp(cmd->cmd, "unset"))
		{
			val->last_exit_status = builtin_unset(cmd->args, NULL);
			return (val->last_exit_status);
		}
	}

	return (-1); // Built-in değilse -1 döndür
}
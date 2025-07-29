#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "minishell.h"

volatile sig_atomic_t g_signal_num = 0;// bunu ilerde kullanabiliriz diye yazdım kullanmazsak sileriz

void signal_handler(int signum) 
{
    g_signal_num = signum;
    write(1, "\n", 1);
    rl_on_new_line(); 
    rl_replace_line("",0);
    rl_redisplay();
}

void sigquit_handler(int signum)
{
    g_signal_num = signum;
    // Hiçbir şey yapma (ignore)
}

void manage_signal()
{
    signal(SIGQUIT,sigquit_handler);// ctrl + 
    signal(SIGINT,signal_handler); // ctrl + C
}


int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_command	*cmds;

	(void)ac;
	(void)av;
	while (1)
	{
		input = readline("minishell>> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokens = tokenize(input);
		cmds = parse_tokens_to_commands(tokens);
		if (cmds)
			execute_command(cmds, envp,);
		free(input);
		free_tokens(tokens);
		free_commands(cmds);
	}
	return (0);
}


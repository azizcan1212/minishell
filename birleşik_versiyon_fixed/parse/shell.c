#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "minishell.h"

volatile sig_atomic_t g_signal_num = 0;

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

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    t_shell_state state;

    init_shell_state(&state);
    manage_signal();
    while (1)
    {
        state.input = read_input_with_history();
        if (!state.input)
            break;
        if (process_tokens(&state))
        {
            if (process_commands(&state))
            {
                handle_execution(&state, envp);
                cleanup_state(&state);
            }
        }
    }    
    free_expansion(state.expansion);
    if (state.val)
        free(state.val); // Val'i program sonunda free et
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

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

int main()
{
    char *input;

    manage_signal();
    while (1)
    {
        input = readline("minishell>> ");
        if (!input) // CTRL + D
            break;
        if (*input)
            add_history(input);

        free(input);
    }
    return 0;
}


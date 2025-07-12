#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "../minishell.h"

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

void print_tokens(t_token *head)
{
    int i = 0;
    while (head)
    {
        printf("Token[%d]: \"%s\"\n", i++, head->value);
        head = head->next;
    }
}
int main()
{
    char *input;
    t_token *tokens;

    manage_signal();
    while (1)
    {
        input = readline("minishell>> ");
        if (!input) // CTRL + D
            break;
        if (*input)
            add_history(input);
        tokens = tokenize(input);      // Burada tokenize et
        print_tokens(tokens); // Tokenleri yazdır
        free_tokens(tokens);           // Belleği temizle
        free(input);                   // input’u serbest bırak
    }
    return 0;
}


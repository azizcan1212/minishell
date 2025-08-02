#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "../minishell.h"
#include <sys/types.h>


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
    //int i = 0;
    while (head)
    {
        //("Token[%d]: \"%s\"\n", i++, head->value);
        //printf("Type: %d\n", head->type);
        //printf("space status : %d\n",head ->space_next_status);
        head = head->next;
    }
}


int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    char *input;
    t_token *tokens;
    t_expansıon *expansion = NULL;
    t_shell_val *val;
    t_command	*cmds;

    manage_signal();
    while (1)
    {
        input = readline("minishell>> ");
        if (!input) // CTRL + D
            break;
        if (*input)
            add_history(input);
        tokens = tokenize(input);     // Burada tokenize et
        int equal_status = control_equal(tokens);
        val = new_shell_val();
        parse_one_dollar(tokens,val);
        tokens =merge_token(tokens);
        cmds = parse_tokens_to_commands(tokens);
        set_environment(tokens, &expansion,equal_status);
        execute_command(cmds, envp, val); // Komutları çalıştır
        print_tokens(tokens); // Token'ları yazdır
        free_tokens(tokens);           // Belleği temizle
        free(val);
        free(input);                   // input’u serbest bırak
    }
    free_expansion(expansion);
    return 0;
}


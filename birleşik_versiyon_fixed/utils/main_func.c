
#include "../minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
// Input okuma ve history ekleme
char *read_input_with_history(void)
{
    char *input;
    
    input = readline("minishell>> ");
    if (!input) // CTRL + D
    {
        printf("exit\n");
        return NULL;
    }
    if (*input)
        add_history(input);
    return input;
}

// Memory temizleme fonksiyonu
void cleanup_memory(t_token *tokens, t_shell_val *val, char *input)
{
    (void)val; // Unused parameter warning'ini suppress et
    free_tokens(tokens);
    // Val'i free etme, döngü boyunca kullanılacak
    free(input);
}



// State başlatma
void init_shell_state(t_shell_state *state)
{
    state->input = NULL;
    state->tokens = NULL;
    state->expansion = NULL;
    state->val = new_shell_val(); // Val'i burada oluştur ve döngü boyunca koru
    state->cmds = NULL;
    state->token_check = 0;
    state->env_status = 0;
    state->equal_status = NO_EQUAL;
}

// Token işlemlerini process et
int process_tokens(t_shell_state *state)
{
    state->tokens = tokenize(state->input);
    if (!state->tokens)
    {
        free(state->input);
        return 0;
    }
    control_equal(state->tokens);
    state->equal_status = get_equal_status(state->tokens);
    
    return 1;
}



#include "../minishell.h"

t_token *new_token(char *val) {
    t_token *tok = malloc(sizeof(t_token));
    if (!tok)
        return NULL;
    tok->value = val;
    tok->next = NULL;
    return tok;
}

void add_token_back(t_token **head, t_token *new) {
    t_token *tmp;

    if (!*head) {
        *head = new;
        return;
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

void free_tokens(t_token *head) {
    t_token *tmp;

    while (head) {
        tmp = head;
        head = head->next;
        free(tmp->value);
        free(tmp);
    }
}
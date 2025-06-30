#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct s_token
{
    char *value;
    struct s_token *next;
} t_token;

char *ft_strndup(const char *s, unsigned int n);
int ft_isspace(char c);
t_token *new_token(char *val);
void add_token_back(t_token **head, t_token *new);
void free_tokens(t_token *head);
t_token *tokenize(const char *input);
char *parse_word(const char **s);
char *parse_single_quote(const char **s);
char *parse_double_quote(const char **s);
int is_meta(const char *s, int *len);
void signal_handler(int signum);
void sigquit_handler(int signum);
void manage_signal();
int main();
#endif


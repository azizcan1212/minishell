/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:13 by muharsla          #+#    #+#             */
/*   Updated: 2025/07/11 16:44:15 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

t_token *new_token(char *val) 
{
    t_token *tok = malloc(sizeof(t_token));
    if (!tok)
        return NULL;
    tok->value = val;
    tok->next = NULL;
    tok->prev = NULL;  // Yeni eklenen
    tok->type = WORD;
    tok->space_next_status = 0;
    return tok;
}

void add_token_back(t_token **head, t_token *new_token) 
{
    t_token *tmp;

    if (!*head) 
    {
        *head = new_token;
        new_token->prev = NULL;
        return;
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_token;
    new_token->prev = tmp;  // Yeni eklenen
}

void token_type(const char *index,t_token *token)
{
    int len;

    len = 0;
    if (index[0] == '\'')
        token->type = SINGLE_QUOTE;
    else if (index[0] == '\"')
        token->type = DOUBLE_QUOTE;
    else if (is_meta(index, &len))
        token->type = META;
    else
        token->type = WORD;  // ← Bu satır eksikti!
}



    
void add_token_with_type(t_token **list,char *value,const char *i,const char *p) {
    t_token *new_tok;
    if(value)
    {
        printf("DEBUG: value='%s', i[0]='%c'\n", value, i[0]);  // Debug
        new_tok = new_token(value);
        if (!new_tok)
            return;
        token_type(i, new_tok);  
        if(*(p))
        {
            if(ft_isspace(*(p)))
                new_tok->space_next_status = 1;
        }
        
        // Debug çıktısı
        printf("DEBUG: Token eklendi: '%s', type: %d, space_next: %d\n", 
               new_tok->value, new_tok->type, new_tok->space_next_status);
        
        add_token_back(list, new_tok);               
    }       
}

void free_tokens(t_token *head) {
    t_token *tmp;

    while (head) {
        tmp = head;
        head = head->next;
        if (tmp->value)
            free(tmp->value);
        free(tmp);
    }
}

void free_expansion(t_expansıon *head) {
    t_expansıon *tmp;

    while (head) {
        tmp = head;
        head = head->next;
        if (tmp->value)
            free(tmp->value);
        if (tmp->key)
            free(tmp->key);
        if(tmp)
            free(tmp);
        tmp = NULL;
    }
}


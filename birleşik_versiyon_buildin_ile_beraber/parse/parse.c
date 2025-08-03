/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:01 by muharsla          #+#    #+#             */
/*   Updated: 2025/08/02 15:52:06 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

int is_meta(const char *s, int *len)

{
    static const char *metas[] = {
        "<<", ">>", "&&", "||", "|", "<", ">","=" ,NULL
    };
    int i;
    int len_meta;

    i = 0;
    while(metas[i])
    {
        len_meta = ft_strlen(metas[i]);
        if (ft_strncmp(s, metas[i], len_meta) == 0)
        {
            *len = len_meta;
            return 1;
        }
        i++;
    }
    return 0;
}

char *parse_single_quote(const char **s)
{
    char *result;
    int i;
    int j;

    i = 1; // Skip the opening single quote
    j = 0;
    result = malloc(ft_strlen(*s) + 1);
    if (!result)
        return NULL;
    while ((*s)[i] && (*s)[i] != '\'')
        result[j++] = (*s)[i++];
    result[j] = '\0';
    if((*s)[i] != '\'') // Check for closing single quote
    {
        free(result);
        write(2, "Error: Unmatched single quote\n", 30);
        return NULL; // Hata durumunda NULL dön
    }
    *s += i + 1; // Move past the closing quote
    return result;
}


char *parse_double_quote(const char **s)
{
    char *result;
    int i;
    int j;

    i = 1; // Skip the opening double quote
    j = 0;
    result = malloc(ft_strlen(*s) + 1);
    if (!result)
        return NULL;

    while ((*s)[i] && (*s)[i] != '"')
        result[j++] = (*s)[i++];
    result[j] = '\0';
    if((*s)[i] != '"') 
    {
        free(result);
        write(2, "Error: Unmatched double quote\n", 31);
        return NULL; // Hata durumunda NULL dön
    }

    *s += i + 1; // Move past the closing quote
    return result;
}



char *parse_word(const char **s)
{
    char *result;
    int i;
    int j;
    int meta_len;

    i = 0;
    j = 0;
    result = malloc(ft_strlen(*s) + 1);
    if (!result)
        return NULL;
    while ((*s)[i] && !ft_isspace((*s)[i]) && (*s)[i] != '\'' && 
           (*s)[i] != '"' && !is_meta(*s + i, &meta_len))
    {
        result[j++] = (*s)[i++];
    }
    result[j] = '\0';
    *s += i;
    return result;
}

char *parse_meta(const char **s,int *meta_len)
{
    char *result;

    result = ft_strndup(*s, *meta_len);
    *s += *meta_len;
    return result;
}


char *value(const char **p)  // pointer to pointer al ve char* döndür
{
    int meta_len;

    meta_len = 0;
    if (**p == '\'')
        return parse_single_quote(p);
    else if (**p == '"')
        return parse_double_quote(p);
    else if(is_meta(*p,&meta_len))
        return parse_meta(p,&meta_len);
    else
        return parse_word(p); 
}

t_token *tokenize(const char *input)
{
    t_token *list;
    const char *p;
    char *val;
    const char *index;

    list = NULL;
    p = input;
    while (*p) 
    {
        while (ft_isspace(*p))
            p++;
        if (*p) 
        {
            index = p;
            val = value(&p);
            if (!val)
            {
                free_tokens(list);
                return NULL;
            }
            add_token_with_type(&list, val, index, p);
        }
   }
    return list;
}

t_token *merge_token(t_token *head)
{
    t_token *current = head;
    
    while (current && current->next)
    {
        if (current->space_next_status == 0 && 
            !(current->type == META && current->value[0] != '=') &&
            !(current->next->type == META && current->next->value[0] != '='))
        {
            t_token *next_token = current->next;
            char *merged_value = ft_strjoin(current->value, next_token->value);
            current ->type = WORD;
            free(current->value);
            current->value = merged_value;
            current->space_next_status = next_token->space_next_status;
            current->next = next_token->next;
            if (next_token->next)
                next_token->next->prev = current;
            free(next_token->value);
            free(next_token);
        }
        else
            current = current->next;
    }
    return head;
}


int is_token_invalid_before_equal(t_token *equal_token)
{
    t_token *temp;
    int i;

    temp = equal_token->prev;

    while(temp && temp->space_next_status == 0)
    {
        i = 0;
        while(temp->value[i])
        {
            if(temp->value[i] == '$' || temp->type == SINGLE_QUOTE || temp->type  == DOUBLE_QUOTE)
                return 1;
            i++;
        }
        if(!temp->prev || temp->prev->space_next_status == 0)
        {
            if(ft_isdigit(temp->value[0]))
                return 1;
        }
        temp = temp->prev;
    }
    return 0;
}

int control_equal(t_token *head)
{
    t_token *cur ;

    cur = head;
    while (cur)
    {
        if (cur->value && cur->value[0] == '=')
        {
            // "=" bulunduğu anda, onun öncesindeki token'ları kontrol et
            return is_token_invalid_before_equal(cur);
        }
        cur = cur->next;
    }
    return 0;
}

void delete_dollars(t_token *token)
{
    char *new_value;
    t_token *cur;
    t_token *to_delete;

    cur = token;
    while(cur && cur->next)
    {
        if(cur->value[0] == '$' && cur->value[1] == '\0' && 
            (cur->next->type == SINGLE_QUOTE || cur->next->type == DOUBLE_QUOTE))
        {
            new_value = ft_strdup(cur->next->value);
            free(cur->value);
            cur->value = new_value;
            to_delete = cur->next;
            cur->next = to_delete->next;
            if (to_delete->next)
                to_delete->next->prev = cur;
            free(to_delete->value);
            free(to_delete);
        }
        else
            cur = cur->next;
    }
}









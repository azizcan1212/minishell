/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:01 by muharsla          #+#    #+#             */
/*   Updated: 2025/07/11 16:44:03 by muharsla         ###   ########.fr       */
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
        "<<", ">>", "&&", "||", "|", "<", ">", NULL
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
            exit(EXIT_FAILURE);

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
        exit(EXIT_FAILURE);
    }

    *s += i + 1; // Move past the closing quote
    return result;
}

char *parse_meta(const char **p) 
{
    int len;
    if (is_meta(*p, &len)) {
        char *res = ft_strndup(*p, len);
        *p += len;
        return res;
    }
    return NULL;
}

char *parse_word(const char **s)
{
    char *result;
    int i;
    int j;
    int meta_len;

    i = 0;
    j = 0;
    if (is_meta(*s, &meta_len)) 
    {
        result = ft_strndup(*s, meta_len);
        *s += meta_len;
        return result;
    }
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
t_token *tokenize(const char *input)
{
    t_token *list ;
    const char *p ;
    char *val;

    list = NULL;
    p = input;
    while (*p) 
    {
        while (ft_isspace(*p))
            p++;
        if (*p) 
        {
            if (*p == '\'')
                val = parse_single_quote(&p);
            else if (*p == '"')
                val = parse_double_quote(&p);
            else
                val = parse_word(&p); // Meta karakterleri de işler
            if (val)
                add_token_back(&list, new_token(val));
        }
    }
    return list;
}





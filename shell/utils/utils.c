/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:16 by muharsla          #+#    #+#             */
/*   Updated: 2025/07/11 16:44:18 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft/libft.h"

char *ft_strndup(const char *s, unsigned int n)
{
    unsigned int len;
    char *dup;

    if (!s)
        return NULL;

    len = ft_strlen(s);
    if (len > n)
        len = n;

    dup = (char *)malloc(len + 1);
    if (!dup)
        return NULL;

    ft_strlcpy(dup, s, len + 1);
    return dup;
}

int ft_isspace(char c)
{
    return (
        c == ' '  ||  // space
        c == '\t' ||  // tab
        c == '\n' ||  // newline
        c == '\v' ||  // vertical tab
        c == '\f' ||  // form feed
        c == '\r' ||    // carriage return
        c == '\\' ||  // backslash
        c == ';' // semicolon

    );
}

// Dollar expansion utility functions
unsigned int ft_strlen_dollar(const char *s)
{
    unsigned int len;

    if (!s)
        return 0;
    len = 0;
    
    // Özel durumlar: $ ve ? için
    if (s[0] == '$' || s[0] == '?' )
        return 1;

    while (s[len] && (ft_isalnum(s[len]) || s[len] == '_'))
        len++;
    return len;
}

char *ft_strdup_dollar(const char *s)
{
    unsigned int len;
    char *dup;

    if (!s)
        return NULL;
    len = ft_strlen_dollar(s);
    dup = (char *)malloc(len + 1);
    if (!dup)
        return NULL;
    ft_strlcpy(dup, s, len + 1);
    return dup;
}
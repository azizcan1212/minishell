/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:44:16 by muharsla          #+#    #+#             */
/*   Updated: 2025/07/31 18:10:29 by muharsla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft/libft.h"
#include "minishell.h"

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
        c == ';' // semicolons

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

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	print_command_debug(t_command *cmd)
{
	int	i;

	//printf("=== Command Debug ===\n");
	while (cmd)
	{
		//printf("Command: %s\n", cmd->cmd ? cmd->cmd : "(null)");
		//printf("Args: ");
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("[%s] ", cmd->args[i]);
			i++;
		}
		//printf("\n");
		//printf("Input: %s\n", cmd->input_file ? cmd->input_file : "(none)");
		//printf("Output: %s\n", cmd->output_file ? cmd->output_file : "(none)");
		//printf("Append: %s\n", cmd->append ? "yes" : "no");
		//printf("Heredoc: %s\n", cmd->heredoc_delim ? cmd->heredoc_delim : "(none)");
		//printf("----\n");
		cmd = cmd->next;
	}
}
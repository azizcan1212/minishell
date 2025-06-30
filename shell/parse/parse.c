#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


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

char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
char *strjoin_free(char *s1, const char *s2)
{
    char *result;
    size_t len1;
    size_t len2;

    if (!s1 && !s2)
        return NULL;
    if (!s1)
        return ft_strdup(s2);
    if (!s2)
        return s1;

    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    result = malloc(len1 + len2 + 1);
    if (!result)
        return NULL;

    ft_strcpy(result, s1);
    ft_strcpy(result + len1, s2);
    free(s1);
    return result;
}

char *close_single_quote(char *result, int *len)
{
    char *line ;
    int i ;
    char *part;
    char *joined;

    i = 0;
    line = readline("> "); // yeni satırdan devam et
    if (!line)
        return result;
    while (line[i] && line[i] != '\'')
        i++;
    part = ft_substr(line, 0, i);
    joined = strjoin_free(result, "\n"); // yeni satıra geçildiği için \n ekle
    result = strjoin_free(joined, part);
    free(part);
    if (line[i] == '\'') // kapanma oldu
    {
        free(line);
        *len += i + 1; // tırnağı da atla
        return result;
    }
    free(line);
    *len += i;
    return close_single_quote(result, len); // recursive
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
        result = close_single_quote(result, &i);
    *s += i + 1; // Move past the closing quote
    return result;

}


char *close_double_quote(char *result, int *len)
{
    char *line ;
    int i ;
    char *part;
    char *joined;

    i = 0;
    line = readline("> "); // yeni satırdan devam et
    if (!line)
        return result;
    while (line[i] && line[i] != '"')
        i++;
    part = ft_substr(line, 0, i);
    joined = strjoin_free(result, "\n"); // yeni satıra geçildiği için \n ekle
    result = strjoin_free(joined, part);
    free(part);
    if (line[i] == '"') // kapanma oldu
    {
        free(line);
        *len += i + 1; // tırnağı da atla
        return result;
    }
    free(line);
    *len += i;
    return close_double_quote(result, len); // recursive
}


char *parse_double_quote_one_line(const char **s)
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
        result = close_double_quote(result, &i);

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
                val = parse_double_quote_one_line(&p);
            else
                val = parse_word(&p); // Meta karakterleri de işler
            if (val)
                add_token_back(&list, new_token(val));
        }
    }
    return list;
}





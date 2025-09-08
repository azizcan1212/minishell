#include "minishell.h"
#include "libft.h"

int	is_meta(const char *s, int *len)
{
	int			i;
	int			len_meta;
	const char	**metas;

	metas = def_metas();
	i = 0;
	while (metas[i])
	{
		len_meta = ft_strlen(metas[i]);
		if (ft_strncmp(s, metas[i], len_meta) == 0)
		{
			*len = len_meta;
			return (1);
		}
		i++;
	}
	return (0);
}

char	*parse_single_quote(const char **s)
{
	char	*result;
	int		i;
	int		j;

	i = 1;
	j = 0;
	result = gc_malloc(ft_strlen(*s) + 1);
	if (!result)
		return (NULL);
	while ((*s)[i] && (*s)[i] != '\'')
		result[j++] = (*s)[i++];
	result[j] = '\0';
	if ((*s)[i] != '\'')
	{
		write(2, "Error: Unmatched single quote\n", 30);
		return (NULL);
	}
	*s += i + 1;
	return (result);
}

char	*parse_double_quote(const char **s)
{
	char	*result;
	int		i;
	int		j;

	i = 1;
	j = 0;
	result = gc_malloc(ft_strlen(*s) + 1);
	if (!result)
		return (NULL);
	while ((*s)[i] && (*s)[i] != '"')
		result[j++] = (*s)[i++];
	result[j] = '\0';
	if ((*s)[i] != '"')
	{
		write(2, "Error: Unmatched double quote\n", 31);
		return (NULL);
	}
	*s += i + 1;
	return (result);
}

char	*parse_word(const char **s)
{
	char	*result;
	int		i;
	int		j;
	int		meta_len;

	i = 0;
	j = 0;
	result = gc_malloc(ft_strlen(*s) + 1);
	if (!result)
		return (NULL);
	while ((*s)[i] && !ft_isspace((*s)[i]) && (*s)[i] != '\'' &&
		(*s)[i] != '"' && !is_meta(*s + i, &meta_len))
		result[j++] = (*s)[i++];
	result[j] = '\0';
	*s += i;
	return (result);
}

char	*parse_meta(const char **s, int *meta_len)
{
	char	*result;

	result = gc_strndup(*s, *meta_len);
	*s += *meta_len;
	return (result);
}

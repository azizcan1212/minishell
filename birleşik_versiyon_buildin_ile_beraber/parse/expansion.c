#include "../minishell.h"
#include "../libft/libft.h"
#include  <sys/types.h>


t_shell_val *new_shell_val()
{
	t_shell_val *shell_val;

	shell_val = malloc(sizeof(t_shell_val));
	if (!shell_val)
		return NULL;
	shell_val->last_exit_status = 0;
	return shell_val;
}


t_expansıon *new_expansion(char *value, char *key)
{
	t_expansıon *expansion;

	expansion = malloc(sizeof(t_expansıon));
	if (!expansion)
		return NULL;
	expansion->value = value;
	expansion->key = key;
	expansion->next = NULL;
	return expansion;
}

void add_expansion_back(t_expansıon **head, t_expansıon *new)
{
	t_expansıon *tmp;

	if (!*head) {
		*head = new;
		return;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void set_environment(t_token *token, t_expansıon **expansion,int a) // ** olmalı
{
	t_token *new_token;
	int i;

	new_token = token;
	while(new_token)
	{
		i = 0;
		while(new_token->value[i] && new_token->value[i] != '=')
			i++;
		if(a == 0 ) // '=' bulundu mu?
		{
			set_env(new_token,expansion,&i);
		}
		new_token = new_token->next;
	}
}

void set_env(t_token *new_token, t_expansıon **expansion,int *i)
{
	char *key;
	char *value;
	t_expansıon *ex;
	int end;

	end = ft_strlen(new_token->value) - *i - 1;
	key = ft_substr(new_token->value, 0, *i);
	//if(ft_isdigit(key[0]))
		//return;
	value = ft_substr(new_token->value, *i + 1, end);
	ex = new_expansion(value, key);
	add_expansion_back(expansion, ex);
	setenv(key, value, 1);
}



int is_valid_var_start(char *s)
{
	if (!s || !s[0]) // string boşsa ya da ilk karakter null ise
		return 0;
	if (!s[1]) // s[1] erişilecekse, önce null olup olmadığını kontrol et
		return 0;
	if (ft_isspace(s[0]))
		return 0;
	return 1;
}


char *get_new_value(char *value, int i,t_shell_val *val)
{
	char *new_value;
	char *ex;
	char *key;
	char *temp;

	ex = ft_substr(value, 0, i);
	key = ft_strdup_dollar(value + i + 1);
	new_value = dollar_value(key,val);
	if (!new_value)
		new_value = ft_strdup("");
	i += ft_strlen(key) + 1;
	temp = ft_strjoin(ex, new_value);
	free(ex);
	ex = ft_substr(value, i, ft_strlen(value) - i);
	free(key);
	free(value);
	free(new_value);
	if (!ex)
		return temp;
	new_value = ft_strjoin(temp, ex);
	free(temp);
	free(ex);
	return new_value;
}

char *dollar_value(char *key,t_shell_val *val)
{
	pid_t pid;
	char *temp_value;
	int last_exit_status;

	pid = getpid();
	if(key[0] == '$')
		return ft_itoa(pid);
	else if(key[0] == '?')
	{
		last_exit_status = val->last_exit_status;
		return ft_itoa(last_exit_status);  // ft_itoa ile string'e çevir
	}
	else if(ft_isdigit(key[0]))
		return ft_strdup(&key[1]);
	else
	{
		temp_value = getenv(key);
		return ft_strdup(temp_value);
	}
}

void parse_one_dollar(t_token *token,t_shell_val *val)
{
	t_token *new_token;
	int i;

	new_token = token;
	while (new_token)  // TÜM token'ları dolaş
	{
		i = 0;
		if(new_token->type != SINGLE_QUOTE)  // SINGLE_QUOTE değilse işle
		{
			while(new_token->value[i])
			{
				if(new_token->value[i] == '$' && is_valid_var_start(&new_token->value[i]))
				{
					new_token->value = get_new_value(new_token->value, i, val);
					i = 0;
				}
				else
					i++;
			}
		}
		new_token = new_token->next;  // Sonraki token'a geç
	}
}

char	*heredoc_expand(const char *line, t_shell_val *val)
{
	int		i;
	int		j;
	char	*result;
	int		skip;

	result = malloc(4096);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			skip = handle_dollar(line + i, result, &j, val);
			i += skip;
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (result);
}

int	handle_dollar(const char *line, char *result, int *j, t_shell_val *val)
{
	int		k;
	char	key[128];
	char	*valstr;

	k = 0;
	while (line[k] && (ft_isalnum(line[k]) || line[k] == '_'))
	{
		key[k] = line[k];
		k++;
	}	

	key[k] = '\0';
	valstr = dollar_value(key, val);
	if (valstr)
	{
		k = 0;
		while (valstr[k])
			result[(*j)++] = valstr[k++];
	}
	return (ft_strlen(key));
}





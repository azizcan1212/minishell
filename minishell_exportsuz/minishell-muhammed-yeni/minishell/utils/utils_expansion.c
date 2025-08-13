#include "minishell.h"
#include "libft/libft.h"

t_shell_val	*new_shell_val(void)
{
	t_shell_val	*shell_val;

	shell_val = malloc(sizeof(t_shell_val));
	if (!shell_val)
		return (NULL);
	shell_val->last_exit_status = 0;
	return (shell_val);
}

t_expansion	*new_expansion(char *value, char *key)
{
	t_expansion	*expansion;

	expansion = malloc(sizeof(t_expansion));
	if (!expansion)
		return (NULL);
	expansion->value = value;
	expansion->key = key;
	expansion->next = NULL;
	expansion -> export = 0; // Initialize export status
	return (expansion);
}

void	add_expansion_back(t_expansion **head, t_expansion *new)
{
	t_expansion	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	is_valid_var_start(char *s)
{
	if (!s || !s[0])
		return (0);
	if (!s[1])
		return (0);
	if (ft_isspace(s[0]))
		return (0);
	return (1);
}

void	free_split(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

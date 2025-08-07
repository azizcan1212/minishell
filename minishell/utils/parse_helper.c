#include "minishell.h"
#include "../libft/libft.h"

void	delete_dollars(t_token *token)
{
	char	*new_value;
	t_token	*cur;
	t_token	*to_delete;

	cur = token;
	while (cur && cur->next)
	{
		if (cur->value[0] == '$'
			&& cur->value[1] == '\0'
			&& (cur->next->type == SINGLE_QUOTE
				|| cur->next->type == DOUBLE_QUOTE))
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

int	is_token_invalid_before_equal(t_token *equal_token)
{
	t_token	*temp;
	int		i;

	temp = equal_token->prev;
	while (temp && temp->space_next_status == 0)
	{
		i = 0;
		while (temp->value[i])
		{
			if (temp->value[i] == '$'
				|| temp->type == SINGLE_QUOTE
				|| temp->type == DOUBLE_QUOTE)
				return (1);
			i++;
		}
		if (!temp->prev || temp->prev->space_next_status == 0)
		{
			if (ft_isdigit(temp->value[0]))
				return (1);
		}
		temp = temp->prev;
	}
	return (0);
}

void	control_equal(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur)
	{
		if (cur->value && cur->value[0] == '=')
		{
			if (is_token_invalid_before_equal(cur))
			{
				cur->equal_status = INVALID;
			}
			else
			{
				cur->equal_status = VALID;
			}
		}
		cur = cur->next;
	}
}

t_equal_status	get_equal_status(t_token *head)
{
	t_token	*cur;
	int		valid_equal_found;

	cur = head;
	valid_equal_found = 0;
	while (cur)
	{
		if (cur->equal_status == INVALID)
			return (INVALID);
		else if (cur->equal_status == VALID)
			valid_equal_found = 1;
		cur = cur->next;
	}
	if (valid_equal_found)
		return (VALID);
	return (NO_EQUAL);
}

int	check_equal_status(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur)
	{
		if (cur->equal_status == INVALID)
			return (1);
		cur = cur->next;
	}
	return (0);
}

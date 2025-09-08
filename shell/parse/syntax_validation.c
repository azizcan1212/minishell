#include "minishell.h"
#include "libft.h"

static int	is_redirect_token(t_token *token)
{
	if (!token || !token->value)
		return (0);
	return (token->type == META && (!ft_strcmp(token->value, "<")
			|| !ft_strcmp(token->value, ">") || !ft_strcmp(token->value, ">>")
			|| !ft_strcmp(token->value, "<<")));
}

static int	is_pipe_token(t_token *token)
{
	if (!token || !token->value)
		return (0);
	return (token->type == META && !ft_strcmp(token->value, "|"));
}

static int	check_pipe_syntax(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (cur && is_pipe_token(cur))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		return (0);
	}
	while (cur)
	{
		if (is_pipe_token(cur))
		{
			if (!cur->next || is_pipe_token(cur->next))
			{
				write(2,
					"minishell: syntax error near unexpected token `|'\n", 51);
				return (0);
			}
		}
		cur = cur->next;
	}
	return (1);
}

static int	print_redirect_error(t_token *token)
{
	if (!ft_strcmp(token->value, ">"))
		write(2, "minishell: syntax error near unexpected token `>'\n", 51);
	else if (!ft_strcmp(token->value, "<"))
		write(2, "minishell: syntax error near unexpected token `<'\n", 51);
	else if (!ft_strcmp(token->value, ">>"))
		write(2, "minishell: syntax error near unexpected token `>>'\n", 52);
	else if (!ft_strcmp(token->value, "<<"))
		write(2, "minishell: syntax error near unexpected token `<<'\n", 52);
	else if (!ft_strcmp(token->value, "|"))
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
	return (0);
}

static int	check_redirect_syntax(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if (is_redirect_token(cur))
		{
			if (!cur->next || !cur->next->value)
			{
				write(2,
					"minishell: syntax error near unexpected token `newline'\n",
					57);
				return (0);
			}
			if (is_redirect_token(cur->next) || is_pipe_token(cur->next))
			{
				if (cur->next->value)
					return (print_redirect_error(cur->next));
			}
		}
		cur = cur->next;
	}
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_pipe_syntax(tokens))
		return (0);
	if (!check_redirect_syntax(tokens))
		return (0);
	return (1);
}

#include "minishell.h"
#include "libft/libft.h"

int	check_include_equal(char **str, int *j, int *i)
{
	while (str[*i][*j])
	{
		if (str[*i][*j] == '=')
			return (1);
		(*j)++;
	}
	return (0);
}

int	is_valid_export(char *str)
{
	int	k;

	if (!str || !str[0])
		return (0);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	k = 0;
	while (str[k] && str[k] != '=')
	{
		if (!(ft_isalnum(str[k]) || str[k] == '_'))
			return (0);
		k++;
	}
	return (1);
}

static int	export_manage_equal(char *arg, t_expansion **expansion, int *j)
{
	char		*key;
	char		*value;
	t_expansion	*n_expansion;

	key = ft_substr(arg, 0, *j);
	if (!key)
		return (1);
	value = ft_strdup(arg + *j + 1);
	if (!value)
	{
		free(key);
		return (1);
	}
	n_expansion = new_expansion(value, key);
	if (!n_expansion)
	{
		free(key);
		free(value);
		return (1);
	}
	n_expansion->export = 1;
	add_expansion_back(expansion, n_expansion);
	return (0);
}

int	control_duplicate_export(t_expansion *expansion, char *key)
{
	t_expansion	*current;

	current = expansion;
	while (current)
	{
		if (current->key && !ft_strcmp(current->key, key) && current->export == 1)
			return (1);
		current = current->next;
	}
	return (0);
}

void	equal_dup_export(t_expansion *expansion, char *key, char *value)
{
	t_expansion	*current;

	current = expansion;
	while (current)
	{
		if (current->key && !ft_strcmp(current->key, key) && current->export == 1)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
			{
				perror("minishell: export: memory allocation failed");
				exit(EXIT_FAILURE);
			}
		}
		current = current->next;
	}
}

static int	handle_no_equal(char *arg, t_expansion **expansion)
{
	t_expansion	*n_expansion;
	char		*key;

	key = ft_strdup(arg);
	if (!key)
		return (1);
	if (!control_duplicate_export(*expansion, key))
	{
		n_expansion = new_expansion(NULL, key);
		if (!n_expansion)
		{
			free(key);
			return (1);
		}
		n_expansion->export = 1;
		add_expansion_back(expansion, n_expansion);
	}
	free(key);
	return (0);
}

static int	process_export_equal(char *arg, t_expansion **expansion)
{
	int		j;
	char	*key;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	key = ft_substr(arg, 0, j);
	if (!key)
		return (1);
	if (!control_duplicate_export(*expansion, key))
		export_manage_equal(arg, expansion, &j);
	else
		equal_dup_export(*expansion, key, arg + j + 1);
	free(key);
	return (0);
}

static int	process_export_no_equal(char *arg, t_expansion **expansion)
{
	return (handle_no_equal(arg, expansion));
}

int	set_export(char **arg, t_expansion **expansion)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (!is_valid_export(arg[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				arg[i]);
			return (1);
		}
		if (check_include_equal(arg, &(int){0}, &i))
		{
			if (process_export_equal(arg[i], expansion))
				return (1);
		}
		else
		{
			if (process_export_no_equal(arg[i], expansion))
				return (1);
		}
		i++;
	}
	return (0);
}

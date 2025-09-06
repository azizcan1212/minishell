/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell_gc                                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 00:00:00 by gc                #+#    #+#             */
/*   Updated: 2025/08/30 00:00:00 by gc               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "gc.h"

/* Token types */
typedef enum e_token_type
{
	WORD ,
	META,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}	t_token_type;

/* Equal status */
typedef enum e_equal_status
{
	NO_EQUAL,
	VALID,
	INVALID,
}	t_equal_status;

/* Token structure */
typedef struct s_token
{
	char				*value;
	t_token_type		type;
	t_equal_status		equal_status;
	int					space_next_status;
	int					expandable_fd;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

/* Command structure */
typedef struct s_hdoc
{
    char            *delim;
    int             expandable;
    struct s_hdoc   *next;
}   t_hdoc;

typedef struct s_command
{
	t_hdoc          	*heredoc_list;
	char				*cmd;
	char				**args;
	char				*input_file;
	char				*output_file;
	char				*heredoc_delim;
	int					heredoc_fd;
	int					append;
	int					expandable_fd;
	struct s_command	*next;
}	t_command;

/* Environment expansion structure */
typedef struct s_expansion
{
	char				*key;
	char				*value;
	int					export;
	int					up;
	struct s_expansion	*next;
}	t_expansion;

/* Shell values structure */
typedef struct s_shell_val
{
	int			last_exit_status;
	t_expansion	*expansion;
}	t_shell_val;

/* Shell state structure */
typedef struct s_shell_state
{
	char			*input;
	t_token			*tokens;
	t_expansion		*expansion;
	t_shell_val		*val;
	t_command		*cmds;
	int				token_check;
	int				env_status;
	t_equal_status	equal_status;
}	t_shell_state;

/* Global signal variable */
extern volatile sig_atomic_t	g_signal_num;

/* Built-in commands */
int			builtin_echo(char **args);
int			builtin_cd(char **args);
int			builtin_pwd(void);
int			builtin_exit(char **args);
int			builtin_env(char **envp);
int			builtin_export(char **args, t_expansion **expansion, char **envp);
int			builtin_unset(char **args, t_expansion **head);
int			handle_builtin_commands(t_command *cmd, t_shell_val *val, char **envp);
int			is_parent_builtin(const char *s);
int			exec_builtin_single(t_command *cmd, t_shell_val *val, char **envp);
int			bi_env_single(t_command *cmd, t_shell_val *val, char **envp);

/* Execution functions */
void		try_exec_resolved_path(t_command *cmd, char **envp);
int			cmd_count(t_command *head);
int			wait_and_status(t_shell_val *shell);
int			execute_command(t_command *cmd, char **envp, t_shell_val *val);
int			fork_and_exec(t_command *first_cmd, t_shell_val *shell, char **envp);
void		close_pipes(int *pipes, int count);
void		setup_child_io(int i, int n, int *pipes);
int			redirect_input(const char *file);
int			redirect_output(const char *file, int append);
void		child_directory(t_command *cur, char *f, char **envp);
int			is_directory(const char *path);
void		check_directory(t_command *cur, t_shell_val *val);

/* Heredoc functions */
int			should_stop_heredoc(char *line, const char *delim, int line_number);
void		process_heredoc_line(char *line, int write_fd, t_shell_val *val, int expandable_fd);

/* Standard I/O management */
int			save_std(int *sv_in, int *sv_out);
void		restore_std(int sv_in, int sv_out);
int			apply_in_parent(t_command *cmd);
int			apply_out_parent(t_command *cmd);

/* Path finding */
char		*find_command_path(const char *cmd, char **envp);
char		*find_path_env(char **envp);
char		*try_in_paths(const char *cmd, char *paths);
char		*join_path_dir(const char *dir, const char *cmd);

/* Tokenization and parsing */
t_token		*tokenize(const char *input);
t_token		*merge_token(t_token *head);
t_command	*parse_tokens_to_commands(t_token *tokens);
char		*value(const char **p);
char		*parse_single_quote(const char **s);
char		*parse_double_quote(const char **s);
char		*parse_word(const char **s);
char		*parse_meta(const char **s, int *meta_len);

/* Token management */
t_token		*new_token(char *val);
void		add_token_back(t_token **head, t_token *new_token);
void		token_type(const char *index, t_token *token);
void		add_token_with_type(t_token **list, char *value, const char *i, const char *p);
int			check_tokens_is_null(t_token *token);
void		delete_dollars(t_token *token);
t_token		*remove_empty_tokens(t_token *head);
t_token		*remove_tokens_before_invalid(t_token *head);

/* Command management */
t_command	*new_command(void);
int			token_is_pipe(t_token *tk);
void		handle_redirection(t_token **tk, t_command *cmd);
void		handle_normal_token(t_token *tk, t_command *cmd, int *argc);
void		process_one_token(t_token **tokens, t_command **cur, t_command **head, int *argc);

/* Environment expansion */
int			set_env(t_token *new_token, t_expansion **expansion, int *i);
int			set_environment(t_token *token, t_expansion **expansion, t_equal_status equal_status);
char		*get_new_value(char *value, int i, t_shell_val *val);
char		*dollar_value(char *key, t_shell_val *val);
void		expand_dollar(t_token *token, t_shell_val *val);

/* Expansion utilities */
t_shell_val	*new_shell_val(void);
t_expansion	*new_expansion(char *value, char *key);
void		add_expansion_back(t_expansion **head, t_expansion *new);
int			is_valid_var_start(char *s);

/* Export functions */
int			count_exported_entries(t_expansion *expansion);
char		**create_exported_keys_array(t_expansion *expansion, int count);
void		copy_to_env_end(t_expansion *expansion, char **envp);
void		sort_str_array(char **arr, int size);
void		print_expansion_by_key(t_expansion *expansion, char *key);
void		print_export(t_expansion *expansion);
int			check_include_equal(char **str, int *j, int *i);
int			is_valid_export(char *str);
int			control_duplicate_export(t_expansion *expansion, char *key);
void		equal_dup_export(t_expansion *expansion, char *key, char *value);
int			set_export(char **arg, t_expansion **expansion);

/* Export environment utilities */
void		reset_updated_flag(t_expansion *expansion);
int			update_env_value(char *key, char *value, char **envp);
int			update_dup_key(t_expansion *expansion, char **envp);
int			count_exported_entries_with_equal(t_expansion *expansion);

/* Equal token handling */
int			is_token_invalid_before_equal(t_token *equal_token);
void		control_equal(t_token *head);
t_equal_status	get_equal_status(t_token *head);
int			check_equal_status(t_token *head);

/* Validation */
int			is_valid_identifier(const char *s);
int			is_numeric(const char *str);
int			validate_syntax(t_token *tokens);

/* Meta token handling */
const char	**def_metas(void);
int			is_meta(const char *s, int *len);
int			has_whitespace(const char *str);

/* Utility functions */
char		*ft_strndup(const char *s, unsigned int n);
int			ft_isspace(char c);
unsigned int	ft_strlen_dollar(const char *s);
char		*ft_strdup_dollar(const char *s);
void		print_command_debug(t_command *cmd);
void		free_split(char **arr);

/* Memory management (GC compatible) */
void		free_args(char **args);
void		free_commands(t_command *cmd);
void		free_tokens(t_token *head);
void		free_expansion(t_expansion *head);
void		gc_collect_if_needed(void);

/* Shell state management */
void		init_shell_state(t_shell_state *state);
int			process_tokens(t_shell_state *state);
int			process_commands(t_shell_state *state);
void		handle_execution(t_shell_state *state, char **envp);
void		cleanup_state(t_shell_state *state);
int			is_export_command(t_token *head);

/* Input/Output */
char		*read_input_with_history(void);
void		cleanup_memory(t_token *tokens, t_shell_val *val, char *input);
void		print_tokens(t_token *tokens);

/* Signal handling */
void		signal_handler(int signum);
void		sigquit_handler(int signum);
void		manage_signal(t_shell_state *state);

#endif

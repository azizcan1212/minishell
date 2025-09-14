/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muharsla <muharsla@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 06:47:28 by atam              #+#    #+#             */
/*   Updated: 2025/09/13 05:43:27 by muharsla         ###   ########.fr       */
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
# include <errno.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "gc.h"

typedef enum e_token_type
{
	WORD,
	META,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}	t_token_type;

typedef enum e_equal_status
{
	NO_EQUAL,
	VALID,
	INVALID,
}	t_equal;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	t_equal				equal_status;
	int					space_next_status;
	int					expandable_fd;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

typedef struct s_hdoc
{
	char			*delim;
	int				expandable;
	struct s_hdoc	*next;
}	t_hdoc;

typedef struct s_command
{
	t_hdoc				*heredoc_list;
	char				*cmd;
	char				**args;
	char				*input_file;
	char				*output_file;
	char				**redir_files;
	int					*redir_types;
	int					redir_count;
	char				*heredoc_delim;
	int					heredoc_fd;
	int					append;
	int					expandable_fd;
	struct s_command	*next;
}	t_command;

typedef struct s_expansion
{
	char				*key;
	char				*value;
	int					export;
	int					up;
	struct s_expansion	*next;
}	t_expansion;

typedef struct s_shell_val
{
	int			last_exit_status;
	t_expansion	*expansion;
}	t_shell_val;

typedef struct s_shell_state
{
	char			*input;
	t_token			*tokens;
	t_expansion		*expansion;
	t_shell_val		*val;
	t_command		*cmds;
	int				token_check;
	int				env_status;
	t_equal			equal_status;
}	t_shell_state;

extern volatile sig_atomic_t	g_signal_num;

typedef struct s_split_ctx
{
	const char	*str;
	int			i;
	int			start;
	int			orig_space_next;
	t_token		*after;
	t_token		*last;
	int			wrote_first;
	t_token		*tok;
}	t_split_ctx;

void		signal_handler(int signum);
void		child_sigint_handler(int sig);
void		update_shlvl(void);
t_token		*split_unquoted_ws_tokens(t_token *head);
void		split_token_in_place(t_token *tok);
int			builtin_echo(char **args);
int			builtin_cd(char **args);
int			builtin_pwd(void);
int			builtin_exit(char **args, t_command *cmd);
int			builtin_exit_parent(char **args, t_command *cmd, t_shell_val *val);
int			builtin_env(char **envp);
int			builtin_export(char **args, t_expansion **expansion, char **envp);
int			builtin_unset(char **args, t_expansion **head);
int			handle_builtin_commands(t_command *cmd, t_shell_val *val,
				char **envp);
int			is_parent_builtin(const char *s);
int			exec_builtin_single(t_command *cmd, t_shell_val *val,
				char **envp);
int			bi_env_single(t_command *cmd, t_shell_val *val, char **envp);
void		env_bootstrap_once(t_shell_val *val, char **envp);
t_expansion	*make_exp_from_str(const char *s);

int			execute_command(t_command *cmd, char **envp, t_shell_val *val);
int			fork_and_exec(t_command *first_cmd, t_shell_val *shell,
				char **envp);
void		close_pipes(int *pipes, int count);
int			redirect_input(const char *file);
int			redirect_output(const char *file, int append);
void		child_directory(t_command *cur, char *f, char **envp);
int			is_directory(const char *path);
void		child_redirections(t_command *cmd);

void		child_setup_signals_and_redirs(t_command *cmd);
void		child_try_run_echo_env(t_command *cmd, char **envp);
void		child_try_run_export(t_command *cmd, char **envp);
void		child_try_run_unset_or_cd(t_command *cmd, char **envp);
void		child_finish_with_exit_pwd_or_exec(t_command *cmd, char **envp);
int			build_pipes(int total, int **fds);
int			count_cmds(t_command *n);
void		builtin_pwd_and_exit(void);
void		check_direct_call(t_command *cur);
void		print_and_exit(const char *name, const char *msg, int code);
void		handle_exec_error(t_command *cur, int err);
int			get_heredoc_fd(const char *delim, t_shell_val *val,
				int expandable_fd);
void		close_all_heredocs(t_command *head);
int			prepare_heredocs_all(t_command *head, t_shell_val *val);
int			open_redirs_check(t_command *h, t_shell_val *val);
int			handle_node_heredoc(t_command *node, t_shell_val *val);
void		heredoc_child(int *p, const char *delim, t_shell_val *val,
				int expandable_fd);
void		heredoc_sigint(int sig);
void		read_heredoc_lines(int write_fd, const char *delim,
				t_shell_val *val, int expandable_fd);
void		process_heredoc_line(char *line, int write_fd, t_shell_val *val,
				int expandable_fd);

int			save_std(int *sv_in, int *sv_out);
void		restore_std(int sv_in, int sv_out);
int			apply_in_parent(t_command *cmd);

char		*find_command_path(const char *cmd, char **envp);
char		*find_path_env(char **envp);
char		*try_in_paths(const char *cmd, char *paths);
char		*join_path_dir(const char *dir, const char *cmd);

t_token		*tokenize(const char *input);
t_token		*merge_token(t_token *head);
t_command	*parse_tokens_to_commands(t_token *tokens);
char		*value(const char **p);
char		*parse_single_quote(const char **s);
char		*parse_double_quote(const char **s);
char		*parse_word(const char **s);
char		*parse_meta(const char **s, int *meta_len);

t_token		*new_token(char *val);
void		add_token_back(t_token **head, t_token *new_token);
void		token_type(const char *index, t_token *token);
void		add_token_with_type(t_token **list, char *value, const char *i,
				const char *p);
int			check_tokens_is_null(t_token *token);
void		delete_dollars(t_token *token);
t_token		*remove_empty_tokens(t_token *head);
t_token		*remove_tokens_before_invalid(t_token *head);

t_command	*new_command(void);
int			token_is_pipe(t_token *tk);
void		handle_redirection(t_token **tk, t_command *cmd);
void		handle_normal_token(t_token *tk, t_command *cmd, int *argc);
void		process_one_token(t_token **tokens, t_command **cur,
				t_command **head, int *argc);
void		handle_pipe_token(t_command **cur, int *argc);
int			is_redirection_token(t_token *token);
void		handle_redirection_with_touch(t_token **tokens, t_command *cur);
void		handle_input_redirection(t_token *next, t_command *cmd);
void		handle_output_redirection(t_token *tk, t_token *next,
				t_command *cmd);
void		handle_heredoc_redirection(t_token *next, t_command *cmd);
void		add_heredoc_to_list(t_command *cmd, t_hdoc *new_node);

int			set_env(t_token *new_token, t_expansion **expansion, int *i);
int			set_environment(t_token *token, t_expansion **expansion,
				t_equal equal_status);
char		*get_new_value(char *value, int i, t_shell_val *val);
char		*dollar_value(char *key, t_shell_val *val);
void		expand_dollar(t_token *token, t_shell_val *val);

t_shell_val	*new_shell_val(void);
t_expansion	*new_expansion(char *value, char *key);
void		add_expansion_back(t_expansion **head, t_expansion *new);
int			is_valid_var_start(char *s);

int			count_new_entries(t_expansion *expansion);
void		add_new_entries_to_env_end(t_expansion *expansion, char **envp);
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
int			export_manage_equal(char *arg, t_expansion **expansion, int *j);
int			process_export_equal(char *arg, t_expansion **expansion);
int			process_export_no_equal(char *arg, t_expansion **expansion);
void		print_invalid_identifier(char *s);

void		reset_updated_flag(t_expansion *expansion);
int			update_env_value(char *key, char *value, char **envp);
int			update_dup_key(t_expansion *expansion, char **envp);
int			count_exported_entries_with_equal(t_expansion *expansion);
char		*create_env_entry(char *key, char *value);
int			key_matches(char *entry, char *key);

int			is_token_invalid_before_equal(t_token *equal_token);
void		control_equal(t_token *head);
int			is_valid_identifier(const char *s);
int			is_numeric(const char *str);
int			validate_syntax(t_token *tokens);

const char	**def_metas(void);
int			is_meta(const char *s, int *len);
int			has_whitespace(const char *str);

int			ft_isspace(char c);
char		*ft_strdup_dollar(const char *s);

void		init_shell_state(t_shell_state *state);
int			process_tokens(t_shell_state *state);
int			process_commands(t_shell_state *state);
void		handle_execution(t_shell_state *state, char **envp);
int			is_export_command(t_token *head);

char		*read_input_with_history(void);

int			process_input_line(t_shell_state *state, char **envp);
void		setup_pwd_env(void);
void		setup_shlvl_env(void);
void		setup_underscore_env(void);
void		init_minimal_env_shlvl(void);
void		init_shell_environment(t_shell_state *state);
void		main_shell_loop(t_shell_state *state, char **envp);
void		process_pending_signal(t_shell_state *state);
void		setup_signal_handlers(void);
void		init_command_fields(t_command *cmd);
int			should_touch_file(t_command *cmd);
void		touch_output_file(char *filename, int append);
void		init_current_command(t_command **cur, t_command **head,
				int *argc);
int			is_redirect_token(t_token *token);

void		ms_setenv_init(t_shell_val *shell_val);
int			ms_setenv(const char *name, const char *value, int overwrite);
int			ms_set_var(const char *name, const char *value, int overwrite);
char		*ms_getenv(const char *name);
char		**build_envp_from_expansion(t_expansion *expansion);

t_expansion	*find_env_var(t_expansion *expansion, const char *name);
int			update_existing_var(t_expansion *var, const char *value,
				int overwrite, int export_flag);
int			create_new_var(t_shell_val *shell, const char *name,
				const char *value, int export_flag);
void		sigpipe_close_all(int sig);
void		ms_child_io_setup(int role, int i, int total, int *fds);
int			fork_prev_children(t_command *first, int total, int *fds,
				char **envp);
pid_t		spawn_pipeline(t_command *first, int total, int *fds,
				char **envp);
void		handle_path_execution(t_command *cur, char *f, char **envp);
t_equal		get_equal_status(t_token *head);
int			ft_strlen_dollar(const char *s);

#endif

#ifndef MINISHELL_H
# define MINISHELL_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> 


typedef enum e_token_type
{
	WORD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	META,
} t_token_type;

typedef enum e_equal_status
{
	VALID,
	INVALID,
	NO_EQUAL
} t_equal_status;

typedef struct s_token
{
	char *value;
	struct s_token *next;
	struct s_token *prev;  // Yeni eklenen
	t_token_type type;
	t_equal_status equal_status;
	int space_next_status;
} t_token;

typedef struct s_expansion
{
	char *value;
	char *key;
	int export; // Yeni eklenen: export durumu
	// export durumu 0 ise normal, 1 ise export edilmiş
	int up; // Yeni eklenen: env güncellenme durumu
	struct s_expansion *next;
} t_expansion;

typedef  struct  s_shell_val
{
	int last_exit_status;
	t_expansion *expansion;

}t_shell_val;

typedef struct s_command
{
	char    *cmd;
	char    **args;
	char    *input_file;
	char	*output_file;
	int		append;
	char	*heredoc_delim;
	int		heredoc_fd;
	struct s_command *next;
}   t_command;

// Shell state struct Maindeki değişkenleri içeriyor maini kısaltmak için kullanılıyor
typedef struct s_shell_state
{
    char *input;
    t_token *tokens;
    t_expansion *expansion;
    t_shell_val *val;
    t_command *cmds;
    int token_check;
    int env_status;
	char *envp[4096];
    int   env_count;
    t_equal_status equal_status;
} t_shell_state;



void	print_command(const t_command *c);
void	print_command_list(const t_command *c);
int	bi_env_single(t_command *cmd, t_shell_val *val, char **envp);
int	is_parent_builtin(const char *s);






int	builtin_echo(char **args);
int	builtin_cd(char **args);
int	builtin_pwd(void);
int	is_numeric(const char *str);
int	builtin_exit(char **args);

int	save_std(int *sv_in, int *sv_out);
void	restore_std(int sv_in, int sv_out);
int	apply_in_parent(t_command *cmd);
int	apply_out_parent(t_command *cmd);
int	exec_builtin_single(t_command *cmd, t_shell_val *val, char **envp);


t_command	*parse_tokens_to_commands(t_token *tokens);
t_command	*new_command(void);
void		free_commands(t_command *cmd);
void		free_args(char **args);
int			execute_command(t_command *cmd, char **envp,t_shell_val *val);
int			token_is_pipe(t_token *tk);
void		handle_redirection(t_token **tk, t_command *cmd);
void		handle_normal_token(t_token *tk, t_command *cmd, int *argc);
void		process_one_token(t_token **tokens, t_command **cur, t_command **head, int *argc);
char		*ft_strndup(const char *s, unsigned int n);
int			ft_isspace(char c);
t_token		*new_token(char *val);
void		add_token_back(t_token **head, t_token *new);
void 		free_tokens(t_token *head);
t_token		*tokenize(const char *input);
char 		*parse_word(const char **s);
char 		*parse_single_quote(const char **s);
char 		*parse_double_quote(const char **s);
int			is_meta(const char *s, int *len);
void		signal_handler(int signum);
void		sigquit_handler(int signum);
void		manage_signal();
void		add_token_with_type(t_token **list, char *value, const char *i,const char *p);
void		token_type(const char *index, t_token *token);
char 	*	ft_strdup_dollar(const char *s);
unsigned int	ft_strlen_dollar(const char *s);
int			set_environment(t_token *token, t_expansion **expansion, t_equal_status equal_status);
int         set_env(t_token *new_token, t_expansion **expansion,int *i);
void        expand_dollar(t_token *token,t_shell_val *val);
char 		*dollar_value(char *key,t_shell_val *val);
t_shell_val	*new_shell_val();
void 		free_expansion(t_expansion *head);
t_token 	*merge_token(t_token *head);
t_token 	*merge_equal(t_token *head);
t_token 	*merge_before_equal(t_token *equal_token);
void		 control_equal(t_token *head);
t_equal_status	 get_equal_status(t_token *head);
void		free_split(char **arr);
char		*find_path_env(char **envp);
char		*try_in_paths(const char *cmd, char *paths);
char		*join_path_dir(const char *dir, const char *cmd);
char		*find_command_path(const char *cmd, char **envp);
void		print_command_debug(t_command *cmd);
void		delete_dollars(t_token *token);
int			handle_builtin_commands(t_command *cmd, t_shell_val *val, char **envp);
int			redirect_input(const char *file);
int			redirect_output(const char *file, int append);
int			validate_syntax(t_token *tokens);
int			is_valid_var_start(char *str);
void 		add_expansion_back(t_expansion **head, t_expansion *new);
void		print_tokens(t_token *tokens);
t_expansion *new_expansion(char *value, char *key);
char 		*parse_meta(const char **s,int *meta_len);
int			check_tokens_is_null(t_token *token);
int			check_equal_status(t_token *token);
t_token		*remove_tokens_before_invalid(t_token *head);
void		init_shell_state(t_shell_state *state);
int			process_tokens(t_shell_state *state);
void		handle_execution(t_shell_state *state, char **envp);
void		cleanup_state(t_shell_state *state);
char	    *read_input_with_history(void);
void		cleanup_memory(t_token *tokens, t_shell_val *val, char *input);
int 		process_commands(t_shell_state *state);
int			builtin_env(char **envp);
int			builtin_export(char **args, t_expansion **expansion, char **envp);
int			count_expansion_entries(t_expansion *expansion);
char		**create_keys_array(t_expansion *expansion, int count);
void		print_expansion_by_key(t_expansion *expansion, char *key);
void		print_sorted_expansion(t_expansion *expansion);
int 		is_valid_identifier(const char *str);
void		sort_str_array(char **arr, int size);
int			builtin_unset(char **args, t_expansion **env);
char 		*get_new_value(char *value, int i, t_shell_val *val);
void		close_pipes(int *pipes, int count);
int			is_directory(const char *path);
void		fork_and_exec(t_command *cmd, char **envp, int *pipes, pid_t *pids, int n);
void		process_heredoc_line(char *line, int write_fd, t_shell_val *val);
int			should_stop_heredoc(char *line, const char *delim, int line_number);
void		setup_child_io(int i, int n, int *pipes);
void		unset_env(const char *key, t_expansion **env);
int 		has_whitespace(const char *str);
const char	**def_metas(void);
int		set_export(char **arg, t_expansion **expansion);
int		update_dup_key(t_expansion *expansion, char **envp);
void	reset_updated_flag(t_expansion *expansion);
void	child_directory(t_command *cur, char *f, char **envp);

#endif


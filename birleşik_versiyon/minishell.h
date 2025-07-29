#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>




typedef enum e_token_type
{
	WORD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	META,
} t_token_type;
typedef struct s_token
{
    char *value;
    struct s_token *next;
    struct s_token *prev;  // Yeni eklenen
    t_token_type type;
    int space_next_status;
} t_token;

typedef struct s_expansıon
{
	char *value;
	char *key;
	struct s_expansıon *next;
} t_expansıon;

typedef  struct  s_shell_val
{
	int last_exit_status;
	t_expansıon expansıon;

}t_shell_val;

typedef struct s_command
{
	char    *cmd;
	char    **args;
	char    *input_file;
	char	*output_file;
	int		append;
	char	*heredoc_delim;
	struct s_command *next;
}   t_command;


t_command	*parse_tokens_to_commands(t_token *tokens);
t_command	*new_command(void);
void		free_commands(t_command *cmd);
void		free_args(char **args);
int			redirect_input(const char *file);
int			redirect_output(const char *file, int append);
int			setup_heredoc(const char *delimiter);
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
void		parse_dollar(t_token *token);
void		set_environment(t_token *token, t_expansıon **expansion,int a);
void        parse_one_dollar(t_token *token,t_shell_val *val);
char 		*dollar_value(char *key,t_shell_val *val);
t_shell_val	*new_shell_val();
void		set_env(t_token *new_token, t_expansıon **expansion,int *i);
void 		free_expansion(t_expansıon *head);
t_token 	*merge_token(t_token *head);
t_token 	*merge_equal(t_token *head);
t_token 	*merge_before_equal(t_token *equal_token);
int 		control_equal(t_token *head);
void		free_split(char **arr);
char		*find_path_env(char **envp);
char		*try_in_paths(const char *cmd, char *paths);
char		*join_path_dir(const char *dir, const char *cmd);
char		*find_command_path(const char *cmd, char **envp);
void		print_command_debug(t_command *cmd);
int			main();

#endif


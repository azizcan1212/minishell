#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct s_token
{
	char *value;
	struct s_token *next;
} t_token;

typedef struct s_command
{
	char    *cmd;
	char    **args;
	char    *input_file;
	char	*output_file;
	int		append;
	char	*heredoc_delim;;
	struct s_command *next;
}   t_command;


t_command	*parse_tokens_to_commands(t_token *tokens);
t_command	*new_command(void);
void		free_commands(t_command *cmd);
void		free_args(char **args);
int			redirect_input(const char *file);
int			redirect_output(const char *file, int append);
int			setup_heredoc(const char *delimiter);
int			execute_command(t_command *cmd, char **envp);
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
int			main();

#endif


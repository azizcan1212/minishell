# Minishell Makefile

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Ilibft -Iparse -I.
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
HEADERS = minishell.h

SRCS = \
	parse/parse.c \
	parse/shell.c \
	parse/parse_helper.c \
	parse/parse_helper_two.c \
	parse/parse_helper_three.c \
	parse/parse_helper_four.c \
	parse/shell_helper.c \
	parse/env_variable.c \
	parse/parse_to_command.c \
	parse/expansion.c \
	parse/expansion_env.c \
	parse/token.c \
	parse/syntax_validation.c \
	utils/utils.c \
	utils/token.c \
	utils/gc.c \
	utils/gc_two.c \
	utils/gc_three.c \
	utils/gc_four.c \
	utils/gc_five.c \
	utils/gc_six.c \
	utils/gc_last.c \
	utils/fd_gc.c \
	utils/parse_helper.c \
	utils/main_func.c \
	utils/main_func_two.c \
	utils/utils_expansion.c \
	utils/utils_export.c \
	utils/utils_export_env.c \
	utils/utils_export_env_helpers.c \
	utils/utils_export_two.c \
	utils/utils_export_two_helpers.c \
	utils/ms_env.c \
	utils/ms_env_utils.c \
	utils/ms_env_build.c \
	utils/find_command_path.c \
	exec/execute_command.c \
	exec/execute_command2.c \
	exec/execute_command3.c \
	exec/execute_command4.c \
	exec/execute_command5.c \
	exec/execute_command6.c \
	exec/execute_command7.c \
	exec/execute_command8.c \
	exec/execute_command9.c \
	exec/execute_command10.c \
	exec/execute_command11.c \
	exec/execute_command12.c \
	exec/builtin.c \
	exec/builtins.c \
	exec/builtins2.c \
	exec/builtins3.c \
	exec/builtins4.c \
	exec/builtins_utils.c \
	exec/env.c \
	exec/unset.c \
	exec/export.c \
	exec/export2.c \
	parse/def_token.c \

	

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT) $(HEADERS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

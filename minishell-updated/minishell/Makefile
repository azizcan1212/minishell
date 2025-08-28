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
	parse/parse_to_command.c \
	parse/expansion.c \
	parse/token.c \
	parse/syntax_validation.c \
	utils/utils.c \
	utils/free.c \
	utils/token.c \
	utils/parse_helper.c \
	utils/main_func.c \
	utils/main_func_two.c \
	utils/utils_expansion.c \
	utils/utils_export.c \
	utils/utils_export_env.c \
	utils/find_command_path.c \
	exec/execute_command.c \
	exec/execute_command2.c \
	exec/execute_command3.c \
	exec/builtin.c \
	exec/builtins.c \
	exec/env.c \
	exec/unset.c \
	exec/export.c \
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

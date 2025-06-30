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
	utils/utils.c \
	utils/token.c

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

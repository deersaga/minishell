NAME = minishell

CC = gcc
SRCS =  built-in/cd.c\
		built-in/echo.c\
		built-in/export.c\
		built-in/pwd.c\
		built-in/unset.c\
		env/env.c\
		parser/expander.c\
		parser/parser.c\
		parser/syntax.c\
		parser/tokenizer.c\
		minishell.c

OBJS = $(SRCS:%.c=%.o)

LIB = ./libftx/libft.a
LIB_DIR = ./libftx
CC = gcc
CFLAGS = -I $(shell brew --prefix readline)/include
LDFLAGS = -lreadline -lhistory -L$(shell brew --prefix readline)/lib

%.o: %.c
	@$(CC) -g -Wall -Wextra -Werror -c $< -o $@

$(NAME): $(OBJS)
	@$(MAKE) -C $(LIB_DIR)
	$(CC)  $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIB) -o $(NAME)

clean:
	$(MAKE) clean -C $(LIB_DIR)
	$(RM) $(OBJS)

fclean:
	$(MAKE) fclean -C $(LIB_DIR)
	$(RM) $(NAME)
	$(RM) $(OBJS)

all : $(NAME)

bonus : $(NAME)

re: fclean all

.PHONY: all  clean fclean re bonus
NAME = minishell

CC = gcc
SRCS = minishell.c \
env/env.c \
parser/parser.c \
parser/tokenizer.c \
parser/expander.c  \
argv.c \
built-in/cd.c \
built-in/echo.c \
built-in/export.c \
built-in/unset.c \
built-in/pwd.c \
path.c \
parser/syntax.c \
signal_handle/signal_handle.c \
executer/executer.c \
executer/heredoc.c \
built-in/env.c \
built-in/exit.c \
file/open_file.c \
executer/executer_deer.c

OBJS = $(SRCS:%.c=%.o)

LIB = ./libftx/libft.a
LIB_DIR = ./libftx
CC = gcc
CFLAGS = -Wall -Wextra -Werror
#IFLAGS = -I /usr/local/opt/readline/include
IFLAGS = -I $(shell brew --prefix readline)/include
LDFLAGS = -lreadline -lhistory -L $(shell brew --prefix readline)/lib
#LDFLAGS = -lreadline -lhistory -L $(shell brew --prefix readline)/lib

#IFLAGS = -I 
%.o: %.c
	@$(CC) $(IFLAGS) -g -c $< -o $@
#	@$(CC) $(CFLAGS) -g -c $< -o $@

$(NAME): $(OBJS)
	@$(MAKE) -C $(LIB_DIR)
	$(CC) $(IFLAGS) $(LDFLAGS) $(OBJS) $(LIB)  -o $(NAME)
#	$(CC) $(IFLAGS)  $(LDFLAGS)  $(OBJS) $(LIB) -o $(NAME)
#	$(CC) $(IFLAGS) -lreadline  $(OBJS) $(LIB) -o $(NAME)
#	$(CC)  $(LDFLAGS) $(IFLAGS) $(OBJS) $(LIB) -o $(NAME)
#	$(CC)  $(IFLAGS) $(LDFLAGS) $(OBJS) $(LIB) -o $(NAME)

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
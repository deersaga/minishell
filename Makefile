NAME = minishell

CC = gcc
<<<<<<< HEAD
SRCS =  built-in/cd.c\
		built-in/echo.c\
		built-in/export.c\
		built-in/pwd.c\
		built-in/unset.c\
		built-in/env.c\
		built-in/exit.c\
		environ/environ.c\
		environ/environ_utils.c\
		environ/environ_for_env.c\
		parser/expander.c\
		parser/parser.c\
		parser/syntax.c\
		parser/redirect.c\
		parser/tokenizer.c\
		parser/token_concat.c\
		parser/token_ctrl.c\
		parser/token_type.c\
		parser/token_utils.c\
		parser/print.c\
		parser/free.c\
		executer/executer.c\
		executer/executer_deer.c\
		executer/heredoc.c \
		file/open_file.c\
		argv.c\
		path.c\
		signal.c\
		minishell.c
OBJ_DIR = ./obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
=======
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
executer/executer.c \
built-in/env.c \
built-in/exit.c \
file/open_file.c \
executer/executer_deer.c

OBJS = $(SRCS:%.c=%.o)

>>>>>>> a03cb3b6e4b5001b542d8eca4c07d04411d8276b
LIB = ./libftx/libft.a
LIB_DIR = ./libftx
CC = gcc
CFLAGS = -Wall -Wextra -Werror
#IFLAGS = -I /usr/local/opt/readline/include
IFLAGS = -I $(shell brew --prefix readline)/include
LDFLAGS = -lreadline -lhistory -L $(shell brew --prefix readline)/lib
#LDFLAGS = -lreadline -lhistory -L $(shell brew --prefix readline)/lib

<<<<<<< HEAD
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@);
	@$(CC) -g -Wall -Wextra -Werror $(IFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(MAKE) -C $(LIB_DIR)
	$(CC) $(IFLAGS) $(LDFLAGS) $(OBJS) $(LIB)  -o $(NAME)
=======
#IFLAGS = -I 
%.o: %.c
	@$(CC) $(IFLAGS) -g -c $< -o $@
#	@$(CC) $(CFLAGS) -g -c $< -o $@

$(NAME): $(OBJS)
	@$(MAKE) -C $(LIB_DIR)
	$(CC) $(OBJS) $(LIB) $(LDFLAGS) -o $(NAME)
>>>>>>> a03cb3b6e4b5001b542d8eca4c07d04411d8276b
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
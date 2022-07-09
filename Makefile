NAME = minishell

CC = gcc
SRCS =  built-in/cd.c\
		built-in/cd_utils.c\
		built-in/echo.c\
		built-in/export.c\
		built-in/pwd.c\
		built-in/unset.c\
		built-in/env.c\
		built-in/exit.c\
		environ/environ.c\
		environ/environ_utils.c\
		environ/environ_for_env.c\
		ft_func/ft_close.c\
		ft_func/ft_signal.c\
		ft_func/ft_dup.c\
		ft_func/ft_dup2.c\
		ft_func/ft_execve.c\
		ft_func/ft_fork.c\
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
		executer/check_cmd.c\
		executer/execute_a_cmd.c\
		executer/execute_cmds.c\
		executer/heredoc.c \
		executer/pipe.c \
		executer/redirect.c \
		executer/wait.c \
		path.c\
		argv.c\
		signal.c\
		minishell.c
OBJ_DIR = ./obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
LIB = ./libftx/libft.a
LIB_DIR = ./libftx
CC = gcc
CFLAGS = -Wall -Wextra -Werror
IFLAGS = -I $(shell brew --prefix readline)/include
LDFLAGS = -lreadline -lhistory -L $(shell brew --prefix readline)/lib

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@);
	@$(CC) -g -Wall -Wextra -Werror $(IFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(MAKE) -C $(LIB_DIR)
	$(CC) $(IFLAGS) $(LDFLAGS) $(OBJS) $(LIB)  -o $(NAME)

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

norm: $(SRCS)
	norminette $(SRCS)

.PHONY: all  clean fclean re bonus
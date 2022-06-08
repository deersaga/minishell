NAME	= libft.a
CC 	= gcc
CFLAG = -Wall -Wextra -Werror
DEBUGF	= -g -fsanitize=undefined -fsanitize=address 
OBJ_DIR = ./obj
SRCS =	ft_strlcpy.c \
		ft_strlen.c \
		ft_strlcat.c \
		ft_strrchr.c \
		ft_strnstr.c \
		ft_strncmp.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memmove.c \
		ft_atoi.c \
		ft_strchr.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_calloc.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_split.c \
		ft_itoa.c \
		ft_calloc.c \
		ft_strmapi.c \
		ft_striteri.c\
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_strtrim.c \
		ft_lstnew.c \
		ft_lstadd_front.c \
		ft_lstsize.c \
		ft_lstlast.c \
		ft_lstadd_back.c \
		ft_lstdelone.c \
		ft_lstclear.c \
		ft_lstiter.c \
		ft_lstmap.c \
		ft_ltoa.c \
		ft_uitoa.c \
		ft_ulltoa.c \
		ft_int_abs.c \
		ft_fdlstnew.c \
		ft_get_next_line.c \
		ft_hextoui.c \
		ft_atol.c \
		ft_strcmp.c

OBJS	=	$(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	ar -r $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAG) -o $@ -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re

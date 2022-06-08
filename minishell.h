#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <sys/signalvar.h>
# include "./libftx/libft.h"

typedef struct s_token {
	char			*token;
	char			*type;
	struct s_token	*next;
	
} t_token;

typedef struct s_command {
	t_token	*token;
	int		num_token;
	
} t_command;

typedef struct s_mshell {
	t_command	**commands;
	int		num_commands;
	
} t_mshell;

#endif

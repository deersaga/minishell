#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <stdbool.h>
# include <sys/signalvar.h>
# include "./libftx/libft.h"

typedef enum {
	T_END,
	T_WORD,		//1
	T_SQUOTE,	//2
	T_DQUOTE,	//3
	T_PIPE,		//4
	T_REDIR_OUT,//5
	T_REDIR_IN,	//6
	T_APPEND,	//7
	T_HEREDOC,	//8
} type_token;

typedef struct s_envList {
	char				*key;
	char				*val;
	struct s_envList	*next;
} t_envList;

typedef struct s_token {
	char			*token;
	type_token		type;
	int				fd;
	struct s_token	*next;
	
} t_token;

typedef struct s_command {
	struct s_token	*token;
	char			**argv;
	int				num_token;
} t_command;

typedef struct s_mshell {
	struct s_command	*commands;
	int					num_commands;
	struct s_envList	*env;
} t_mshell;


int		ft_cd(int argc, char **argv, t_mshell *mshell);
void	init_env(t_mshell *mshell);
void	register_or_update_env(t_mshell *mshell, char *tar_key, char *tar_val);
char	*get_env(t_mshell *mshell, char *key);
void	print_env(t_envList *env);
int		tokenizer(t_mshell *mshell, char *cmdline);
#endif

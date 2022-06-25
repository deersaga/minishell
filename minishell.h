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
	T_DELM,
	T_WORD,		//1
	T_SQUOTE,	//2
	T_DQUOTE,	//3
	T_PIPE,		//4
	T_REDIR_OUT,//5
	T_REDIR_IN,	//6
	T_APPEND,	//7
	T_HEREDOC,	//8
	T_END,
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

typedef struct s_redir {
	char			*file;
	type_token		type;
	int				fd;
	struct s_redir	*next;
	
} t_redir;

typedef struct s_command {
	struct s_token		*token;
	char				**argv;
	int					argc;
	struct s_redir		*redir_in;
	struct s_redir		*redir_out;
	struct s_redir		*redir_append;
	struct s_redir		*heredoc;
	struct s_command	*next;
	int					num_token;
} t_command;

typedef struct s_mshell {
	struct s_command	*commands;
	int					num_commands;
	struct s_envList	*env;
} t_mshell;

//built-in
int		ft_cd(t_mshell *mshell, t_command *cmd);
int		ft_pwd(t_mshell *mshell, t_command *cmd);
int		ft_unset(t_mshell *mshell, t_command *cmd);
int		ft_export(t_mshell *mshell, t_command *cmd);
int		ft_exit(t_mshell *mshell, t_command *cmd);
int		ft_env(t_mshell *mshell, t_command *cmd);
int		ft_echo(t_mshell *mshell, t_command *cmd);

//environment
void	init_env(t_mshell *mshell);
void	register_or_update_env(t_mshell *mshell, char *tar_key, char *tar_val);
char	*get_env(t_mshell *mshell, char *key);
void	sort_env(t_envList *head);
void	delete_one_env(t_mshell *mshell, char *del_key);
void	print_env(t_envList *env);
void	delete_all_env(t_mshell *mshell);

//tokenizer
t_token	*tokenizer(t_mshell *mshell, char *cmdline);
t_token	*verbose_tokenizer(char	*cmdline, t_token *head);
void	delete_one_token(t_token **head, t_token *pre, t_token *cur, t_token *next);
void	print_tokens(t_token *head);
void	free_all_token(t_token *head);
char	*concat_expanded_tokens(t_mshell *mshell, t_token *head);
int		is_redirect_token(type_token type);
t_token	*add_front_tokens(t_token **head, t_token *retoken, t_token *pre, t_token *cur);
t_token	*expand_and_retokenize(t_mshell *mshell, t_token *head);
t_token	*format_tokens(t_token *head);
t_token	*skip_delimiter_token(t_token *cur);
t_token	*skip_by_next_delimiter_token(t_token *cur);
char	*concat_tokens(t_mshell *mshell, t_token *head);

//parser内のtokenizer以外
int		parser(t_mshell *mshell, char *cmdline);
void	free_commands(t_command *cmd);
void	print_commands(t_mshell *mshell);
char	*expansion(t_mshell *mshell, char *str);
char	*ft_strreplace(char *src, char *target, char *implant, size_t *start);
int		check_syntax(t_token *head);

//other
void	free_array(char **array);
char	**create_argv(t_mshell *mshell, t_command *cmd);
void	print_array(char **array);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:05:45 by ktada             #+#    #+#             */
/*   Updated: 2022/07/14 23:04:42 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define BUFF_SIZE 1028
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <err.h>
# include <errno.h>
# include <limits.h>
# include <sys/signalvar.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include "./libftx/libft.h"

volatile sig_atomic_t	g_heredoc_sigint;

typedef enum e_type_token {
	T_DELM,
	T_WORD,
	T_SQUOTE,
	T_DQUOTE,
	T_PIPE,
	T_REDIR_OUT,
	T_REDIR_IN,
	T_APPEND,
	T_HEREDOC,
	T_END,
	T_ERR,
}	t_type_token;

typedef struct s_envList {
	char				*key;
	char				*val;
	struct s_envList	*next;
}	t_envList;

typedef struct s_token {
	char			*token;
	t_type_token	type;
	int				fd;
	struct s_token	*next;
}	t_token;

typedef struct s_redir {
	char			*file;
	char			*heredoc_eof;
	t_type_token	type;
	int				has_quote;
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command {
	struct s_token		*token;
	char				**argv;
	size_t				argc;
	struct s_redir		*redir;
	struct s_command	*next;
	int					num_token;
}	t_command;

typedef struct s_info {
	char	*pwd;
	char	*shlvl;
}	t_info;

typedef struct s_mshell {
	struct s_command	*commands;
	size_t				num_commands;
	int					exit_status;
	struct s_envList	*env;
	struct s_info		info;
}	t_mshell;

//built-in
int				ft_cd(t_mshell *mshell, t_command *cmd);
char			*get_abs_path(char	*path);
int				ft_pwd(t_mshell *mshell, t_command *cmd);
int				ft_unset(t_mshell *mshell, t_command *cmd);
int				ft_export(t_mshell *mshell, t_command *cmd);
void			append_lefthand_side(t_mshell *mshell,
					t_command *cmd, size_t i, char *key_val[2]);
int				ft_exit(t_mshell *mshell, t_command *cmd);
int				ft_env(t_mshell *mshell, t_command *cmd);
int				ft_echo(t_mshell *mshell, t_command *cmd);

//environment
void			init_env(t_mshell *mshell, char **environ);
void			register_or_update_env(t_mshell *mshell,
					char *tar_key, char *tar_val);
char			*get_env(t_mshell *mshell, char *key);
void			sort_env(t_envList *head);
void			delete_one_env(t_mshell *mshell, char *del_key);
void			print_env(t_envList *env);
void			delete_all_env(t_mshell *mshell);
void			get_key_val(char *key_eq_val, char *key_val[2]);

//tokenizer
t_token			*tokenizer(char	*cmdline);
t_token			*new_token(t_token *cur, char *token, t_type_token type);
void			delete_one_token(t_token **head, t_token *pre,
					t_token *cur, t_token *next);
t_token			*add_front_tokens(t_token **head, t_token *retoken,
					t_token *pre, t_token *cur);
t_token			*format_tokens(t_token *head);
void			print_tokens(t_token *head);
void			free_all_token(t_token *head);
t_token			*expand_and_retokenize(t_mshell *mshell, t_token *head);
char			*concat_expanded_tokens(t_mshell *mshell, t_token *head);
char			*concat_tokens(t_token *head);
char			*subtoken(t_token *start, t_token *end);
void			add_redir_info(t_command *cmd, t_token *cur);
t_token			*delete_redir_token(t_command *cmd,
					t_token *previous, t_token *current);
void			print_redir(t_command *cmd);
t_token			*skip_delimiter_token(t_token *cur);
t_token			*skip_by_next_delimiter_token(t_token *cur);
t_token			*skip_word_quote_token(t_token *cur);
t_token			*get_first_non_delimiter_token(t_token *head);
t_type_token	get_token_type(char	*cur, size_t *i);
void			get_quote_type_and_len( \
					char *cmdline, size_t i, size_t *len, t_type_token *type);
int				is_redirect_token(t_type_token type);
int				is_operator(char c);
int				is_quote(char cur);
int				is_delimiter(char c);
int				is_operator_token(t_type_token type);
int				all_num(char *s);

//parser and expansion
int				parser(t_mshell *mshell, char *cmdline);
char			*expansion(t_mshell *mshell, char *str);
char			*ft_strreplace(char *src, char *target,
					char *implant, size_t *start);
int				check_syntax(t_token *head);
void			free_commands(t_command *cmd);
void			free_array(char **array);
void			print_commands(t_mshell *mshell);
void			print_array(char **array);

//executer
void			execute_any_cmd(t_mshell *mshell);
int				execute_a_builtin_cmd(t_mshell *mshell, t_command *cmd);
int				**make_pipe_list(t_mshell *mshell);
void			close_pipe_list(t_mshell *mshell, int **pipe_list);
void			free_pipe_list(t_mshell *mshell, int **pipe_list);
void			reconnect_pipe_with_stdio(\
					t_mshell *mshell, size_t cur_idx, int **pipe_list);
int				reconnect_redir_with_stdio(t_command *cmd);
int				openfile(char *filename, t_type_token mode);
void			wait_childs(t_mshell *mshell, pid_t pid);
char			*get_cmd_name(t_token *head);
//int				check_minishell(t_mshell *mshell, t_command *cmd);
int				is_export_cmd(t_token *head);
int				is_builtin_cmd(t_mshell *mshell, t_command *cur_com);
int				execute_a_builtin(t_mshell *mshell, t_command *cmd);
void			create_argv(t_mshell *mshell, t_command *cmd);
char			**make_environ(t_mshell *mshell);

//heredoc
char			*make_heredoc_filename(size_t	heredoc_id);
int				create_heredoc_files(t_mshell *mshell);
void			create_heredoc_file(t_mshell *mshell, t_redir *heredoc);
void			delete_heredoc_files(t_mshell	*mshell);

//signal
void			signal_handler_int(int sig);
void			signal_handler_heredoc(int sig);
void			signal_handler_pipe(int sig);

//utils
char			*get_cmd_path(t_mshell *mshell, char *cmd);
bool			is_valid_cmdline(t_mshell *mshell, char *cmdline);

//ft_func
void			ft_close(int fd);
void			ft_signal(int sig, void (*func)(int));
int				ft_dup(int oldfd);
void			ft_dup2(int oldfd, int newfd);
void			ft_execve(char *file, char **argv, char **env);
pid_t			ft_fork(void);
long long		ft_try_atoll(const char *str, bool *error);

#endif

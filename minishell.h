/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:05:45 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 21:09:20 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <fcntl.h>
# include <err.h>
# include <errno.h>
# include <limits.h>
# include <sys/signalvar.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <limits.h>
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
	struct s_redir		*redir_in;
	struct s_redir		*redir_out;
	struct s_command	*next;
	int					num_token;
}	t_command;

typedef struct s_mshell {
	struct s_command	*commands;
	size_t				num_commands;
	int					exit_status;
	struct s_envList	*env;
}	t_mshell;

//built-in
int				ft_cd(t_mshell *mshell, t_command *cmd);
int				ft_pwd(t_mshell *mshell, t_command *cmd);
int				ft_unset(t_mshell *mshell, t_command *cmd);
int				ft_export(t_mshell *mshell, t_command *cmd);
int				ft_exit(t_mshell *mshell, t_command *cmd);
int				ft_env(t_mshell *mshell, t_command *cmd);
int				ft_echo(t_mshell *mshell, t_command *cmd);

//environment
void			init_env(t_mshell *mshell);
void			register_or_update_env(t_mshell *mshell,
					char *tar_key, char *tar_val);
char			*get_env(t_mshell *mshell, char *key);
void			sort_env(t_envList *head);
void			delete_one_env(t_mshell *mshell, char *del_key);
void			print_env(t_envList *env);
void			print_export(t_envList *env);
void			delete_all_env(t_mshell *mshell);
char			**make_environ(t_mshell *mshell);
t_envList		*copy_env(t_envList *env);
void			get_key_val(char *key_eq_val, char *key_val[2]);

//tokenizer
t_token			*tokenizer(char	*cmdline);
void			delete_one_token(t_token **head, t_token *pre,
					t_token *cur, t_token *next);
void			print_tokens(t_token *head);
void			free_all_token(t_token *head);
char			*concat_expanded_tokens(t_mshell *mshell, t_token *head);
int				is_redirect_token(t_type_token type);
t_token			*add_front_tokens(t_token **head, t_token *retoken,
					t_token *pre, t_token *cur);
t_token			*expand_and_retokenize(t_mshell *mshell, t_token *head);
t_token			*format_tokens(t_token *head);
t_token			*skip_delimiter_token(t_token *cur);
t_token			*skip_by_next_delimiter_token(t_token *cur);
t_token			*skip_word_quote_token(t_token *cur);
char			*concat_tokens(t_token *head);
t_token			*get_first_non_delimiter_token(t_token *head);
void			print_redir(t_command *cmd);
char			*subtoken(t_token *start, t_token *end);
t_type_token	get_token_type(char	*cur, size_t *i);
void			get_quote_type_and_len( \
					char *cmdline, size_t i, size_t *len, t_type_token *type);
int				is_operator(char c);
int				is_quote(char cur);
int				is_delimiter(char c);
int				is_operator_token(t_type_token type);
int				all_num(char *s);
t_token			*new_token(t_token *cur, char *token, t_type_token type);

//parser内のtokenizer以外
int				parser(t_mshell *mshell, char *cmdline);
void			free_commands(t_command *cmd);
void			print_commands(t_mshell *mshell);
char			*expansion(t_mshell *mshell, char *str);
char			*ft_strreplace(char *src, char *target,
					char *implant, size_t *start);
int				check_syntax(t_token *head);

//executer
int				execute_a_builtin_cmd(t_mshell *mshell, t_command *cmd);
void			free_pipe_list(t_mshell *mshell, int **pipe_list);
int				**make_array_2d_int(size_t h, size_t w);
int				**make_pipe_list(t_mshell *mshell);
void			close_pipe_list(t_mshell *mshell, int **pipe_list);
void			reconnect_pipe_with_stdio(\
					t_mshell *mshell, size_t cur_idx, int **pipe_list);
void			execute_any_cmd(t_mshell *mshell);

//heredoc.c
char			*make_heredoc_filename(size_t	heredoc_id);
void			create_heredoc_files(t_mshell *mshell);
//static int		check_heredoc_sigint(void);
void			create_heredoc_file(t_mshell *mshell, t_redir *heredoc);
void			delete_heredoc_files(t_mshell	*mshell);

//other
void			free_array(char **array);
void			create_argv(t_mshell *mshell, t_command *cmd);
void			print_array(char **array);
char			*get_cmd_path(t_mshell *mshell, char *cmd);
int				openfile(char *filename, t_type_token mode);
void			signal_handler_int(int sig);
void			signal_handler_heredoc(int sig);
void			signal_handler_quit(int sig);
void			signal_handler_exec_parent(int sig);
int				check_builtin(t_mshell *mshell, t_command *cmd);
int				reconnect_redir_with_stdio(t_command *cmd);
void			close_pipe_list(t_mshell *mshell, int **pipe_list);
int				execute_a_builtin(t_mshell *mshell, t_command *cmd);
int				**make_pipe_list(t_mshell *mshell);
void			add_redir_info(t_command *cmd, t_token *cur);
t_token			*delete_redir_token(t_command *cmd,
					t_token *previous, t_token *current);
void			create_heredoc_file(t_mshell *mshell, t_redir *heredoc);
void			delete_heredoc_files(t_mshell *mshell);
char			*make_heredoc_filename(size_t heredoc_id);
void			wait_childs(t_mshell *mshell);
char			*get_cmd_name(t_token *head);
int				is_export_cmd(t_token *head);
int				is_builtin_cmd(t_mshell *mshell, t_command *cur_com);
char			*get_abs_path(char	*path);

#endif

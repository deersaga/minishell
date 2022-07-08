/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 17:07:53 by kaou              #+#    #+#             */
/*   Updated: 2022/07/08 17:08:15 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_one_of_cmds(t_mshell *mshell, size_t cur_idx, \
					t_command *cur_com, int **pipe_list)
{
	extern char	**environ;
	char		*command_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	reconnect_pipe_with_stdio(mshell, cur_idx, pipe_list);
	reconnect_redir_with_stdio(cur_com);
	if (!ft_strcmp("./minishell", cur_com->token->token) \
		&& mshell->num_commands > 1)
		exit(0);
	if (is_builtin_cmd(mshell, cur_com))
		exit(execute_a_builtin(mshell, cur_com));
	create_argv(mshell, cur_com);
	command_path = get_cmd_path(mshell, cur_com->argv[0]);
	environ = make_environ(mshell);
	execve(command_path, cur_com->argv, environ);
	free_array(environ);
	exit(127);
}

void	execute_empty_cmd(t_mshell *mshell, t_command *cmd)
{
	int		dup_stdio_fd[2];

	dup_stdio_fd[0] = dup(0);
	dup_stdio_fd[1] = dup(1);
	reconnect_redir_with_stdio(cmd);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	dup2(dup_stdio_fd[0], STDIN_FILENO);
	dup2(dup_stdio_fd[1], STDOUT_FILENO);
	close(dup_stdio_fd[0]);
	close(dup_stdio_fd[1]);
	delete_heredoc_files(mshell);
	signal(SIGINT, signal_handler_int);
	signal(SIGQUIT, SIG_IGN);
}

void	execute_cmds(t_mshell *mshell, t_command *head)
{
	t_command	*cur_com;
	size_t		cur_idx;
	pid_t		pid;
	int			**pipe_list;

	cur_com = head;
	pipe_list = make_pipe_list(mshell);
	cur_idx = 0;
	while (cur_idx < mshell->num_commands)
	{
		if (cur_idx > 0 && !is_export_cmd(cur_com->token))
			cur_com->token = expand_and_retokenize(mshell, cur_com->token);
		pid = fork();
		if (pid == 0)
			execute_one_of_cmds(mshell, cur_idx, cur_com, pipe_list);
		cur_idx++;
		cur_com = cur_com->next;
	}
	close_pipe_list(mshell, pipe_list);
	wait_childs(mshell);
	free_pipe_list(mshell, pipe_list);
}

void	execute_any_cmd(t_mshell *mshell)
{
	t_command	*head_cmd;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	create_heredoc_files(mshell);
	head_cmd = mshell->commands;
	if (skip_delimiter_token(head_cmd->token)->token == NULL)
	{
		execute_empty_cmd(mshell, head_cmd);
		return ;
	}
	if (!is_export_cmd(head_cmd->token))
		head_cmd->token = expand_and_retokenize(mshell, head_cmd->token);
	if (mshell->num_commands == 1 && is_builtin_cmd(mshell, head_cmd))
		mshell->exit_status = execute_a_builtin_cmd(mshell, head_cmd);
	else
		execute_cmds(mshell, head_cmd);
	delete_heredoc_files(mshell);
	signal(SIGINT, signal_handler_int);
	signal(SIGQUIT, SIG_IGN);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 17:07:53 by ktada             #+#    #+#             */
/*   Updated: 2022/07/15 16:15:49 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_empty_cmd(t_mshell *mshell, t_command *cmd)
{
	int		dup_stdio_fd[2];
	int		status;

	status = 0;
	dup_stdio_fd[0] = ft_dup(0);
	dup_stdio_fd[1] = ft_dup(1);
	if (reconnect_redir_with_stdio(cmd) == -1)
		status = 1;
	ft_close(STDIN_FILENO);
	ft_close(STDOUT_FILENO);
	ft_dup2(dup_stdio_fd[0], STDIN_FILENO);
	ft_dup2(dup_stdio_fd[1], STDOUT_FILENO);
	ft_close(dup_stdio_fd[0]);
	ft_close(dup_stdio_fd[1]);
	delete_heredoc_files(mshell);
	ft_signal(SIGINT, signal_handler_int);
	ft_signal(SIGQUIT, SIG_IGN);
	return (status);
}

void	execute_one_of_cmds(t_mshell *mshell, size_t cur_idx, \
					t_command *cur_com, int **pipe_list)
{
	char		**environ;
	char		*command_path;

	ft_signal(SIGINT, SIG_DFL);
	ft_signal(SIGQUIT, SIG_DFL);
	reconnect_pipe_with_stdio(mshell, cur_idx, pipe_list);
	if (skip_delimiter_token(cur_com->token)->token == NULL)
		exit(execute_empty_cmd(mshell, cur_com));
	if (reconnect_redir_with_stdio(cur_com) == -1)
		exit(1);
	if (is_builtin_cmd(mshell, cur_com))
		exit(execute_a_builtin(mshell, cur_com));
	create_argv(mshell, cur_com);
	command_path = get_cmd_path(mshell, cur_com->argv[0]);
	environ = make_environ(mshell);
	ft_execve(command_path, cur_com->argv, environ);
	free_array(environ);
	exit(127);
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
		pid = ft_fork();
		if (pid == 0)
			execute_one_of_cmds(mshell, cur_idx, cur_com, pipe_list);
		mshell->exit_status = 0;
		cur_idx++;
		cur_com = cur_com->next;
	}
	close_pipe_list(mshell, pipe_list);
	wait_childs(mshell, pid);
	free_pipe_list(mshell, pipe_list);
}

void	execute_any_cmd(t_mshell *mshell)
{
	t_command	*head_cmd;

	ft_signal(SIGINT, SIG_IGN);
	ft_signal(SIGQUIT, SIG_IGN);
	if (!create_heredoc_files(mshell))
	{
		head_cmd = mshell->commands;
		if (skip_delimiter_token(head_cmd->token)->token == NULL \
			&& mshell->num_commands == 1)
		{
			mshell->exit_status = execute_empty_cmd(mshell, head_cmd);
			return ;
		}
		if (!is_export_cmd(head_cmd->token))
			head_cmd->token = expand_and_retokenize(mshell, head_cmd->token);
		if (mshell->num_commands == 1 && is_builtin_cmd(mshell, head_cmd))
			mshell->exit_status = execute_a_builtin_cmd(mshell, head_cmd);
		else
			execute_cmds(mshell, head_cmd);
	}
	else
		mshell->exit_status = 1;
	delete_heredoc_files(mshell);
	ft_signal(SIGINT, signal_handler_int);
	ft_signal(SIGQUIT, SIG_IGN);
}

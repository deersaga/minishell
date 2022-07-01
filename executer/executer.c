/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 17:07:53 by kaou              #+#    #+#             */
/*   Updated: 2022/07/01 16:12:50 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_pipe_list(t_mshell *mshell, int **pipe_list)
{
	size_t	i;
	size_t	pipe_list_num;

	i = 0;
	pipe_list_num = mshell->num_commands - 1;
	while (i < pipe_list_num)
		free(pipe_list[i++]);
	free(pipe_list);
}

int	**make_array_2d_int(size_t h, size_t w)
{
	int		**array;
	size_t	i;

	array = ft_calloc(h, sizeof(int *));
	i = 0;
	while (i < h)
	{
		array[i] = ft_calloc(w, sizeof(int));
		i++;
	}
	return (array);
}
int	**make_pipe_list(t_mshell *mshell)
{
	int		**pipe_list;
	size_t	pipe_list_num;
	size_t	i;

	pipe_list_num = mshell->num_commands - 1;
	pipe_list = make_array_2d_int(pipe_list_num, 2);
	i = 0;
	while (i < pipe_list_num)
	{
		if (pipe(pipe_list[i]) == -1)
		{
			//todo 標準エラー?
			printf("make pipe error\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return (pipe_list);
}

void	close_pipe_list(t_mshell *mshell, int **pipe_list)
{
	size_t	i;
	size_t	num_pipe_list;

	num_pipe_list = mshell->num_commands - 1;
	i = 0;
	while (i < num_pipe_list)
	{
		close(pipe_list[i][0]);
		close(pipe_list[i][1]);
		i++;
	}
}

void	reconnect_pipe_with_stdio(\
	t_mshell *mshell, size_t cur_idx, int **pipe_list)
{
	if (cur_idx > 0)
		dup2(pipe_list[cur_idx - 1][0], STDIN_FILENO);
	if (cur_idx + 1 < mshell->num_commands)
		dup2(pipe_list[cur_idx][1], STDOUT_FILENO);
	close_pipe_list(mshell, pipe_list);
}

void	reconnect_redir_with_stdio(\
	t_mshell *mshell, t_command *cur_com, size_t cur_idx, int **pipe_list)
{
	int		fd;
	t_redir	*cur_redir_in;
	t_redir	*cur_redir_out;

	(void)mshell;
	(void)cur_idx;
	(void)pipe_list;
	cur_redir_in = cur_com->redir_in;
	//現状のredir_inは終端がnullになっていないため問題が起きる
	while (cur_redir_in && cur_redir_in->file)
	{
		if (cur_redir_in->type == T_REDIR_IN || cur_redir_in->type == T_HEREDOC)
		{
			fd = openfile(cur_redir_in->file, T_REDIR_IN);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else
			exit(EXIT_FAILURE || !printf("reconnect_redir_with_stdio error\n"));
		cur_redir_in = cur_redir_in->next;
	}
	cur_redir_out = cur_com->redir_out;
	while (cur_redir_out && cur_redir_out->file)
	{
		if (cur_redir_out->type != T_REDIR_OUT && cur_redir_out->type != T_APPEND)
			exit(EXIT_FAILURE || !printf("reconnect_redir_with_stdio error\n"));
		fd = openfile(cur_redir_out->file, cur_redir_out->type);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		cur_redir_out = cur_redir_out->next;
	}
}

void	wait_childs(t_mshell *mshell)
{
	int	status;

	while (wait(&status) >= 0);
	mshell->exit_status = WEXITSTATUS(status);
}

void	execute_command(t_mshell *mshell, size_t cur_idx, \
					t_command *cur_com, int **pipe_list)
{
	extern char	**environ;
	char		*command_path;

	reconnect_pipe_with_stdio(mshell, cur_idx, pipe_list);
	reconnect_redir_with_stdio(mshell, cur_com, cur_idx, pipe_list);
	if (check_builtin(mshell, cur_com))
		exit(execute_a_builtin(mshell, cur_com));
	command_path = get_cmd_path(mshell, cur_com->argv[0]);
	execve(command_path, cur_com->argv, environ);
}

void	create_heredoc_files(t_mshell *mshell)
{
	t_command	*cur_com;
	t_redir		*cur_redir_in;
	size_t		i;

	cur_com = mshell->commands;
	i = 0;
	while (i < mshell->num_commands)
	{
		cur_redir_in = cur_com->redir_in;
		while (cur_redir_in && cur_redir_in->file)
		{
			if (cur_redir_in->type == T_HEREDOC)
				create_heredoc_file(mshell, cur_redir_in);
			cur_redir_in = cur_redir_in->next;
		}
		cur_com = cur_com->next;
		i++;
	}
	g_heredoc_sigint = 0;
}

void	execute_commands(t_mshell *mshell)
{
	int			**pipe_list;
	pid_t		*child_pid_list;
	size_t		cur_idx;
	t_command	*cur_com;

	//num_command > 0という前提で考えている
	if (skip_delimiter_token(mshell->commands->token)->token == NULL)
	{
		mshell->commands->argv = NULL;
		return;
	}
	create_heredoc_files(mshell);
	if (mshell->num_commands == 1)
	{
		mshell->commands->token = expand_and_retokenize(mshell, mshell->commands->token);
		create_argv(mshell, mshell->commands);
		execute_a_command(mshell, mshell->commands);
		//heredocの一時ファイルが消されないことがあるかも
		delete_heredoc_files(mshell);
		return ;
	}
	child_pid_list = ft_calloc(mshell->num_commands, sizeof(pid_t));
	pipe_list = make_pipe_list(mshell);
	cur_idx = 0;
	cur_com = mshell->commands;
	while (cur_idx < mshell->num_commands)
	{
		cur_com->token = expand_and_retokenize(mshell, cur_com->token);
		create_argv(mshell, cur_com);
		child_pid_list[cur_idx] = fork();
		if (child_pid_list[cur_idx] == 0)
			execute_command(mshell, cur_idx, cur_com, pipe_list);
		cur_idx++;
		cur_com = cur_com->next;
	}
	close_pipe_list(mshell, pipe_list);
	wait_childs(mshell);
	free(child_pid_list);
	free_pipe_list(mshell, pipe_list);
	delete_heredoc_files(mshell);
}

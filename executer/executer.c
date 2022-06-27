/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 17:07:53 by kaou              #+#    #+#             */
/*   Updated: 2022/06/27 18:58:04 by kaou             ###   ########.fr       */
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
		free(pipe_list[i]);
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
	}
	return (array);
}

int	**make_pipe_list(t_mshell *mshell)
{
	int		**pipe_list;
	int		pipe_list_num;
	size_t	i;

	pipe_list_num = mshell->num_commands - 1;
	pipe_list = make_array_2d_int(pipe_list_num, 2);
	i = 0;
	while (i < pipe_list_num)
	{
		if (pipe(pipe_list[i]) == -1)
		{
			//todo 標準エラー?
			ft_printf("make pipe error\n");
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

	cur_redir_in = cur_com->redir_in;
	//現状のredir_inは終端がnullになっていないため問題が起きる
	while (cur_redir_in)
	{
		if (cur_redir_in->type == T_REDIR_IN)
		{
			//redir_inがひとつならこれでいいはず
			//複数あるときに上書きしていけるのかは未確認
			fd = openfile(cur_redir_in->file, T_REDIR_IN);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cur_redir_in->type == T_HEREDOC)
		{
			//todo;
		}else
			exit(EXIT_FAILURE || !ft_printf("reconnect_redir_with_stdio error\n"));
		cur_redir_in = cur_redir_in->next;
	}
	cur_redir_out = cur_com->redir_out;
	while (cur_redir_out)
	{
		if (cur_redir_out->type != T_REDIR_OUT && cur_redir_out->type != T_APPEND)
			exit(EXIT_FAILURE || !ft_printf("reconnect_redir_with_stdio error\n"));
		fd = openfile(cur_redir_out->file, cur_redir_out->type);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		cur_redir_out = cur_redir_out->next;
	}
}

void	execute_command(t_mshell *mshell, size_t cur_idx, \
					t_command *cur_com, int **pipe_list)
{
	extern char	**environ;
	char		*command_path;

	reconnect_pipe_with_stdio(mshell, cur_idx, pipe_list);
	reconnect_redir_with_stdio(mshell, cur_com, cur_idx, pipe_list);
	command_path = get_command_path(cur_com->argv[0]);
	execve(command_path, cur_com->argv, environ);
}

void	execute_commands(t_mshell *mshell)
{
	int			**pipe_list;
	pid_t		*child_pid_list;
	size_t		cur_idx;
	t_command	*cur_com;

	//num_command > 0という前提で考えている
	child_pid_list = ft_calloc(mshell->num_commands, sizeof(pid_t));
	pipe_list = make_pipe_list(mshell);
	cur_idx = 0;
	cur_com = mshell->commands;
	while (cur_idx < mshell->num_commands)
	{
		child_pid_list[cur_idx] = fork();
		if (child_pid_list[cur_idx] == 0)
			execute_command(mshell, cur_idx, cur_com, pipe_list);
		cur_idx++;
		cur_com = cur_com->next;
	}
	close_pipe_list(mshell, pipe_list);
	wait_childs();
	free(child_pid_list);
	free_pipe_list(mshell, pipe_list);
}

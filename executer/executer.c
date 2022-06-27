/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 17:07:53 by kaou              #+#    #+#             */
/*   Updated: 2022/06/27 17:57:28 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
/*
typedef struct s_mshell {
	struct s_command	*commands;
	int					num_commands;
	struct s_envList	*env;
} t_mshell;
*/

/*
typedef struct s_command {
	struct s_token		*token;
	char				**argv;
	struct s_redir		*redir_in;
	struct s_redir		*redir_out;
	struct s_command	*next;
	int					num_token;
} t_command;
*/

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
			execute_command(cur_idx, cur_com, pipe_list);
		cur_idx++;
		cur_com = cur_com->next;
	}
	close_pipe_list();
	wait_childs();
	free(child_pid_list);
	free_pipe_list(mshell, pipe_list);
}

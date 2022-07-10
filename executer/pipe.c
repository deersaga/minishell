/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:23:31 by ktada             #+#    #+#             */
/*   Updated: 2022/07/10 14:34:39 by katakagi         ###   ########.fr       */
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

static int	**make_array_2d_int(size_t h, size_t w)
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
	if (pipe_list_num == 0)
		return (NULL);
	pipe_list = make_array_2d_int(pipe_list_num, 2);
	i = 0;
	while (i < pipe_list_num)
	{
		if (pipe(pipe_list[i]) == -1)
		{
			perror("pipe");
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
		ft_close(pipe_list[i][0]);
		ft_close(pipe_list[i][1]);
		i++;
	}
}

void	reconnect_pipe_with_stdio(\
	t_mshell *mshell, size_t cur_idx, int **pipe_list)
{
	if (cur_idx > 0)
		ft_dup2(pipe_list[cur_idx - 1][0], STDIN_FILENO);
	if (cur_idx + 1 < mshell->num_commands)
		ft_dup2(pipe_list[cur_idx][1], STDOUT_FILENO);
	close_pipe_list(mshell, pipe_list);
}

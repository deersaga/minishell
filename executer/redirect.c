/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:31:36 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 21:04:00 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	reconnect_redir_with_stdio(t_command *cur_com)
{
	int		fd;
	t_redir	*cur_redir_in;
	t_redir	*cur_redir_out;

	cur_redir_in = cur_com->redir_in;
	while (cur_redir_in && cur_redir_in->file)
	{
		if (cur_redir_in->type == T_REDIR_IN || cur_redir_in->type == T_HEREDOC)
		{
			fd = openfile(cur_redir_in->file, T_REDIR_IN);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		cur_redir_in = cur_redir_in->next;
	}
	cur_redir_out = cur_com->redir_out;
	while (cur_redir_out && cur_redir_out->file)
	{
		fd = openfile(cur_redir_out->file, cur_redir_out->type);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		cur_redir_out = cur_redir_out->next;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:31:36 by kaou              #+#    #+#             */
/*   Updated: 2022/07/09 13:49:01 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	openfile(char *filename, t_type_token mode)
{
	int	fd;

	if (mode == T_REDIR_IN)
	{
		if (access(filename, F_OK))
		{
			fprintf(stderr, "%s: No such file or directory\n", filename);
			exit(EXIT_FAILURE);
		}
		fd = (open(filename, O_RDONLY));
	}
	else if (mode == T_REDIR_OUT)
		fd = (open(filename, O_CREAT | O_WRONLY | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	else
		fd = (open(filename, O_CREAT | O_WRONLY | O_APPEND, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	if (fd < 0)
		fprintf(stderr, "%s: No such file or directory\n", filename);
	return (fd);
}

int	reconnect_redir_in(t_command *cur_com)
{
	int		fd;
	t_redir	*cur_redir_in;

	cur_redir_in = cur_com->redir_in;
	while (cur_redir_in && cur_redir_in->file)
	{
		if (cur_redir_in->type == T_REDIR_IN || cur_redir_in->type == T_HEREDOC)
		{
			fd = openfile(cur_redir_in->file, T_REDIR_IN);
			if (fd < 0)
				return (-1);
			ft_dup2(fd, STDIN_FILENO);
			ft_close(fd);
		}
		cur_redir_in = cur_redir_in->next;
	}
	return (0);
}

int	reconnect_redir_out(t_command *cur_com)
{
	int		fd;
	t_redir	*cur_redir_out;

	cur_redir_out = cur_com->redir_out;
	while (cur_redir_out && cur_redir_out->file)
	{
		fd = openfile(cur_redir_out->file, cur_redir_out->type);
		if (fd < 0)
			return (-1);
		ft_dup2(fd, STDOUT_FILENO);
		ft_close(fd);
		cur_redir_out = cur_redir_out->next;
	}
	return (0);
}

int	reconnect_redir_with_stdio(t_command *cur_com)
{
	if (reconnect_redir_in(cur_com) < 0)
		return (-1);
	if (reconnect_redir_out(cur_com) < 0)
		return (-1);
	return (0);
}

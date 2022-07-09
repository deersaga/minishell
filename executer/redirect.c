/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:31:36 by ktada              #+#    #+#             */
/*   Updated: 2022/07/09 18:18:46 by katakagi         ###   ########.fr       */
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
			perror(filename);
			return (-1);
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
		perror(filename);
	return (fd);
}

int	reconnect_redir_in(t_redir *redir)
{
	int		fd;

	fd = openfile(redir->file, T_REDIR_IN);
	if (fd < 0)
		return (-1);
	ft_dup2(fd, STDIN_FILENO);
	ft_close(fd);
	return (0);
}

int	reconnect_redir_out(t_redir *redir)
{
	int		fd;

	fd = openfile(redir->file, redir->type);
	if (fd < 0)
		return (-1);
	ft_dup2(fd, STDOUT_FILENO);
	ft_close(fd);
	return (0);
}

int	reconnect_redir_with_stdio(t_command *cmd)
{
	t_redir			*cur_redir;
	t_type_token	type;
	int				fail;

	cur_redir = cmd->redir;
	fail = 0;
	while (cur_redir && cur_redir->file)
	{
		type = cur_redir->type;
		if (type == T_REDIR_OUT || type == T_APPEND)
			fail = reconnect_redir_out(cur_redir);
		else if (type == T_REDIR_IN || type == T_HEREDOC)
			fail = reconnect_redir_in(cur_redir);
		if (fail)
			return (-1);
		cur_redir = cur_redir->next;
	}
	return (0);
}

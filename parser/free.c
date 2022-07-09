/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:19:31 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 17:50:41 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_all_token(t_token *cur)
{
	t_token	*nxt;

	if (cur == NULL)
		return ;
	while (cur && cur->type != T_END)
	{
		nxt = cur->next;
		free(cur->token);
		free(cur);
		cur = nxt;
	}
	free(cur);
}

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->file);
		free(redir->heredoc_eof);
		free(redir);
		redir = tmp;
	}
	free(redir);
}

void	free_command(t_command *cmd)
{
	free_redir(cmd->redir);
	free_all_token(cmd->token);
	if (cmd->argv)
		free_array(cmd->argv);
	free(cmd);
}

void	free_commands(t_command *cmd)
{
	t_command	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_command(cmd);
		cmd = tmp;
	}
	free(cmd);
}

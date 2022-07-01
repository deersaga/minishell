/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:19:31 by katakagi          #+#    #+#             */
/*   Updated: 2022/06/30 13:30:25 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_one_token( \
			t_token **head, t_token *pre, t_token *cur, t_token *next)
{
	if (!pre)
		*head = next;
	else
		pre->next = next;
	free(cur->token);
	free(cur);
}

void	free_all_token(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur && cur->type != T_END)
	{
		free(head->token);
		free(head);
		head = cur->next;
		cur = cur->next;
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
		free(redir);
		redir = tmp;
	}
	free(redir);
}

void	free_command(t_command *cmd)
{
	free_redir(cmd->redir_in);
	free_redir(cmd->redir_out);
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

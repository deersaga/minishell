/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:20:38 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 10:34:32 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_redir(t_command *cmd)
{
	t_redir	*cur;

	cur = cmd->redir_in;
	printf("redir_in\n");
	while (cur)
	{
		printf("file %s fd %d type %d has_qupte %d\n", cur->file,
			cur->fd, cur->type, cur->has_quote);
		cur = cur->next;
	}
	cur = cmd->redir_out;
	printf("redir_out\n");
	while (cur)
	{
		printf("file %s fd %d type %d has_qupte %d\n", cur->file,
			cur->fd, cur->type, cur->has_quote);
		cur = cur->next;
	}
}

void	print_commands(t_mshell *mshell)
{
	t_command	*cur;

	cur = mshell->commands;
	while (cur->token)
	{
		printf("----------------------------\n");
		print_tokens(cur->token);
		print_redir(cur);
		cur = cur->next;
	}
}

void	print_tokens(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur)
	{
		if (cur->token)
			printf("%-20s type: %u  fd %d x %x\n",
				 cur->token, cur->type, cur->fd, cur->token[0]);
		else
			printf("%-20s type: %u  fd %d\n",
				 cur->token, cur->type, cur->fd);
		cur = cur->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:43:13 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/07 21:12:55 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*terminate_command(t_mshell *mshell, t_token *cur, t_command **cmd)
{
	t_token	*next;

	(void)mshell;
	next = cur->next;
	cur->type = T_END;
	free(cur->token);
	cur->token = NULL;
	cur->next = NULL;
	(*cmd)->next = ft_calloc(1, sizeof(t_command));
	*cmd = (*cmd)->next;
	mshell->num_commands += 1;
	return (next);
}

void	split_to_commands(t_mshell *mshell, t_command *cmd)
{
	t_token	*cur;
	t_token	*pre;

	pre = NULL;
	cur = cmd->token;
	while (cur && cur->token)
	{
		if (cur->type == T_PIPE)
		{
			cur = terminate_command(mshell, cur, &cmd);
			cmd->token = cur;
			continue ;
		}
		if (is_redirect_token(cur->type))
		{
			add_redir_info(cmd, cur);
			cur = delete_redir_token(cmd, pre, cur);
			continue ;
		}
		pre = cur;
		cur = cur->next;
	}
	cur = terminate_command(mshell, cur, &cmd);
}

t_token	*format_tokens(t_token *head)
{
	t_token	*pre;
	t_token	*cur;
	t_token	*next;

	pre = NULL;
	cur = head;
	next = cur->next;
	while (cur->token)
	{
		if (cur->type == T_DELM && next->type == T_DELM)
			delete_one_token(&head, pre, cur, next);
		else if (cur->type == T_DELM && ((pre && is_operator_token(pre->type)) \
		|| is_operator_token(next->type)))
			delete_one_token(&head, pre, cur, next);
		else if (all_num(cur->token) && is_redirect_token(next->type))
		{
			next->fd = ft_atoi(cur->token);
			delete_one_token(&head, pre, cur, next);
		}
		else
			pre = cur;
		cur = next;
		next = cur->next;
	}
	return (head);
}

int	parser(t_mshell *mshell, char *cmdline)
{
	t_token		*head;
	t_command	*cmd;

	mshell->num_commands = 0;
	head = tokenizer(cmdline);
	head = format_tokens(head);
	if (check_syntax(head))
	{
		free_all_token(head);
		ft_putstr_fd("syntax error\n", 2);
		return (258);
	}
	mshell->commands = ft_calloc(1, sizeof(t_command));
	cmd = mshell->commands;
	cmd->token = head;
	split_to_commands(mshell, mshell->commands);
	return (0);
}

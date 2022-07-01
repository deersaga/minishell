/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:21:42 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 10:34:24 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redir	*get_redir_last(t_command *cmd, e_type_token type)
{
	t_redir	*last;

	if (type == T_REDIR_IN || type == T_HEREDOC)
	{
		if (!cmd->redir_in)
			cmd->redir_in = ft_calloc(1, sizeof(t_redir));
		last = cmd->redir_in;
	}
	else
	{
		if (!cmd->redir_out)
			cmd->redir_out = ft_calloc(1, sizeof(t_redir));
		last = cmd->redir_out;
	}
	while (last->next)
		last = last->next;
	return (last);
}

/*int	get_fd(t_token *op)
{
	if (op->fd != 0)
		return (op->fd);
	else if (op->type == T_REDIR_IN)
		return (STDIN_FILENO);
	else if (op->type == T_REDIR_OUT)
		return (STDOUT_FILENO);
	else if (op->type == T_APPEND)
		return (STDOUT_FILENO);
	else if (op->type == T_HEREDOC)
		return (STDIN_FILENO);
	return (-1);
}*/

int	has_quote(t_token *start, t_token *end)
{
	t_token	*cur;

	cur = start;
	while (cur != end)
	{
		if (cur->type == T_DQUOTE || cur->type == T_SQUOTE)
			return (1);
		cur = cur->next;
	}
	return (0);
}

//last->fd = get_fd(op);は対応しなくても良さそうなので外した。
void	new_redir(t_command *cmd, t_token *op, e_type_token type)
{
	t_redir	*last;
	t_token	*file;
	t_token	*end;

	file = op->next;
	end = skip_word_quote_token(file);
	last = get_redir_last(cmd, type);
	last->type = type;
	last->has_quote = has_quote(file, end);
	last->file = subtoken(file, end);
	last->next = ft_calloc(1, sizeof(t_redir));
}

void	add_redir_info(t_command *cmd, t_token *cur)
{
	if (cur->type == T_REDIR_IN)
		new_redir(cmd, cur, cur->type);
	else if (cur->type == T_REDIR_OUT)
		new_redir(cmd, cur, cur->type);
	else if (cur->type == T_APPEND)
		new_redir(cmd, cur, cur->type);
	else if (cur->type == T_HEREDOC)
		new_redir(cmd, cur, cur->type);
}

t_token	*delete_redir_token(t_command *cmd, t_token *previous, t_token *current)
{
	t_token	*end;
	t_token	*cur;
	t_token	*pre;

	end = skip_word_quote_token(current->next);
	pre = previous;
	cur = current;
	while (cur != end)
	{
		delete_one_token(&(cmd->token), pre, cur, cur->next);
		cur = pre->next;
	}
	return (cur);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:21:42 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 17:51:06 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_redir	*get_redir_last(t_command *cmd)
{
	t_redir	*last;

	if (!cmd->redir)
		cmd->redir = ft_calloc(1, sizeof(t_redir));
	last = cmd->redir;
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

/*
heredocは一時的に下のようなファイルに入力を書き込み
.heredoc_tmp_(heredoc_id)
以後そのファイルへのridirect_inとして受け取る
cat <<aa <<aa | cat <<cc <<dd
でheredoc_idはそれぞれ下のようになる
一つ目(aa):0
二つ目(aa):1
三つ目(cc):2
四つ目(dd):3
初期化していないので、続けて下のようにすると
cat <<bb <<aa | cat <<ee <<ff
一つ目(bb):4
二つ目(aa):5
三つ目(ee):6
四つ目(ff):7
になる
*/
/*void	new_redir(t_command *cmd, t_token *op, t_type_token type)
{
	t_redir			*last;
	t_token			*file;
	t_token			*end;
	static size_t	heredoc_id;

	file = op->next;
	last = get_redir_last(cmd);
	last->type = type;
	end = skip_word_quote_token(file);
	last->has_quote = has_quote(file, end);
	last->file = subtoken(file, end);
	last->next = ft_calloc(1, sizeof(t_redir));
	if (type == T_HEREDOC)
	{
		last->heredoc_eof = last->file;
		last->file = make_heredoc_filename(heredoc_id);
		heredoc_id++;
	}
}*/

//last->fd = get_fd(op);は対応しなくても良さそうなので外した。
void	add_redir_info(t_command *cmd, t_token *op)
{	
	t_redir			*last;
	t_token			*file;
	t_token			*end;
	static size_t	heredoc_id;

	file = op->next;
	last = get_redir_last(cmd);
	last->type = op->type;
	end = skip_word_quote_token(file);
	last->has_quote = has_quote(file, end);
	last->file = subtoken(file, end);
	last->next = ft_calloc(1, sizeof(t_redir));
	if (op->type == T_HEREDOC)
	{
		last->heredoc_eof = last->file;
		last->file = make_heredoc_filename(heredoc_id);
		heredoc_id++;
	}
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
		if (pre == NULL)
			cur = cmd->token;
		else
			cur = pre->next;
	}
	return (cur);
}

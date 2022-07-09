/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:51:46 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:31:57 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redirect_token(t_type_token type)
{
	if (type == T_REDIR_IN || type == T_REDIR_OUT)
		return (1);
	if (type == T_APPEND || type == T_HEREDOC)
		return (1);
	return (0);
}

int	is_operator_token(t_type_token type)
{
	if (type == T_REDIR_IN || type == T_REDIR_OUT)
		return (1);
	if (type == T_APPEND || type == T_HEREDOC)
		return (1);
	if (type == T_PIPE)
		return (1);
	return (0);
}

int	is_redir(t_type_token	type)
{
	if (type == T_REDIR_IN || type == T_REDIR_OUT \
	|| type == T_APPEND || type == T_HEREDOC)
		return (1);
	return (0);
}

t_type_token	get_token_type(char	*cur, size_t *i)
{
	if (cur[0] == '>' && cur[1] == '>')
	{
		(*i)++;
		return (T_APPEND);
	}
	else if (cur[0] == '<' && cur[1] == '<')
	{
		(*i)++;
		return (T_HEREDOC);
	}
	if (cur[0] == '>')
		return (T_REDIR_OUT);
	else if (cur[0] == '<')
		return (T_REDIR_IN);
	else if (cur[0] == '|')
		return (T_PIPE);
	else if (cur[0] == '\'')
		return (T_SQUOTE);
	else if (cur[0] == '\"')
		return (T_DQUOTE);
	else if (is_delimiter(cur[0]))
		return (T_DELM);
	return (T_WORD);
}

void	get_quote_type_and_len( \
			char *cmdline, size_t i, size_t *len, t_type_token *type)
{
	char	*quote_pos;

	quote_pos = 0;
	if (cmdline[i] == '\'')
	{
		quote_pos = ft_strchr(&cmdline[i + 1], '\'');
		*len = quote_pos - &cmdline[i + 1];
		*type = T_SQUOTE;
	}
	else if (cmdline[i] == '\"')
	{
		quote_pos = ft_strchr(&cmdline[i + 1], '\"');
		*len = quote_pos - &cmdline[i + 1];
		*type = T_DQUOTE;
	}
	if (!quote_pos)
	{
		*len = ft_strlen(cmdline);
		*type = T_ERR;
	}
}

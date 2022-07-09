/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_ctrl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:47:03 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:31:56 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*skip_delimiter_token(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur->type == T_DELM && cur->token != NULL)
		cur = cur->next;
	return (cur);
}

t_token	*skip_by_next_delimiter_token(t_token *token)
{
	t_token	*cur;

	cur = token;
	while (cur->type != T_DELM && cur->token != NULL)
		cur = cur->next;
	return (cur);
}

t_token	*skip_word_quote_token(t_token *token)
{
	t_token	*cur;

	cur = token;
	while ((cur->type == T_WORD || cur->type == T_SQUOTE \
		|| cur->type == T_DQUOTE) && cur->token != NULL)
	{
		cur = cur->next;
	}
	return (cur);
}

t_token	*get_first_non_delimiter_token(t_token *head)
{
	t_token	*cur;

	cur = head;
	return (skip_delimiter_token(cur));
}

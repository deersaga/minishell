/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:26:48 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/06 08:02:21 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_syntax(t_token *head)
{
	t_token			*cur;
	t_token			*next;
	t_type_token	type;

	cur = head;
	if (cur->type == T_PIPE)
		return (1);
	while (cur->token)
	{
		next = cur->next;
		type = cur->type;
		if (type == T_ERR)
			return (1);
		else if (type == T_PIPE && next->type == T_PIPE)
			return (1);
		else if (is_redirect_token(type) && (is_redirect_token(next->type) \
		|| next->type == T_PIPE))
			return (1);
		else if ((is_redirect_token(type) || type == T_PIPE) && !next->token)
			return (1);
		cur = next;
	}
	return (0);
}

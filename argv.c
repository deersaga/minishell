/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:00:44 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:43:06 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_size(t_token *head)
{
	size_t	size;
	t_token	*cur;
	int		flag;

	size = 0;
	flag = 0;
	cur = head;
	while (1)
	{
		if (cur->type == T_DELM || !cur->token)
		{
			if (flag == 1)
				size++;
			if (!cur->token)
				break ;
			flag = 0;
		}
		else
			flag = 1;
		cur = cur->next;
	}
	return (size);
}

void	create_argv(t_mshell *mshell, t_command *cmd)
{
	size_t	len;
	size_t	i;
	t_token	*cur;
	char	**argv;

	(void)mshell;
	len = get_size(cmd->token);
	cmd->argc = len;
	argv = ft_calloc(len + 1, sizeof(char *));
	i = 0;
	cur = cmd->token;
	while (i < len)
	{
		cur = skip_delimiter_token(cur);
		argv[i] = concat_tokens(cur);
		cur = skip_by_next_delimiter_token(cur);
		i++;
	}
	argv[i] = NULL;
	cmd->argv = argv;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:00:44 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 13:58:02 by katakagi         ###   ########.fr       */
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

/*int main(int argc, char **argv)
{
	t_token	*head;
	t_mshell mshell;
	t_token	*cur;

	init_env(&mshell);
	register_or_update_env(&mshell, "test", "cho           hello 3<3test");
	register_or_update_env(&mshell, "test1", "sekai");
	parser(&mshell, "$test   \nhello world  \t kon  '$PWD'\"hi\" \t\n a   \t\n");
	mshell.commands->token = expand_and_retokenize(&mshell, mshell.commands->token);
	//cur = head;
	print_tokens(mshell.commands->token);
	argv = create_argv(&mshell, mshell.commands);
	while (*argv)
	{
		printf("argv %s\n", *argv);
		argv++;
	}
	//printf("%zu\n", get_size(mshell.commands->token));
}*/
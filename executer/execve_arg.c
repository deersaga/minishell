/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/10 13:54:34 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/10 14:24:17 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	get_argv_size(t_token *head)
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
	len = get_argv_size(cmd->token);
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

static size_t	get_environ_size(t_envList *head)
{
	size_t		size;
	t_envList	*cur;

	cur = head;
	size = 0;
	while (cur)
	{
		size++;
		cur = cur->next;
	}
	return (size);
}

char	**make_environ(t_mshell *mshell)
{
	t_envList	*cur;
	size_t		i;
	char		*tmp;
	char		**env;

	i = get_environ_size(mshell->env);
	env = ft_calloc(i + 1, sizeof(char *));
	cur = mshell->env;
	i = 0;
	while (cur->key)
	{
		env[i] = ft_strdup(cur->key);
		if (cur->val)
		{
			tmp = env[i];
			env[i] = ft_strjoin(env[i], "=");
			free(tmp);
			tmp = env[i];
			env[i] = ft_strjoin(env[i], cur->val);
			free(tmp);
		}
		i++;
		cur = cur->next;
	}
	return (env);
}

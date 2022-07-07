/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_for_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:22:20 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/07 20:59:09 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_envList	*copy_env(t_envList *env)
{
	t_envList	*copy;
	t_envList	*cur;
	t_envList	*copy_cur;

	copy = ft_calloc(1, sizeof(t_envList));
	copy_cur = copy;
	cur = env;
	while (cur && cur->key)
	{
		copy_cur->key = ft_strdup(cur->key);
		if (cur->val)
			copy_cur->val = ft_strdup(cur->val);
		cur = cur->next;
		copy_cur->next = ft_calloc(1, sizeof(t_envList));
		copy_cur = copy_cur->next;
	}
	return (copy);
}

static size_t	get_env_size(t_envList *head)
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

	i = get_env_size(mshell->env);
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

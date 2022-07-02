/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:46:48 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/02 10:27:58 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_env(t_envList *head)
{
	t_envList	*cur;
	t_envList	*tmp;
	char		*key;
	char		*val;

	cur = head;
	while (cur->next->key)
	{
		tmp = cur->next;
		while (tmp->key)
		{
			if (ft_strcmp(cur->key, tmp->key) > 0)
			{
				key = cur->key;
				val = cur->val;
				cur->key = tmp->key;
				cur->val = tmp->val;
				tmp->key = key;
				tmp->val = val;
			}
			tmp = tmp->next;
		}
		cur = cur->next;
	}
}

void	print_env(t_envList *env)
{
	t_envList	*cur;
	char		*val;

	(void)val;
	cur = env;
	while (cur->key)
	{
		if (cur->val)
			printf("%s=%s\n", cur->key, cur->val);
		cur = cur->next;
	}
}

void	delete_all_env(t_mshell *mshell)
{
	t_envList	*cur;
	t_envList	*next;

	cur = mshell->env;
	next = cur->next;
	while (cur->key)
	{
		free(cur->key);
		free(cur->val);
		free(cur);
		cur = next;
		next = cur->next;
	}
	free(cur);
}

void	delete_one_env(t_mshell *mshell, char *del_key)
{
	t_envList	*cur;
	t_envList	*pre;

	pre = NULL;
	cur = mshell->env;
	while (cur->key)
	{
		if (!ft_strcmp(cur->key, del_key))
		{
			if (pre == NULL)
				mshell->env = cur->next;
			else
				pre->next = cur->next;
			free(cur->key);
			free(cur->val);
			free(cur);
			return ;
		}
		pre = cur;
		cur = cur->next;
	}
}

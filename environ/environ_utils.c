/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:46:48 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 15:51:36 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sort_env(t_envList *head)
{
	t_envList	*cur;
	t_envList	*comp;
	char		*key;
	char		*val;

	cur = head;
	while (cur->next->key)
	{
		comp = cur->next;
		while (comp->key)
		{
			if (ft_strcmp(cur->key, comp->key) > 0)
			{
				key = cur->key;
				val = cur->val;
				cur->key = comp->key;
				cur->val = comp->val;
				comp->key = key;
				comp->val = val;
			}
			comp = comp->next;
		}
		cur = cur->next;
	}
}

void	print_env(t_envList *env)
{
	t_envList	*cur;

	sort_env(env);
	cur = env;
	while (cur->key)
	{
		printf("%s", cur->key);
		if (cur->val)
			printf("=%s", cur->val);
		printf("\n");
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

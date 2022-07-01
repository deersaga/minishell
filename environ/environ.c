/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:54:04 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 11:55:08 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
* envについて
* envListの終端にenvList.keyがNULLのものを置くことで終端目印としている。
*/

void	get_key_val(char *key_eq_val, char *key_val[2])
{
	char	*eq;

	eq = ft_strchr(key_eq_val, '=');
	if (!eq)
	{
		key_val[0] = ft_strdup(key_eq_val);
		key_val[1] = NULL;
	}
	else
	{
		key_val[0] = ft_substr(key_eq_val, 0, eq - key_eq_val);
		key_val[1] = ft_substr(eq + 1, 0, ft_strlen(key_eq_val));
	}
}

char	*get_env(t_mshell *mshell, char *key)
{
	t_envList	*cur;

	cur = mshell->env;
	while (cur->key)
	{
		if (!ft_strcmp(cur->key, key))
			return (cur->val);
		cur = cur->next;
	}
	return (NULL);
}

void	register_or_update_env(t_mshell *mshell, char *tar_key, char *tar_val)
{
	t_envList	*cur;

	cur = mshell->env;
	while (cur->key)
	{
		if (!ft_strcmp(cur->key, tar_key))
		{
			if (tar_val)
			{
				free(cur->val);
				cur->val = ft_strdup(tar_val);
			}
			return ;
		}
		cur = cur->next;
	}
	cur->key = ft_strdup(tar_key);
	if (tar_val != NULL)
		cur->val = ft_strdup(tar_val);
	cur->next = (t_envList *)malloc(sizeof(t_envList));
	if (!cur->next)
		exit(EXIT_FAILURE);
	cur->next->key = NULL;
	cur->next->val = NULL;
}

void	init_env(t_mshell *mshell)
{
	extern char	**environ;
	char		*key_val[2];
	size_t		i;
	t_envList	*cur;

	i = 0;
	mshell->env = ft_calloc(1, sizeof(t_envList));
	cur = mshell->env;
	while (environ[i])
	{
		get_key_val(environ[i], key_val);
		cur->key = key_val[0];
		cur->val = key_val[1];
		cur->next = ft_calloc(1, sizeof(t_envList));
		cur = cur->next;
		i++;
	}
	register_or_update_env(mshell, "OLDPWD", "");
}

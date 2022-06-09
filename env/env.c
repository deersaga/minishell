#include "../minishell.h"

/*
* envについて
* envListの終端にenvList.keyがNULLのものを置くことで終端目印としている。
* 登録するvalがexpandしなきゃいけない値の場合は未対応
*/
void	print_env(t_envList *env)
{
	t_envList	*cur;

	cur = env;
	while (cur->key)
	{
		printf("key %s val %s\n", cur->key, cur->val);
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

void	init_env(t_mshell *mshell)
{
	extern char	**environ;
	char		*eq_pos;
	size_t		i;
	t_envList	*cur;

	i = 0;
	mshell->env = (t_envList *)malloc(sizeof(t_envList));
	if (!mshell->env)
		exit(EXIT_FAILURE);
	cur = mshell->env;
	while (environ[i])
	{
		eq_pos = ft_strchr(environ[i], '=');
		cur->key = ft_substr(environ[i], 0, eq_pos - environ[i]);
		cur->val = ft_substr(eq_pos + 1, 0, ft_strlen(environ[i]));
		cur->next = (t_envList *)malloc(sizeof(t_envList));
		if (!cur->next)
			exit(EXIT_FAILURE);
		cur = cur->next;
		i++;
	}
	cur->key = NULL;
	register_or_update_env(mshell, "OLDPWD", "");
}

void	free_env_node(t_envList *env)
{
	free(env->key);
	free(env->val);
	free(env);
}

void	delete_one_env(t_mshell *mshell, char *del_key)
{
	t_envList	*cur;
	t_envList	*pre;

	pre = mshell->env;
	cur = pre->next;
	if (!ft_strcmp(pre->key, del_key))
	{
		free_env_node(pre);
		mshell->env = cur;
		return ;
	}
	while (cur->key)
	{
		if (!ft_strcmp(cur->key, del_key))
		{
			pre->next = cur->next;
			free_env_node(cur);
			return ;
		}
		pre = cur;
		cur = cur->next;
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
			free(cur->val);
			cur->val = ft_strdup(tar_val);
			return ;
		}
		cur = cur->next;
	}
	cur->key = ft_strdup(tar_key);
	cur->val = ft_strdup(tar_val);
	cur->next = (t_envList *)malloc(sizeof(t_envList));
	if (!cur->next)
		exit(EXIT_FAILURE);
	cur->next->key = NULL;
}
/*
int	main(void)
{
	t_mshell	mshell;

	init_env(&mshell);
	register_or_update_env(&mshell, "TEST", "hello");
	print_env(mshell.env);
	printf("----------------------\n");
	delete_one_env(&mshell, "COMMAND_MODE");
	print_env(mshell.env);
	delete_all_env(&mshell);
}
*/
/*__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:55:27 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/13 17:18:58 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*static t_envList	*copy_env(t_envList *env)
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
}*/

static void	get_key_val_env(char *key_eq_val, char *key_val[2])
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

ssize_t	update_mshell(t_mshell *mshell, t_command *cmd)
{
	size_t	i;
	char	*key_val[2];

	mshell->num_commands = 0;
	i = 1;
	while (cmd->argv[i] && ft_strchr(cmd->argv[i], '='))
	{
		if (*(cmd->argv[i]) == '=')
		{
			ft_putstr_fd("env: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[i], STDERR_FILENO);
			ft_putstr_fd(": Invalid argument\n", STDERR_FILENO);
			return (-1);
		}
		get_key_val_env(cmd->argv[i], key_val);
		register_or_update_env(mshell, key_val[0], key_val[1]);
		free(key_val[0]);
		free(key_val[1]);
		i++;
	}
	return (i);
}

int	ft_env(t_mshell *mshell, t_command *cmd)
{
	ssize_t		i;
	pid_t		pid;
	char		*path;

	create_argv(mshell, cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		i = update_mshell(mshell, cmd);
		if (i == -1)
			exit(EXIT_FAILURE);
		if (cmd->argv[i] == NULL)
			print_env(mshell->env);
		else
		{
			path = get_cmd_path(mshell, cmd->argv[i]);
			ft_execve(path, &(cmd->argv[i]), make_environ(mshell));
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	wait_childs(mshell, pid);
	return (mshell->exit_status);
}

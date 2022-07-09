/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:55:27 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:51:07 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	update_mshell(t_mshell *mshell, t_command *cmd)
{
	size_t	i;
	char	*key_val[2];

	mshell->num_commands = 0;
	i = 1;
	while (cmd->argv[i] && ft_strchr(cmd->argv[i], '='))
	{
		get_key_val(cmd->argv[i], key_val);
		register_or_update_env(mshell, key_val[0], key_val[1]);
		free(key_val[0]);
		free(key_val[1]);
		i++;
	}
	return (i);
}

int	ft_env(t_mshell *mshell, t_command *cmd)
{
	size_t		i;
	pid_t		pid;
	char		*path;

	create_argv(mshell, cmd);
	pid = ft_fork();
	if (pid == 0)
	{
		i = update_mshell(mshell, cmd);
		if (cmd->argv[i] == NULL)
			print_env(mshell->env);
		else
		{
			path = get_cmd_path(mshell, cmd->argv[i]);
			ft_execve(path, &(cmd->argv[i]), make_environ(mshell));
		}
		exit(EXIT_SUCCESS);
	}
	wait_childs(mshell);
	return (mshell->exit_status);
}

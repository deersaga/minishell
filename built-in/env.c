/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:55:27 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 12:57:15 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	create_new_mshell(t_mshell *origin, t_mshell *copy, t_command *cmd)
{
	size_t	i;
	char	*key_val[2];

	copy->env = copy_env(origin->env);
	copy->num_commands = 0;
	i = 1;
	while (cmd->argv[i] && ft_strchr(cmd->argv[i], '='))
	{
		get_key_val(cmd->argv[i], key_val);
		register_or_update_env(copy, key_val[0], key_val[1]);
		free(key_val[0]);
		free(key_val[1]);
		i++;
	}
}

int	ft_env(t_mshell *mshell, t_command *cmd)
{
	t_mshell	copy;
	size_t		i;
	pid_t		pid;
	char		**env;
	char		*path;

	create_argv(mshell, cmd);
	pid = fork();
	i = 1;
	if (pid == 0)
	{
		create_new_mshell(mshell, &copy, cmd);
		env = make_environ(&copy);
		if (cmd->argv[i] == NULL)
		{
			print_env(copy.env);
			exit(0);
		}
		else
		{
			path = get_cmd_path(&copy, cmd->argv[i]);
			free(cmd->argv[i]);
			cmd->argv[i] = path;
			execve(cmd->argv[i], &(cmd->argv[i]), env);
			exit(127);
		}
	}
	return (0);
}

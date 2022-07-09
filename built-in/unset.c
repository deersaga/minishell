/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:27 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:30:36 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid(char *key)
{
	size_t	i;

	if (!(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 0;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_unset(t_mshell *mshell, t_command *cmd)
{
	size_t	i;
	int		status;

	create_argv(mshell, cmd);
	status = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid(cmd->argv[i]))
		{
			ft_putstr_fd("unset: ", STDERR_FILENO);
			ft_putstr_fd(cmd->argv[i], STDERR_FILENO);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
			status = EXIT_FAILURE;
		}
		else
			delete_one_env(mshell, cmd->argv[i]);
		i++;
	}
	return (status);
}

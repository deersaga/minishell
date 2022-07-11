/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 12:36:35 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/11 22:23:28 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(t_mshell *mshell, t_command *cmd)
{
	bool		error;
	long long	exit_value;

	create_argv(mshell, cmd);
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (cmd->argv[1] == NULL)
		exit(EXIT_SUCCESS);
	error = false;
	exit_value = ft_try_atoll(cmd->argv[1], &error);
	if (error)
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (cmd->argc == 2)
		exit(exit_value % 256);
	ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
	return (1);
}

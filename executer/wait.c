/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:36:50 by ktada             #+#    #+#             */
/*   Updated: 2022/07/13 14:05:15 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_childs(t_mshell *mshell, pid_t pid)
{
	pid_t	tmp;
	int		status;

	while (1)
	{
		tmp = wait(&status);
		if (tmp == -1)
			break;
		if (status == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if (status == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		if (pid == tmp && (status == SIGINT || status == SIGQUIT))
			mshell->exit_status = 128 + status;
		else if (pid == tmp)
			mshell->exit_status = WEXITSTATUS(status);
	}
	/*if (mshell->exit_status == SIGINT)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		mshell->exit_status = 128 + mshell->exit_status;
	}
	else if (mshell->exit_status == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		mshell->exit_status = 128 + mshell->exit_status;
	}
	else
		mshell->exit_status = WEXITSTATUS(mshell->exit_status);*/
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:36:50 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:43:15 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_childs(t_mshell *mshell)
{
	int		status;

	while (wait(&status) > 0)
		;
	if (status == SIGINT)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		mshell->exit_status = 128 + status;
	}
	else if (status == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		mshell->exit_status = 128 + status;
	}
	else
		mshell->exit_status = WEXITSTATUS(status);
}

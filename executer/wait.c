/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:36:50 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 16:37:08 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	wait_childs(t_mshell *mshell)
{
	int	status;

	while (wait(&status) >= 0)
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

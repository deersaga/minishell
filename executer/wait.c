/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:36:50 by ktada             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/07/09 18:43:15 by katakagi         ###   ########.fr       */
=======
/*   Updated: 2022/07/09 18:34:16 by ktada            ###   ########.fr       */
>>>>>>> 334f157f5c1137db7fb1e380be1e52c241ce5b36
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

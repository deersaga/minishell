/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:22 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:37:47 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_mshell *mshell, t_command *cmd)
{
	char	*pwd;

	(void)cmd;
	pwd = mshell->info.pwd;
	if (*pwd == '\0')
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			perror("getcwd");
			return (EXIT_FAILURE);
		}
		ft_putstr_fd(pwd, STDOUT_FILENO);
		free(pwd);
	}
	else
		ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

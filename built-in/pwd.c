/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:22 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/07 12:54:44 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_mshell *mshell, t_command *cmd)
{
	char	*pwd;

	(void)cmd;
	(void)mshell;
	pwd = get_env(mshell, "PWD");
	if (!pwd)
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

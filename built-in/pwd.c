/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:22 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 13:58:29 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_mshell *mshell, t_command *cmd)
{
	char	*pwd;

	(void)cmd;
	(void)mshell;
	pwd = getcwd(NULL, 0);
	if (!pwd || *pwd == '\0')
	{
		perror("getcwd");
		return (EXIT_FAILURE);
	}
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(pwd);
	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:22 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 16:15:25 by kaou             ###   ########.fr       */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 12:36:35 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/10 20:54:00 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	all_signed_num(char *s)
{
	size_t	i;

	i = 0;
	if (*s == '\0')
		return (0);
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_mshell *mshell, t_command *cmd)
{
	create_argv(mshell, cmd);
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (cmd->argv[1] == NULL)
		exit(EXIT_SUCCESS);
	if (!all_signed_num(cmd->argv[1]))
	{
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (cmd->argc == 2)
		exit(ft_atol(cmd->argv[1]) % 256);
	ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
	return (1);
}

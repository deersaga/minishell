/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 13:14:27 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/07 20:59:10 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_unset(t_mshell *mshell, t_command *cmd)
{
	size_t	i;

	i = 1;
	create_argv(mshell, cmd);
	while (cmd->argv[i])
	{
		delete_one_env(mshell, cmd->argv[i]);
		i++;
	}
	return (0);
}

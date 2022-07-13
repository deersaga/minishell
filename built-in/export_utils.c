/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 13:39:19 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/13 15:54:14 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	append_lefthand_side( \
	t_mshell *mshell, t_command *cmd, size_t i, char *key_val[2])
{
	char		*eq;
	char		*tmp;
	char		*env_val;

	eq = ft_strchr(cmd->argv[i], '=');
	if (eq && *(eq - 1) == '+')
	{
		tmp = key_val[1];
		env_val = get_env(mshell, key_val[0]);
		if (env_val == NULL)
			key_val[1] = ft_strjoin("", key_val[1]);
		else
			key_val[1] = ft_strjoin(env_val, key_val[1]);
		free(tmp);
	}
}

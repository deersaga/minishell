/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 16:30:19 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:34:16 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_cmd_name(t_token *head)
{
	t_token	*cur;
	char	*cmd;

	cur = head;
	cur = skip_delimiter_token(cur);
	cmd = concat_tokens(cur);
	return (cmd);
}

int	is_builtin_cmd(t_mshell *mshell, t_command *cmd)
{
	char	*cmd_name;

	(void)mshell;
	cmd_name = get_cmd_name(cmd->token);
	if (!ft_strcmp("cd", cmd_name) \
		|| !ft_strcmp("echo", cmd_name) \
		|| !ft_strcmp("pwd", cmd_name) \
		|| !ft_strcmp("export", cmd_name) || !ft_strcmp("unset", cmd_name) \
		|| !ft_strcmp("env", cmd_name) \
		|| !ft_strcmp("exit", cmd_name))
	{
		free(cmd_name);
		return (1);
	}
	free(cmd_name);
	return (0);
}

int	is_export_cmd(t_token *head)
{
	char	*cmd;

	cmd = get_cmd_name(head);
	if (!ft_strcmp("export", cmd))
	{
		free(cmd);
		return (1);
	}
	free(cmd);
	return (0);
}

int	check_minishell(t_mshell *mshell, t_command *cmd)
{
	char		*cmdname;
	struct stat	buff;
	int			flag;

	flag = 0;
	cmdname = get_cmd_name(cmd->token);
	stat(cmdname, &buff);
	if (mshell->info.mshell_inode == buff.st_ino)
		flag = 1;
	free(cmdname);
	return (flag);
}

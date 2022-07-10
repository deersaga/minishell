/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_a_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:12:42 by ktada             #+#    #+#             */
/*   Updated: 2022/07/10 17:20:25 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_a_builtin(t_mshell *mshell, t_command *cmd)
{
	char	*cmd_name;
	int		status;

	status = 1;
	cmd_name = get_cmd_name(cmd->token);
	if (!ft_strcmp(cmd_name, "cd"))
		status = ft_cd(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "pwd"))
		status = ft_pwd(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "echo"))
		status = ft_echo(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "export"))
		status = ft_export(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "unset"))
		status = ft_unset(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "env"))
		status = ft_env(mshell, cmd);
	else if (!ft_strcmp(cmd_name, "exit"))
		status = ft_exit(mshell, cmd);
	free(cmd_name);
	return (status);
}

int	execute_a_builtin_cmd(t_mshell *mshell, t_command *cmd)
{
	int		status;
	int		dup_stdio_fd[2];

	status = 0;
	dup_stdio_fd[0] = ft_dup(0);
	dup_stdio_fd[1] = ft_dup(1);
	if (reconnect_redir_with_stdio(cmd) != -1)
		status = execute_a_builtin(mshell, cmd);
	else
		status = 1;
	ft_close(STDIN_FILENO);
	ft_close(STDOUT_FILENO);
	ft_dup2(dup_stdio_fd[0], STDIN_FILENO);
	ft_dup2(dup_stdio_fd[1], STDOUT_FILENO);
	ft_close(dup_stdio_fd[0]);
	ft_close(dup_stdio_fd[1]);
	return (status);
}

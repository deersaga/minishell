/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_a_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:12:42 by kaou              #+#    #+#             */
/*   Updated: 2022/07/09 14:07:03 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

extern char	**environ;

void	print_array(char **array)
{
	size_t	i;

	i = 0;
	if (!array)
	{
		printf("array = NULL\n");
		return ;
	}
	while (array[i])
	{
		printf("argv%zu %s\n", i, array[i]);
		i++;
	}
}

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

int	execute_a_add_on(t_mshell *mshell, t_command *cmd)
{
	pid_t		pid;
	char		*path;
	extern char	**environ;

	pid = fork();
	if (pid == 0)
	{
		create_argv(mshell, cmd);
		path = get_cmd_path(mshell, cmd->argv[0]);
		environ = make_environ(mshell);
		print_array(environ);
		ft_execve(path, cmd->argv, environ);
		free_commands(mshell->commands);
		exit(1);
	}
	return (0);
}

int	execute_a_builtin_cmd(t_mshell *mshell, t_command *cmd)
{
	int		status;
	int		dup_stdio_fd[2];

	status = 0;
	dup_stdio_fd[0] = dup(0);
	dup_stdio_fd[1] = dup(1);
	reconnect_redir_with_stdio(cmd);
	status = execute_a_builtin(mshell, cmd);
	ft_close(STDIN_FILENO);
	ft_close(STDOUT_FILENO);
	ft_dup2(dup_stdio_fd[0], STDIN_FILENO);
	ft_dup2(dup_stdio_fd[1], STDOUT_FILENO);
	ft_close(dup_stdio_fd[0]);
	ft_close(dup_stdio_fd[1]);
	return (status);
}

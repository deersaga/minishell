/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:54 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/10 20:40:28 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

bool	is_valid_cmdline(t_mshell *mshell, char *cmdline)
{
	ft_signal(SIGINT, SIG_DFL);
	if (!cmdline)
	{
		delete_all_env(mshell);
		free(mshell->info.pwd);
		free(mshell->info.home);
		free(mshell->info.shlvl);
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(0);
	}
	else if (!ft_strcmp(cmdline, ""))
		return (false);
	else
		return (true);
}

static void	exit_with_errmsg(char *cmd, int permission_denied)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (permission_denied)
	{
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit(126);
	}
	else
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
}

char	*search_cmd_path(char **paths, char *cmd)
{
	size_t		i;
	struct stat	buff;
	char		*cand;
	char		*tmp;
	int			permission_denied;

	i = 0;
	permission_denied = 0;
	if (*cmd == '\0')
		exit_with_errmsg(cmd, permission_denied);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i++], "/");
		cand = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(cand, X_OK))
		{
			free_array(paths);
			return (cand);
		}
		else if (stat(cand, &buff) == 0)
			permission_denied = 1;
		free(cand);
	}
	free_array(paths);
	exit_with_errmsg(cmd, permission_denied);
	return (NULL);
}

char	*get_cmd_path(t_mshell *mshell, char *cmd)
{
	char		**paths;
	int			permission_denied;
	struct stat	buff;

	permission_denied = 0;
	if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3) || *cmd == '/')
	{
		if (!access(cmd, X_OK))
			return (ft_strdup(cmd));
		ft_putstr_fd(cmd, STDERR_FILENO);
		if (stat(cmd, &buff) == 0)
			permission_denied = 1;
		exit_with_errmsg(cmd, permission_denied);
	}
	paths = ft_split(get_env(mshell, "PATH"), ':');
	if (!paths)
		exit(EXIT_FAILURE);
	return (search_cmd_path(paths, cmd));
}

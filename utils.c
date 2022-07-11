/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:54 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/11 20:54:03 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*concat_cmd_with_path(char *path, char *cmd)
{
	size_t	len;
	char	*ret;
	char	*slash_terminated;

	len = ft_strlen(path);
	if (path[len - 1] != '/')
		slash_terminated = ft_strjoin(path, "/");
	else
		slash_terminated = ft_strdup(path);
	ret = ft_strjoin(slash_terminated, cmd);
	free(slash_terminated);
	return (ret);
}

char	*search_cmd_path(t_mshell *mshell, char ***paths, char *cmd)
{
	size_t		i;
	struct stat	buff;
	char		*cand;
	int			permission_denied;

	i = 0;
	permission_denied = 0;
	if (*cmd == '\0')
		exit_with_errmsg(cmd, permission_denied);
	if (*paths == NULL)
		*paths = ft_split(mshell->info.pwd, '\0');
	while ((*paths)[i])
	{
		cand = concat_cmd_with_path((*paths)[i++], cmd);
		if (!access(cand, X_OK))
			return (cand);
		else if (stat(cand, &buff) == 0)
			permission_denied = 1;
		free(cand);
	}
	exit_with_errmsg(cmd, permission_denied);
	return (NULL);
}

char	*get_cmd_path(t_mshell *mshell, char *cmd)
{
	char		**paths;
	char		*cand;
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
	cand = search_cmd_path(mshell, &paths, cmd);
	free_array(paths);
	return (cand);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:55:20 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:37:47 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_special_path(t_mshell *mshell, char *argv1)
{
	if (!argv1)
		return (ft_strdup(mshell->info.home));
	else if (*argv1 == '~')
		return (ft_strjoin(mshell->info.home, &argv1[1]));
	return (ft_strdup(argv1));
}

static char	*get_simple_path(t_mshell *mshell, char *argv1)
{
	char	*tmp;
	char	*path;

	if (mshell->info.pwd[0] != '/')
	{
		tmp = mshell->info.pwd;
		mshell->info.pwd = getcwd(NULL, 0);
		if (!mshell->info.pwd)
			mshell->info.pwd = tmp;
		else
			free(tmp);
	}
	tmp = mshell->info.pwd;
	if (*tmp == '\0' || tmp[ft_strlen(tmp) - 1] == '/')
		tmp = ft_strdup(mshell->info.pwd);
	else
		tmp = ft_strjoin(mshell->info.pwd, "/");
	path = ft_strjoin(tmp, argv1);
	free(tmp);
	return (path);
}

static int	get_path(char **argv, char **path, t_mshell *mshell)
{
	if (!argv[1] || *argv[1] == '~' || *argv[1] == '/')
		*path = get_special_path(mshell, argv[1]);
	else if (!ft_strcmp(argv[1], "-"))
	{
		if (get_env(mshell, "OLDpwd"))
			*path = ft_strdup(get_env(mshell, "OLDpwd"));
		else
		{
			ft_putstr_fd("cd: OLDpwd not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		ft_putendl_fd(*path, STDOUT_FILENO);
	}
	else
		*path = get_simple_path(mshell, argv[1]);
	return (0);
}

static void	update_dir_env(t_mshell *mshell, char *path, char *oldpwd)
{
	register_or_update_env(mshell, "pwd", path);
	register_or_update_env(mshell, "OLDpwd", oldpwd);
	free(oldpwd);
	free(mshell->info.pwd);
	mshell->info.pwd = path;
}

int	ft_cd(t_mshell *mshell, t_command *cmd)
{
	char	*path;
	char	*oldpwd;

	create_argv(mshell, cmd);
	oldpwd = ft_strdup(mshell->info.pwd);
	if (!oldpwd)
		perror("cd");
	if (get_path(cmd->argv, &path, mshell))
	{
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	if (mshell->info.pwd[0] == '/')
		path = get_abs_path(path);
	if (chdir(path) == -1)
	{
		perror("chdir");
		free(path);
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	update_dir_env(mshell, path, oldpwd);
	return (EXIT_SUCCESS);
}

//cd: error retrieving current directory: getcwd: 
//cannot access parent directories: No such file or directory
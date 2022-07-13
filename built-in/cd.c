/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:55:20 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/13 17:28:38 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_special_path(t_mshell *mshell, char *argv1)
{
	if (!argv1)
	{
		if (!get_env(mshell, "HOME"))
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
		else if (!ft_strcmp(get_env(mshell, "HOME"), ""))
			return (ft_strdup(mshell->info.pwd));
		return (ft_strdup(get_env(mshell, "HOME")));
	}
	else if (*argv1 == '~')
	{
		if (!get_env(mshell, "HOME"))
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
		else if (!ft_strcmp(get_env(mshell, "HOME"), ""))
			return (ft_strdup(mshell->info.pwd));
		return (ft_strjoin(get_env(mshell, "HOME"), &argv1[1]));
	}
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
		if (get_env(mshell, "OLDPWD"))
			*path = ft_strdup(get_env(mshell, "OLDPWD"));
		else
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		ft_putendl_fd(*path, STDOUT_FILENO);
	}
	else
		*path = get_simple_path(mshell, argv[1]);
	if (*path == NULL)
		return (EXIT_FAILURE);
	return (0);
}

static void	update_dir_env(t_mshell *mshell, char *path, char *oldpwd)
{
	register_or_update_env(mshell, "PWD", path);
	register_or_update_env(mshell, "OLDPWD", oldpwd);
	free(oldpwd);
	free(mshell->info.pwd);
	mshell->info.pwd = path;
}

int	ft_cd(t_mshell *mshell, t_command *cmd)
{
	char	*path;
	char	*oldpwd;

	create_argv(mshell, cmd);
	oldpwd = ft_strdup(get_env(mshell, "PWD"));
	if (get_path(cmd->argv, &path, mshell))
	{
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	if (mshell->info.pwd[0] == '/')
		path = get_abs_path(path);
	if (chdir(path) == -1)
	{
		perror(cmd->argv[1]);
		free(path);
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	update_dir_env(mshell, path, oldpwd);
	return (EXIT_SUCCESS);
}

//cd: error retrieving current directory: getcwd: 
//cannot access parent directories: No such file or directory
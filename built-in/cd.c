/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 12:55:20 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/08 14:23:38 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_all(char *path, char *oldpwd)
{
	free(path);
	free(oldpwd);
}

char	*get_simple_path(t_mshell *mshell, char *argv1)
{
	if (!argv1)
		return (ft_strdup(get_env(mshell, "HOME")));
	else if (*argv1 == '~')
		return (ft_strjoin(get_env(mshell, "HOME"), &argv1[1]));
	return (ft_strdup(argv1));
}

static int	get_path(char **argv, char **path, t_mshell *mshell)
{
	char	*tmp;

	if (!argv[1] || *argv[1] == '~' || *argv[1] == '/')
		*path = get_simple_path(mshell, argv[1]);
	else if (!ft_strcmp(argv[1], "-"))
	{
		*path = ft_strdup(get_env(mshell, "OLDPWD"));
		if (!ft_strcmp(*path, ""))
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		ft_putendl_fd(*path, STDOUT_FILENO);
	}
	else
	{
		tmp = get_env(mshell, "PWD");
		if (tmp[ft_strlen(tmp) - 1] != '/')
			tmp = ft_strjoin(get_env(mshell, "PWD"), "/");
		else
			tmp = ft_strdup(get_env(mshell, "PWD"));
		*path = ft_strjoin(tmp, argv[1]);
		free(tmp);
	}
	return (0);
}

static void	update_dir_env(t_mshell *mshell, char *path, char *oldpwd)
{
	register_or_update_env(mshell, "PWD", path);
	register_or_update_env(mshell, "OLDPWD", oldpwd);
}

int	ft_cd(t_mshell *mshell, t_command *cmd)
{
	char	*path;
	char	*oldpwd;

	create_argv(mshell, cmd);
	oldpwd = ft_strdup(get_env(mshell, "PWD"));
	if (!oldpwd)
		perror("cd");
	if (get_path(cmd->argv, &path, mshell))
	{
		free_all(path, oldpwd);
		return (EXIT_FAILURE);
	}
	path = get_abs_path(path);
	if (chdir(path) == -1)
	{
		perror("chdir");
		if (!ft_strcmp(cmd->argv[1], ".") || !ft_strcmp(cmd->argv[1], ".."))
			register_or_update_env(mshell, "PWD", path);
		free_all(path, oldpwd);
		return (EXIT_FAILURE);
	}
	update_dir_env(mshell, path, oldpwd);
	free_all(path, oldpwd);
	return (EXIT_SUCCESS);
}

//cd: error retrieving current directory: getcwd: 
//cannot access parent directories: No such file or directory
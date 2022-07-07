/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:54 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/07 21:08:33 by katakagi         ###   ########.fr       */
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

char	*search_cmd_path(char **paths, char *cmd)
{
	size_t	i;
	char	*cand;
	char	*tmp;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cand = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!access(cand, X_OK))
		{
			free_array(paths);
			return (cand);
		}
		free(cand);
		i++;
	}
	free_array(paths);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

char	*get_cmd_path(t_mshell *mshell, char *cmd)
{
	char		**paths;
	struct stat	buff;

	if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3) || *cmd == '/')
	{
		if (!access(cmd, X_OK))
			return (ft_strdup(cmd));
		ft_putstr_fd(cmd, STDERR_FILENO);
		if (stat(cmd, &buff))
		{
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			exit(127);
		}
		else
		{
			ft_putstr_fd(": permission denied\n", STDERR_FILENO);
			exit(126);
		}
		return (NULL);
	}
	paths = ft_split(get_env(mshell, "PATH"), ':');
	if (!paths)
		exit(EXIT_FAILURE);
	return (search_cmd_path(paths, cmd));
}

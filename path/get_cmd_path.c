/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:21:01 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 17:23:26 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	return (NULL);
}

char	*get_cmd_path(t_mshell *mshell, char *cmd)
{
	char	**paths;

	if (!ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3) || *cmd == '/')
	{
		if (!access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	paths = ft_split(get_env(mshell, "PATH"), ':');
	if (!paths)
		exit(EXIT_FAILURE);
	return (search_cmd_path(paths, cmd));
}

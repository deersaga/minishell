/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_abs_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:54 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/07 17:35:56 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*delete_dot_dot(char *path);

char	*delete_dot_slash(char *path)
{
	char	*tmp;
	char	*target;
	size_t	i;

	i = 0;
	target = ft_strnstr(path, "/.", ft_strlen(path));
	while (target)
	{
		if (*(target + 2) != '.')
		{
			tmp = path;
			path = ft_strreplace(path, "/.", "", &i);
			free(tmp);
		}
		else
		{
			i = target - path + 3;
		}
		target = ft_strnstr(path + i, "/.", ft_strlen(path));
	}
	return (path);
}

char	*get_abs_path(char	*path)
{
	path = delete_dot_slash(path);
	path = delete_dot_dot(path);
	return (path);
}

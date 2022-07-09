/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_dot_dot.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:54 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:35:01 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_dot_dot_when_dot_dot(\
	char **path, char **pre_slash, char **dot_dot, char **cur)
{
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = *path;
	*path = ft_strreplace(*path, "/..", "", &i);
	free(tmp);
	*cur = *path;
	*dot_dot = ft_strnstr(*path, "/..", ft_strlen(*path));
	*pre_slash = *path;
}

char	*delete_dot_dot_get_path(\
	char *path, char *pre_slash, char *dot_dot, char *cur)
{
	char	*tmp;
	char	buf[PATH_MAX];
	size_t	i;

	while (*cur)
	{
		i = 0;
		if (path == dot_dot)
			delete_dot_dot_when_dot_dot(&path, &pre_slash, &dot_dot, &cur);
		else if (cur == dot_dot)
		{
			ft_memcpy(buf, pre_slash, cur - pre_slash + 4);
			buf[cur - pre_slash + 3] = '\0';
			tmp = path;
			path = ft_strreplace(path, buf, "", &i);
			free(tmp);
			cur = path;
			dot_dot = ft_strnstr(path, "/..", ft_strlen(path));
			pre_slash = path;
		}
		else if (*cur == '/')
			pre_slash = cur;
		cur++;
	}
	return (path);
}

char	*delete_dot_dot(char *path)
{
	path = delete_dot_dot_get_path(\
		path, \
		path, \
		ft_strnstr(path, "/..", ft_strlen(path)), \
		path);
	if (*path == '\0')
	{
		free(path);
		path = ft_strdup("/");
	}
	return (path);
}

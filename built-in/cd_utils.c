/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 18:06:45 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/08 20:57:59 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*delete_head_dot_dot(char **path, char **dot_dot, char **pre_slash)
{
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = *path;
	*path = ft_strreplace(*path, "/..", "", &i);
	free(tmp);
	*dot_dot = ft_strnstr(*path, "/..", ft_strlen(*path));
	*pre_slash = *path;
	return (*path);
}

char	*delete_mid_dot_dot( \
				char *cur, char **path, char **dot_dot, char **pre_slash)
{
	char	*tmp;
	char	buf[PATH_MAX];
	size_t	i;

	i = 0;
	ft_memcpy(buf, *pre_slash, cur - *pre_slash + 4);
	buf[cur - *pre_slash + 3] = '\0';
	tmp = *path;
	*path = ft_strreplace(*path, buf, "", &i);
	free(tmp);
	*pre_slash = *path;
	*dot_dot = ft_strnstr(*path, "/..", ft_strlen(*path));
	return (*path);
}

char	*delete_dot_dot(char *path)
{
	char	*pre_slash;
	char	*dot_dot;
	char	*cur;

	pre_slash = path;
	dot_dot = ft_strnstr(path, "/..", ft_strlen(path));
	cur = path;
	while (*cur)
	{
		if (path == dot_dot)
			cur = delete_head_dot_dot(&path, &dot_dot, &pre_slash);
		else if (cur == dot_dot)
			cur = delete_mid_dot_dot(cur, &path, &dot_dot, &pre_slash);
		else if (*cur == '/')
			pre_slash = cur;
		cur++;
	}
	if (*path == '\0')
	{
		free(path);
		path = ft_strdup("/");
	}
	return (path);
}

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

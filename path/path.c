#include "../minishell.h"
#include <limits.h>

void	free_array(char **array)
{
	size_t i;

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
	char	*cand;
	char	*tmp;
	size_t	i;

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

//-----------------------------------------------------------------------------

char	*delete_dot_dot(char *path)
{
	char	*pre_slash;
	char	*dot_dot;
	char	*cur;
	char	*tmp;
	char	buf[PATH_MAX];
	size_t	i;

	pre_slash = path;
	dot_dot = ft_strnstr(path, "/..", ft_strlen(path));
	cur = path;
	while (*cur)
	{
		i = 0;
		if (path == dot_dot)
		{
			tmp = path;
			path = ft_strreplace(path, "/..", "", &i);
			free(tmp);
			cur = path;
			dot_dot = ft_strnstr(path, "/..", ft_strlen(path));
			pre_slash = path;
		}
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
		{
			pre_slash = cur;
		}
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
	char	*tmp;
	char	*dot_slash;

	path = delete_dot_slash(path);
	path = delete_dot_dot(path);
	return (path);
}


int main(int argc, char **argv)
{
	t_mshell mshell;
	char	*path;

	path = ft_strdup("/.././../hello/sekai/../.././../he/you/./kon/../../././world/../././..");
	path = get_abs_path(path);
	printf("path %s\n", path);
	free(path);
	/*init_env(&mshell);
	path = get_cmd_path(&mshell, "ls");
	delete_all_env(&mshell);
	if (!path)
		exit(EXIT_FAILURE);
	printf("path %s\n", path);
	free(path);*/
}
/*__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}*/
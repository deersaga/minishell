#include "../minishell.h"

static void	free_all(char *path, char *oldpwd, char **argv)
{
	free(path);
	free(oldpwd);
	free_array(argv);
}

static int	get_path(char **argv, char **path, t_mshell *mshell)
{
	char	*tmp;

	if (!argv[1])
		*path = ft_strdup(get_env(mshell, "HOME"));
	else if (*argv[1] == '~')
		*path = ft_strjoin(get_env(mshell, "HOME"), &argv[1][1]);
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
	else if (*argv[1] == '/')
		*path = ft_strdup(argv[1]);
	else
	{
		tmp = ft_strjoin(get_env(mshell, "PWD"), "/");
		*path = ft_strjoin(tmp, argv[1]);
		free(tmp);
	}
	return (0);
}

static void	update_dir_env(t_mshell *mshell, char *path, char *oldpwd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
		register_or_update_env(mshell, "PWD", pwd);
	else
		register_or_update_env(mshell, "PWD", path);
	register_or_update_env(mshell, "OLDPWD", oldpwd);
	free(pwd);
}

int	ft_cd(t_mshell *mshell, t_command *cmd)
{
	char	*path;
	char	*oldpwd;
	char	**argv;

	argv = create_argv(mshell, cmd);
	if (cmd->argc > 2)
	{
		free_array(argv);
		return (EXIT_FAILURE);
	}
	oldpwd = getcwd(NULL,0);
	if (!oldpwd)
		perror("getcwd");
	if (get_path(argv, &path, mshell))
	{
		free_all(path, oldpwd, argv);
		return (EXIT_FAILURE);
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		if (!ft_strcmp(argv[1], ".") || !ft_strcmp(argv[1], ".."))
			register_or_update_env(mshell, "PWD", path);
		free_all(path, oldpwd, argv);
		return (EXIT_FAILURE);
	}
	update_dir_env(mshell, path, oldpwd);
	free_all(path, oldpwd, argv);
	return (EXIT_SUCCESS);
}

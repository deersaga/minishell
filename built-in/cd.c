#include "../minishell.h"

static int	get_path(char **argv, char **path, t_mshell *mshell)
{
	if (!argv[1] || !ft_strcmp(argv[1], "~"))
		*path = get_env(mshell, "HOME");
	else if (!ft_strcmp(argv[1], "-"))
	{
		*path = get_env(mshell, "OLDPWD");
		if (!ft_strcmp(*path, ""))
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		ft_putendl_fd(*path, STDOUT_FILENO);
	}
	else
		*path = argv[1];
	return (0);
}

int	ft_cd(int argc, char **argv, t_mshell *mshell)
{
	char	*path;
	char	*oldpwd;

	if (argc > 2)
		return (EXIT_FAILURE);
	if (get_path(argv, &path, mshell))
		return (EXIT_FAILURE);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd");
		return (EXIT_FAILURE);
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		free(oldpwd);
		return (EXIT_FAILURE);
	}
	register_or_update_env(mshell, "OLDPWD", oldpwd);
	free(oldpwd);
	return (EXIT_SUCCESS);
}

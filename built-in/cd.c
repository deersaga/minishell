#include "../minishell.h"

static int	get_path(char **argv, char **path)
{
	if (!argv[1] || !ft_strcmp(argv[1], "~"))
		*path = getenv("HOME");
	else if (!ft_strcmp(argv[1], "-"))
	{
		*path = getenv("OLDPWD");
		if (ft_strcmp(*path, ""))
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			return (1);
		}
	}
	else
		*path = argv[1];
	return (0);
}

int	ft_cd(int argc, char **argv)
{
	char	*path;
	char	*pwd;

	if (argc > 2)
		return (1);
	if (get_path(argv, &path))
		return (1);
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		free(pwd);
		return (1);
	}
	//ここでOLDPWDという環境変数にpwdをvalueとして登録する。
	free(pwd);
	return (0);
}

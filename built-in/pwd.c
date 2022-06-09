#include "../minishell.h"

int	ft_pwd(int argc, char **argv)
{
	char	*curdir;

	curdir = getcwd(NULL, 0);
	if (!curdir)
	{
		perror("getcwd");
		free(curdir);
		return (EXIT_FAILURE);
	}
	ft_putstr_fd(curdir, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(curdir);
	return (EXIT_SUCCESS);
}

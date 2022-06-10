#include "../minishell.h"

int	ft_pwd(int argc, char **argv, t_mshell *mshell)
{
	char	*pwd;

	pwd = get_env(mshell, "PWD");
	if (!pwd || *pwd == '\0')
	{
		perror("getcwd");
		return (EXIT_FAILURE);
	}
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

#include "../minishell.h"

int	ft_unset(int argc, char **argv, t_mshell *mshell)
{
	size_t	i;

	i = 1;
	while (i < argc)
	{
		delete_one_env(mshell, argv[i]);
		i++;
	}
	return (0);
}
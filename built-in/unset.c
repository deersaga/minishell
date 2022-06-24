#include "../minishell.h"

int	ft_unset(int argc, char **argv, t_mshell *mshell)
{
	size_t	i;

	(void)argc;
	i = 1;
	while (argv[i])
	{
		delete_one_env(mshell, argv[i]);
		i++;
	}
	return (0);
}

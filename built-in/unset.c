#include "../minishell.h"

int	ft_unset(t_mshell *mshell, t_command *cmd)
{
	size_t	i;
	char	**argv;

	i = 1;
	argv = create_argv(mshell, cmd);
	while (argv[i])
	{
		delete_one_env(mshell, argv[i]);
		i++;
	}
	free_array(argv);
	return (0);
}

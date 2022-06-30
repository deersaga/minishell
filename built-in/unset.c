#include "../minishell.h"

int	ft_unset(t_mshell *mshell, t_command *cmd)
{
	size_t	i;
	char	**argv;

	i = 1;
	create_argv(mshell, cmd);
	while (cmd->argv[i])
	{
		delete_one_env(mshell, cmd->argv[i]);
		i++;
	}
	return (0);
}

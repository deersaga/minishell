#include "../minishell.h"

static void	msg_exit(int status)
{
	ft_putstr_fd("exit\n", 2);
	exit(status);
}

int	ft_exit(t_mshell *mshell, t_command *cmd)
{
	long	status;

	create_argv(mshell, cmd);
	if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else if (cmd->argc == 2)
	{
		if (all_signed_num(cmd->argv[1]))
		{
			status = ft_atol(cmd->argv[1]);
			msg_exit(status % 256);
		}
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		msg_exit(255);
	}
	msg_exit(EXIT_SUCCESS);
	return(0);
}
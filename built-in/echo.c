#include "../minishell.h"

static int	is_all_n(char *s)
{
	size_t	i;

	i = 1;
	if (!s[1])
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_mshell *mshell, t_command *cmd)
{
	int		n_opt;
	size_t	i;
	size_t	j;

	(void)mshell;
	n_opt = 0;
	i = 1;
	create_argv(mshell, cmd);
	if (!cmd->argv[1])
		write(STDOUT_FILENO, "\n", 1);
	else
	{
		while (*(cmd->argv[i]) ==  '-')
		{
			if (!is_all_n(cmd->argv[i]))
				break;
			n_opt = 1;
			i++;
		}
		while (i < cmd->argc)
		{
			ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
			if (i != cmd->argc - 1)
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
		if (!n_opt)
			write(STDOUT_FILENO, "\n", 1);
	}
	//free_array(cmd->argv);
	return (EXIT_SUCCESS);
}

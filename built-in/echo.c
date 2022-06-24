#include "../minishell.h"

int	ft_echo(int argc, char **argv, t_mshell *mshell)
{
	int		n_opt;
	size_t	i;

	(void)mshell;
	n_opt = 0;
	i = 1;
	if (!argv[1])
		write(STDOUT_FILENO, "\n", 1);
	else
	{
		while (!ft_strcmp(argv[i], "-n"))
		{
			n_opt = 1;
			i++;
		}
		while (i < (size_t)argc)
		{
			ft_putstr_fd(argv[i], STDOUT_FILENO);
			if (i != (size_t)(argc - 1))
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
		if (!n_opt)
			write(STDOUT_FILENO, "\n", 1);
	}
	return (EXIT_SUCCESS);
}

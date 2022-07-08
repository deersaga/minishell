/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:06:18 by kaou              #+#    #+#             */
/*   Updated: 2022/07/08 20:01:18 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

void	init_mshell(t_mshell *mshell)
{
	mshell->commands = NULL;
	mshell->env = NULL;
	mshell->num_commands = 0;
	mshell->exit_status = 0;
}

bool	is_valid_cmdline(t_mshell *mshell, char *cmdline)
{
	ft_signal(SIGINT, SIG_DFL);
	if (!cmdline)
	{
		delete_all_env(mshell);
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(0);
	}
	else if (!ft_strcmp(cmdline, ""))
		return (false);
	else
		return (true);
}

int	main(void)
{
	t_mshell	mshell;
	char		*cmdline;
	int			status;

	init_mshell(&mshell);
	init_env(&mshell);
	while (1)
	{
		ft_signal(SIGINT, signal_handler_int);
		ft_signal(SIGQUIT, SIG_IGN);
		cmdline = readline("minishell$>");
		if (!is_valid_cmdline(&mshell, cmdline))
		{
			free(cmdline);
			continue ;
		}
		add_history(cmdline);
		status = parser(&mshell, cmdline);
		if (status)
			mshell.exit_status = status;
		else
		{
			execute_any_cmd(&mshell);
			free_commands(mshell.commands);
		}
		free(cmdline);
	}
	return (0);
}

/*__attribute__((destructor)) static void destructor()
{
	system("leaks -q minishell");
}*/

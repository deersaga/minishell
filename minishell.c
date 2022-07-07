/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:06:18 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 18:03:00 by kaou             ###   ########.fr       */
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
	if (!cmdline)
	{
		delete_all_env(mshell);
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

	init_mshell(&mshell);
	init_env(&mshell);
	while (1)
	{
		signal(SIGINT, signal_handler_int);
		signal(SIGQUIT, SIG_IGN);
		cmdline = readline("minishell$>");
		if (!is_valid_cmdline(&mshell, cmdline))
		{
			free(cmdline);
			continue ;
		}
		add_history(cmdline);
		mshell.exit_status = parser(&mshell, cmdline);
		if (mshell.exit_status == 0)
		{
			execute_any_cmd(&mshell);
			free_commands(mshell.commands);
		}
		free(cmdline);
	}
	return (0);
}
/*
__attribute__((destructor)) static void destructor()
{
	system("leaks -q minishell");
}*/

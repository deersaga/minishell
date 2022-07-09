/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:06:18 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 20:05:16 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_info(t_mshell *mshell, char **environ)
{
	char	*key_val[2];
	size_t	i;

	i = 0;
	while (environ[i])
	{
		get_key_val(environ[i], key_val);
		if (!ft_strcmp(key_val[0], "HOME"))
			mshell->info.home = ft_strdup(key_val[1]);
		else if (!ft_strcmp(key_val[0], "SHLVL"))
			mshell->info.shlvl = ft_itoa(ft_atoi(key_val[1]) + 1);
		free(key_val[1]);
		free(key_val[0]);
		i++;
	}
	mshell->info.pwd = getcwd(NULL, 0);
	if (!mshell->info.pwd)
		mshell->info.pwd = ft_strdup("");
}

void	init_mshell(t_mshell *mshell, char **argv, char **environ)
{
	struct stat	buff;

	if (stat(argv[0], &buff) == 0)
		mshell->info.mshell_inode = buff.st_ino;
	mshell->commands = NULL;
	mshell->env = NULL;
	mshell->num_commands = 0;
	mshell->exit_status = 0;
	init_info(mshell, environ);
}

bool	is_valid_cmdline(t_mshell *mshell, char *cmdline)
{
	ft_signal(SIGINT, SIG_DFL);
	if (!cmdline)
	{
		delete_all_env(mshell);
		free(mshell->info.pwd);
		free(mshell->info.home);
		free(mshell->info.shlvl);
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(0);
	}
	else if (!ft_strcmp(cmdline, ""))
		return (false);
	else
		return (true);
}

void	mshell_interactive(t_mshell *mshell)
{
	char		*cmdline;
	int			status;

	while (1)
	{
		ft_signal(SIGINT, signal_handler_int);
		ft_signal(SIGQUIT, SIG_IGN);
		cmdline = readline("minishell$>");
		if (!is_valid_cmdline(mshell, cmdline))
		{
			free(cmdline);
			continue ;
		}
		add_history(cmdline);
		status = parser(mshell, cmdline);
		if (status)
			mshell->exit_status = status;
		else
		{
			execute_any_cmd(mshell);
			free_commands(mshell->commands);
		}
		free(cmdline);
	}
}

int	main(int argc, char **argv, char **environ)
{
	t_mshell	mshell;

	init_mshell(&mshell, argv, environ);
	init_env(&mshell, environ);
	if (argc == 1)
		mshell_interactive(&mshell);
	else
		return (127);
	return (0);
}

/*__attribute__((destructor)) static void destructor()
{
	system("leaks -q minishell");
}*/

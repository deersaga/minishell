/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:06:18 by ktada             #+#    #+#             */
/*   Updated: 2022/07/13 14:58:56 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fail_invoke(char **argv)
{
	char		buff[BUFF_SIZE];
	ssize_t		count;

	if (argv[1])
		ft_putstr_fd(argv[1], STDERR_FILENO);
	else
	{
		count = read(STDIN_FILENO, buff, BUFF_SIZE);
		if (count == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		if (count == BUFF_SIZE)
			buff[count - 1] = '\0';
		else
			buff[count] = '\0';
		ft_putstr_fd(buff, STDERR_FILENO);
	}
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	return (127);
}

void	init_info(t_mshell *mshell, char **environ)
{
	char	*key_val[2];
	size_t	i;

	i = 0;
	ft_bzero(&mshell->info, sizeof(t_info));
	while (environ[i])
	{
		get_key_val(environ[i], key_val);
		if (!ft_strcmp(key_val[0], "HOME"))
			mshell->info.home = ft_strdup(key_val[1]);
		else if (!ft_strcmp(key_val[0], "SHLVL") && all_num(key_val[1]))
			mshell->info.shlvl = ft_itoa(ft_atoi(key_val[1]) + 1);
		free(key_val[1]);
		free(key_val[0]);
		i++;
	}
	mshell->info.pwd = getcwd(NULL, 0);
	if (!mshell->info.pwd)
		mshell->info.pwd = ft_strdup("");
	if (!mshell->info.shlvl)
		mshell->info.shlvl = ft_strdup("1");
	if (!mshell->info.home)
		mshell->info.home = ft_strdup("");
}

void	init_mshell(t_mshell *mshell, char **environ)
{
	mshell->commands = NULL;
	mshell->env = NULL;
	mshell->num_commands = 0;
	mshell->exit_status = 0;
	init_info(mshell, environ);
}

void	mshell_interactive(t_mshell *mshell)
{
	char		*cmdline;
	int			status;

	ft_signal(SIGINT, signal_handler_int);
	ft_signal(SIGQUIT, SIG_IGN);
	while (1)
	{
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

	init_mshell(&mshell, environ);
	init_env(&mshell, environ);
	rl_outstream = stderr;
	if (argc == 1 && isatty(STDIN_FILENO))
		mshell_interactive(&mshell);
	else
		return (fail_invoke(argv));
	return (0);
}

/*__attribute__((destructor)) static void destructor()
{
	system("leaks -q minishell");
}*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 12:26:13 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 13:12:10 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	check_option(char **argv, size_t *i)
{
	int	n_opt;

	n_opt = 0;
	while (argv[*i] && *(argv[*i]) == '-')
	{
		if (!is_all_n(argv[*i]))
			break ;
		n_opt = 1;
		(*i)++;
	}
	return (n_opt);
}

int	ft_echo(t_mshell *mshell, t_command *cmd)
{
	int		n_opt;
	size_t	i;

	(void)mshell;
	n_opt = 0;
	i = 1;
	create_argv(mshell, cmd);
	if (cmd->argv[1])
	{
		n_opt = check_option(cmd->argv, &i);
		while (i < cmd->argc)
		{
			ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
			if (i != cmd->argc - 1)
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
	}
	if (!n_opt)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}

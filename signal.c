/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:28 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 12:18:01 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_int(int sig)
{
	(void)sig;
	g_flag = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*static int 	check_done(void)
{
	rl_delete_text(0, rl_end);
	rl_done = 1;
	return (0);
}*/

void	signal_handler_heredoc(int sig)
{
	(void)sig;
	g_flag = 1;
}

void	signal_handler_quit(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	write(STDOUT_FILENO, "\n", 1);
	exit(3);
}

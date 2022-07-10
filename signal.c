/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 11:59:28 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/10 19:28:59 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_int(int sig)
{
	(void)sig;
	ft_write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_handler_heredoc(int sig)
{
	(void)sig;
	g_heredoc_sigint = 1;
}

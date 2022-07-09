/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 19:54:46 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:51:04 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_signal(int sig, void (*func)(int))
{
	if (signal(sig, func) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
}

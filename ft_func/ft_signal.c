/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 19:54:46 by kaou              #+#    #+#             */
/*   Updated: 2022/07/08 20:04:33 by kaou             ###   ########.fr       */
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

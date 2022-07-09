/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 13:39:26 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:30:56 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}

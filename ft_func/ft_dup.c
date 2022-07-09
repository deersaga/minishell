/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 13:39:26 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 17:56:55 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_dup(int oldfd)
{
	int	fd;

	fd = dup(oldfd);
	if (fd == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

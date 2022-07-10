/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 20:14:24 by ktada             #+#    #+#             */
/*   Updated: 2022/07/10 19:23:38 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_write(int fd, void *buf, unsigned int byte)
{
	signal(SIGPIPE, SIG_IGN);
	if (write(fd, buf, byte) == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
}

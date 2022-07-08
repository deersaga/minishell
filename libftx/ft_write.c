/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 20:14:24 by kaou              #+#    #+#             */
/*   Updated: 2022/07/08 20:15:43 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_write(int fd, void *buf, unsigned int byte)
{
	if (write(fd, buf, byte) == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
}

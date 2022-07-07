/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fdlstnew.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 18:17:56 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_fdlist	*ft_fdlstnew(int fd)
{
	t_fdlist	*new;
	size_t		i;

	new = (t_fdlist *)ft_malloc(sizeof(t_fdlist));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->buf = (char *)ft_malloc(BUFFER_SIZE + 1);
	if (!new->buf)
		return (NULL);
	i = 0;
	while (i < BUFFER_SIZE + 1)
	{
		new->buf[i] = '\0';
		i++;
	}
	new->next = NULL;
	return (new);
}

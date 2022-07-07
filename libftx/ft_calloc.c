/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 17:01:23 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count)
		if (size > SIZE_MAX / count)
			return (NULL);
	if (!count || !size)
	{
		count = 1;
		size = 1;
	}
	ptr = (void *)malloc((count * size));
	if (!ptr)
	{
		exit(EXIT_FAILURE);
	}
	ft_bzero((void *)ptr, count * size);
	return (ptr);
}

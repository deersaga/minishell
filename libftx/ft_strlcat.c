/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada              #+#    #+#             */
/*   Updated: 2022/07/07 17:03:00 by ktada             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dest[i] != '\0' && i < size)
		i++;
	j = i;
	if (i == size || size == 0)
	{
		while (src[i - j] != '\0')
			i++;
		return (i);
	}
	while (src[i - j] != '\0' && i < size - 1)
	{
		dest[i] = src[i - j];
		i++;
	}
	dest[i] = '\0';
	while (src[i - j] != '\0')
		i++;
	return (i);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:35:01 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	get_size(int n)
{
	unsigned int	size;

	size = 1;
	while (n / 10 != 0)
	{
		n /= 10;
		size++;
	}
	if (n < 0)
		size++;
	return (size);
}

char	*ft_itoa(int n)
{
	unsigned int	i;
	char			*result;
	unsigned int	size;

	size = get_size(n);
	result = (char *)ft_malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (n < 0)
	{
		i = n * -1;
		result[0] = '-';
	}
	else
		i = n;
	result[size] = 0;
	while (i / 10 != 0)
	{
		result[--size] = '0' + i % 10;
		i /= 10;
	}
	result[--size] = '0' + i % 10;
	return (result);
}

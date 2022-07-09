/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ulltoa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada              #+#    #+#             */
/*   Updated: 2022/07/07 18:17:56 by ktada             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_size(unsigned long long n, int base)
{
	size_t	size;

	size = 1;
	while (n / base != 0)
	{
		n /= base;
		size++;
	}
	return (size);
}

/*
This function can convert
unsigned long long to 2 ~ 16 base string.
*/
char	*ft_ulltoa(unsigned long long n, int base)
{
	char				*result;
	size_t				size;

	if (base < 2 || base > 16)
		return (NULL);
	size = get_size(n, base);
	result = (char *)ft_malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[size] = 0;
	while (n / base != 0)
	{
		result[--size] = BASE[n % base];
		n /= base;
	}
	result[--size] = BASE[n % base];
	return (result);
}

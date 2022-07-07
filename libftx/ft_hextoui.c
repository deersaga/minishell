/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hextoui.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 17:01:40 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_hextoui(const char *str)
{
	unsigned long long	result;

	result = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'\
	|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (str[0] == '0' && str[1] == 'x')
		str += 2;
	else
		return (0);
	while (ft_strchr(BASE, *str) - &BASE[0] != 16)
	{
		if (result * 16 > LLMAX || result * 16 + \
		(ft_strchr(BASE, *str) - &BASE[0]) > LLMAX)
		{
			return (0);
		}
		result = result * 16 + (ft_strchr(BASE, *str) - &BASE[0]);
		str++;
	}
	return ((unsigned int)(result));
}

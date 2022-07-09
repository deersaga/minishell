/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:35:01 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_sign(const char *sign)
{
	if (*sign == '-')
		return (-1);
	return (1);
}

long	ft_atol(const char *str)
{
	unsigned long		result;
	long				minus;

	result = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'\
	|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	minus = check_sign(str);
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		if (result * 10 > LLMAX || result * 10 + (*str) - '0' > LLMAX)
		{
			if (minus > 0)
				return (-1);
			return (0);
		}
		result = result * 10 + (*str++) - '0';
	}
	return ((long)(minus * (long long)result));
}

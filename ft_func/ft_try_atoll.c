/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_try_atoll.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada             #+#    #+#             */
/*   Updated: 2022/07/11 22:20:10 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_sign(const char *sign)
{
	if (*sign == '-')
		return (-1);
	return (1);
}

static int	all_signed_num(const char *s)
{
	size_t	i;

	i = 0;
	if (*s == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long	atoll_error(bool *error)
{
	*error = 1;
	return (255);
}

long long	ft_atoll(const char *str)
{
	long long		result;
	long long		sign;

	result = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'\
	|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	sign = check_sign(str);
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
		result = result * 10 + (*str++) - '0';
	return (sign * result);
}

long long	ft_try_atoll(const char *str, bool *error)
{
	const char			*str_arg;
	unsigned long		result;
	long				sign;

	str_arg = str;
	result = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'\
	|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	sign = check_sign(str);
	if (*str == '+' || *str == '-')
		str++;
	if (!all_signed_num(str) || ft_strlen(str) > 19)
		return (atoll_error(error));
	else if (ft_strlen(str) < 19)
		return (ft_atoll(str_arg));
	else
	{
		if (sign == -1 && ft_strcmp(str, "9223372036854775808") > 0)
			return (atoll_error(error));
		if (sign == 1 && ft_strcmp(str, "9223372036854775807") > 0)
			return (atoll_error(error));
		return (ft_atoll(str_arg));
	}
}

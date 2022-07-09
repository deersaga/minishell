/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:35:01 by ktada            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_x_digit_fd(unsigned int n, int fd)
{
	char	msb;

	if (n / 10 > 0)
		print_x_digit_fd(n / 10, fd);
	msb = '0' + n % 10;
	ft_putchar_fd(msb, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	nbr;

	if (n < 0)
	{
		nbr = -1 * n;
		ft_putchar_fd('-', fd);
	}
	else
		nbr = n;
	print_x_digit_fd(nbr, fd);
}

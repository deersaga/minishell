/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada              #+#    #+#             */
/*   Updated: 2022/07/07 17:02:31 by ktada             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dp;
	const unsigned char	*sp;

	dp = (unsigned char *)dest;
	sp = (const unsigned char *)src;
	if (dest < src)
	{
		while (n--)
			*dp++ = *sp++;
	}
	else if (dest > src)
	{
		dp = dp + n;
		sp = sp + n;
		while (n--)
			*(--dp) = *(--sp);
	}
	return ((void *)dest);
}

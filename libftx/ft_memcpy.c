#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dp;
	const unsigned char	*sp;

	dp = (unsigned char *)dest;
	sp = (const unsigned char *)src;
	if (dp == sp)
		return ((void *)dest);
	while (n--)
		*dp++ = *sp++;
	return ((void *)dest);
}

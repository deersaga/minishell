#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*sp;
	size_t			i;

	sp = (unsigned char *)s;
	i = -1;
	while (++i < n)
		*sp++ = 0;
}

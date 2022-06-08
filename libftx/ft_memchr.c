#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*sp;
	unsigned char	cc;

	sp = (unsigned char *)s;
	cc = (unsigned char)c;
	while (n && *sp != cc)
	{
		sp++;
		n--;
	}
	if (n != 0)
		return ((void *)sp);
	return (NULL);
}

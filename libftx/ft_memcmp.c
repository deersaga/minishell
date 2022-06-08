#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*us1;
	unsigned char	*us2;

	us1 = (unsigned char *)s1;
	us2 = (unsigned char *)s2;
	n = n + 1;
	while (--n && *us1 == *us2)
	{
		us1++;
		us2++;
	}
	if (n != 0)
		return ((int)(*us1 - *us2));
	return (0);
}

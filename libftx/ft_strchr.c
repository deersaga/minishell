#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	cc;

	cc = (char)c;
	i = 0;
	while (s[i] != cc && s[i] != 0)
		i++;
	if (cc != 0 && s[i] == 0)
		return (NULL);
	return ((char *)&s[i]);
}

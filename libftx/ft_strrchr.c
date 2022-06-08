#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	ssize_t	last;
	char	cc;

	last = -1;
	i = 0;
	cc = (char)c;
	while (s[i] != 0)
	{
		if (s[i] == cc)
			last = i;
		i++;
	}
	if (cc != 0 && last == -1)
		return (NULL);
	if (cc == 0)
		return ((char *)&s[i]);
	return ((char *)&s[last]);
}

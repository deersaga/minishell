#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (to_find[0] == '\0')
		return ((char *)str);
	while (str[i] != '\0' && i < n)
	{
		if (str[i] != to_find[0])
		{
			i++;
			continue ;
		}
		j = i;
		while (str[i] == to_find[i - j] && i < n)
		{
			i++;
			if (to_find[i - j] == '\0')
				return ((char *)&str[j]);
		}
		i = j + 1;
	}
	return (0);
}

#include "libft.h"

static char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	cat_start;

	i = 0;
	while (dest[i] != '\0')
		i++;
	cat_start = i;
	while (src[i - cat_start] != '\0')
	{
		dest[i] = src[i - cat_start];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	char	*ptr;

	if (!s1 || !s2)
		return (NULL);
	size = ft_strlen(s1) + ft_strlen(s2) + 1;
	ptr = (char *)malloc(size * sizeof(char));
	if (!ptr)
		return (NULL);
	ptr[0] = '\0';
	ft_strcat(ptr, s1);
	ft_strcat(ptr, s2);
	return (ptr);
}

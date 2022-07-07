#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *src)
{
	char	*ptr;

	ptr = (char *)malloc((ft_strlen((const char *)src) + 1) * sizeof(char));
	if (ptr == NULL)
	{
		perror("malloc");
		exit(1);
	}
	ft_strcpy(ptr, src);
	return (ptr);
}

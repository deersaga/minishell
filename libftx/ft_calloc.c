#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count)
		if (size > SIZE_MAX / count)
			return (NULL);
	if (!count || !size)
	{
		count = 1;
		size = 1;
	}
	ptr = (void *)malloc((count * size));
	if (!ptr)
		return (NULL);
	ft_bzero((void *)ptr, count * size);
	return (ptr);
}

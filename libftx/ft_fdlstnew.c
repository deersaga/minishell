#include "libft.h"

t_fdlist	*ft_fdlstnew(int	fd)
{
	t_fdlist	*new;
	size_t		i;

	new = (t_fdlist *)malloc(sizeof(t_fdlist));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!new->buf)
		return (NULL);
	i = 0;
	while (i < BUFFER_SIZE + 1)
	{
		new->buf[i] = '\0';
		i++;
	}
	new->next = NULL;
	return (new);
}

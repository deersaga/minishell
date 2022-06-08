#include "libft.h"

int	ft_int_abs(int n)
{
	if (n == -2147483648)
		return (-1);
	if (n < 0)
		return (-1 * n);
	return (n);
}

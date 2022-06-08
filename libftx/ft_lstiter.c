#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!f || !lst)
		return ;
	while (1)
	{
		f(lst->content);
		if (lst->next == NULL)
			break ;
		lst = lst->next;
	}
}

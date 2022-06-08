#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*cur;
	t_list	*next;

	if (!lst || !(*lst))
		return ;
	cur = *lst;
	next = cur->next;
	while (1)
	{
		ft_lstdelone(cur, del);
		if (next == NULL)
			break ;
		cur = next;
		next = cur->next;
	}
	*lst = 0;
}

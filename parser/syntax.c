#include "../minishell.h"

int	is_redir(e_type_token	type)
{
	if (type == T_REDIR_IN || type == T_REDIR_OUT \
	|| type == T_APPEND || type == T_HEREDOC)
		return (1);
	return (0);
}

int	check_syntax(t_token *head)
{
	t_token		*cur;
	t_token		*next;
	e_type_token	type;

	cur = head;
	if (cur->type == T_PIPE)
		return (1);
	while (cur->token)
	{
		next = cur->next;
		type = cur->type;
		if (type == T_PIPE && next->type == T_PIPE)
			return (1);
		else if (is_redir(type) && (is_redir(next->type) \
		|| next->type == T_PIPE))
			return (1);
		else if ((is_redir(type) || type == T_PIPE) && !next->token)
			return (1);
		cur = next;
	}
	return (0);
}

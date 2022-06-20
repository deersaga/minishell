#include "../minishell.h"

int	is_quote(char cur)
{
	if (cur == '\'' || cur == '\"')
		return (1);
	return (0);
}

int	is_delimiter(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (1);
	return (0);
}

t_token	*new_token(t_token *cur, char *token, type_token type)
{
	cur->token = token;
	cur->type = type;
	cur->next = (t_token *)calloc(1, sizeof(t_token));
	if (!cur->next)
		exit(EXIT_FAILURE);
	return (cur->next);
}

type_token	get_token_type(char	*cur, size_t *i)
{
	if (cur[0] == '>' && cur[1] == '>')
	{
		(*i)++;
		return (T_APPEND);
	}
	else if (cur[0] == '<' && cur[1] == '<')
	{
		(*i)++;
		return (T_HEREDOC);
	}
	if (cur[0] == '>')
		return (T_REDIR_OUT);
	else if (cur[0] == '<')
		return (T_REDIR_IN);
	else if (cur[0] == '|')
		return (T_PIPE);
	else if (cur[0] == '\'')
		return (T_SQUOTE);
	else if (cur[0] == '\"')
		return (T_DQUOTE);
	return (T_WORD);
}

void	check_quote_type_and_len(char *cmdline,size_t i, size_t *len, type_token *type)
{
	if (cmdline[i] == '\'')
	{
		*len = ft_strchr(&cmdline[i + 1], '\'') - &cmdline[i + 1];
		*type = T_SQUOTE;
	}
	else if (cmdline[i] == '\"')
	{
		*len = ft_strchr(&cmdline[i + 1], '\"') - &cmdline[i + 1];
		*type = T_DQUOTE;
	}
}

void	free_token(t_token *token)
{
	free(token->token);
	free(token);
}

void	print_tokens(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur)
	{
		if (cur->token)
			printf("%-20s type: %u  fd %d x %x\n", cur->token, cur->type, cur->fd, cur->token[0]);
		else
			printf("%-20s type: %u  fd %d\n", cur->token, cur->type, cur->fd);
		cur = cur->next;
	}
}

void	delete_one_token(t_token **head, t_token *pre, t_token *cur, t_token *next)
{
	if (!pre)
		*head = next;
	else
		pre->next = next;
	free_token(cur);
}

int	all_num(char *s)
{
	size_t	i;

	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_redirect(type_token type)
{
	if (type == T_REDIR_IN || type == T_REDIR_OUT)
		return (1);
	if (type == T_APPEND || type == T_HEREDOC)
		return (1);
	return (0);
}

t_token	*format_tokens(t_token *head)
{
	t_token	*pre;
	t_token	*cur;
	t_token	*next;

	pre = NULL;
	cur = head;
	next = cur->next;
	while (cur->token)
	{
		if (cur->type == T_WORD && (!cur->token[0] || is_delimiter(cur->token[0])))
			delete_one_token(&head, pre, cur, next);
		else if (all_num(cur->token) && is_redirect(next->type))
		{
			next->fd = ft_atoi(cur->token);
			delete_one_token(&head, pre, cur, next);
		}
		else
			pre = cur;
		cur = next;
		next = cur->next;
	}
	return (head);
}

void	free_all_token(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur)
	{
		free_token(head);
		head = cur->next;
		cur = cur->next;
	}
}

/*
* "3>"と"3 >"を見分けるためにtokenizer関数中のwhile文では冗長な空文字列やデリミターのtokenも生成している(ただこれらのtypeはT_WORDになっている)
* その後にformat_tokensで不要なdelimiterTokenや空文字列tokenを削除しつつ、3>みたいな時はリダイレクションのtokenのfdに値(3)を入れて数字のtokenは削除している。
* expansionに対応できるのかはまだ不明(おそらく多少の修正は必要になるかもしれないが大きな修正にはならないと思われる)。
* あまり綺麗な方法でないので改善の余地あり?
* 
* 追記　3>みたいなケースには対応しなくていいとpedagoに記載があるので必須ではないが一応対応しようと思えばできる状態。
*/
t_token	*verbose_tokenizer(char	*cmdline, t_token *head)
{
	size_t	i;
	size_t	len;
	t_token	*cur;
	size_t	start;
	type_token type;

	i = 0;
	start = 0;
	head = ft_calloc(1, sizeof(t_token));
	if (!head)
		exit(EXIT_FAILURE);
	cur = head;
	while (cmdline[i])
	{
		if (is_delimiter(cmdline[i]) || is_operator(cmdline[i]))
		{
			if (start != i)
				cur = new_token(cur, ft_substr(cmdline, start, i - start), T_WORD);
			start = i;
			type = get_token_type(&cmdline[i], &i);
			cur = new_token(cur, ft_substr(cmdline, start, i - start + 1), type);
			start = i + 1;
		}
		else if (is_quote(cmdline[i]))
		{
			if (start != i)
				cur = new_token(cur, ft_substr(cmdline, start, i - start), T_WORD);
			check_quote_type_and_len(cmdline, i, &len, &type);
			cur = new_token(cur, ft_substr(cmdline, i + 1, len), type);
			i += len + 1;
			start = i + 1;
		}
		i++;
	}
	cur = new_token(cur, ft_substr(cmdline, start, i - start), get_token_type(&cmdline[i], &i));
	return (head);
}

int	tokenizer(t_mshell *mshell, char *cmdline)
{
	t_token	*head;

	head = verbose_tokenizer(cmdline, head);
	print_tokens(head);
	head = format_tokens(head);
	print_tokens(head);
	free_all_token(head);
	return (0);
}

/*
int	main(int argc, char **argv)
{
	t_mshell mshell;

	tokenizer(&mshell, argv[1]);
	
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}


*/
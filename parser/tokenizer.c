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

t_token	*new_token(t_token *cur, char *token, e_type_token type)
{
	cur->token = token;
	cur->type = type;
	cur->next = (t_token *)calloc(1, sizeof(t_token));
	if (!cur->next)
		exit(EXIT_FAILURE);
	return (cur->next);
}

e_type_token	get_token_type(char	*cur, size_t *i)
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
	else if (is_delimiter(cur[0]))
		return (T_DELM);
	return (T_WORD);
}

void	check_quote_type_and_len(char *cmdline,size_t i, size_t *len, e_type_token *type)
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
	while (cur->token)
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

void	free_all_token(t_token *head)
{
	t_token	*cur;

	cur = head;
	while (cur && cur->type != T_END)
	{
		free_token(head);
		head = cur->next;
		cur = cur->next;
	}
	free(cur);
}

int	all_num(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_redirect_token(e_type_token type)
{
	if (type == T_REDIR_IN || type == T_REDIR_OUT)
		return (1);
	if (type == T_APPEND || type == T_HEREDOC)
		return (1);
	return (0);
}

int	is_operator_token(e_type_token type)
{
	if (type == T_REDIR_IN || type == T_REDIR_OUT)
		return (1);
	if (type == T_APPEND || type == T_HEREDOC)
		return (1);
	if (type == T_PIPE)
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
		if (cur->type == T_DELM && next->type == T_DELM)
			delete_one_token(&head, pre, cur, next);
		else if (cur->type == T_DELM && ((pre && is_operator_token(pre->type)) || is_operator_token(next->type)))//(pre->type == T_PIPE || next->type == T_PIPE))
			delete_one_token(&head, pre, cur, next);
		else if (all_num(cur->token) && is_redirect_token(next->type))
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

t_token	*add_front_tokens(t_token **head, t_token *retoken, t_token *pre, t_token *cur)
{
	t_token	*tmp;

	if (!pre)
		*head = retoken;
	else
		pre->next = retoken;
	tmp = retoken;
	while (tmp->next->token)
		tmp = tmp->next;
	free(tmp->next);
	tmp->next = cur;
	return (tmp);
}

t_token	*skip_delimiter_token(t_token *cur)
{
	while (cur->type == T_DELM && cur->token != NULL)
		cur = cur->next;
	return (cur);
}

t_token	*skip_by_next_delimiter_token(t_token *cur)
{
	while (cur->type != T_DELM && cur->token != NULL)
		cur = cur->next;
	return (cur);
}

/*
* "3>"と"3 >"を見分けるためにtokenizer関数中のwhile文では冗長な空文字列やデリミターのtokenも生成している(ただこれらのtypeはT_WORDになっている)
* その後にformat_tokensで不要なdelimiterTokenや空文字列tokenを削除しつつ、3>みたいな時はリダイレクションのtokenのfdに値(3)を入れて数字のtokenは削除している。
* expansionに対応できるのかはまだ不明(おそらく多少の修正は必要になるかもしれないが大きな修正にはならないと思われる)。
* あまり綺麗な方法でないので改善の余地あり?
* 
* 追記　3>みたいなケースには対応しなくていいとpedagoに記載があるので必須ではないが一応対応しようと思えばできる状態。
*/
t_token	*verbose_tokenizer(char	*cmdline)
{
	size_t	i;
	size_t	len;
	t_token	*cur;
	size_t	start;
	e_type_token type;
	t_token *head;
	
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
				cur = new_token(cur, ft_substr(cmdline, start, i - start), get_token_type(&cmdline[start], &start));//T_WORD);
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
	if (start != i)
		cur = new_token(cur, ft_substr(cmdline, start, i - start), get_token_type(&cmdline[i], &i));
	return (head);
}

t_token	*tokenizer(t_mshell *mshell, char *cmdline)
{
	t_token	*head;

	(void)mshell;
	head = verbose_tokenizer(cmdline);
	//print_tokens(head);
	head = format_tokens(head);
	//print_tokens(head);
	//free_all_token(head);
	return (head);
}

char	*concat_tokens(t_mshell *mshell, t_token *head)
{
	char	*ret;
	char	*tmp;
	t_token	*cur;

	cur = head;
	ret = ft_strdup("");
	while (cur->token != NULL && cur->type != T_DELM)
	{
		tmp = ret;
		ret = ft_strjoin(ret, cur->token);
		free(tmp);
		cur = cur->next;
	}
	return (ret);
}


char	*concat_expanded_tokens(t_mshell *mshell, t_token *head)
{
	char	*ret;
	char	*expanded;
	char	*tmp;
	t_token	*cur;

	cur = head;
	ret = ft_strdup("");
	while (cur->token != NULL && cur->type != T_DELM)
	{
		if (cur->type != T_SQUOTE)
			expanded = expansion(mshell, ft_strdup(cur->token));
		else
			expanded = ft_strdup(cur->token);

		tmp = ret;
		ret = ft_strjoin(ret, expanded);
		free(expanded);
		free(tmp);
		cur = cur->next;
	}
	return (ret);
}

/*
* 正直、めちゃくちゃ非効率なことをやっている。
* tokenに$が含まれるか含まれないかを無視してexpansion操作をしているので無駄な操作が多分に含まれている。
* 概要としては、tokenを手前から見て行ってダブルクオートならcur->token(文字列)をexpansionした値に置き換えている。
* (expansion関数の中で展開される前のcur->tokenはfreeされている)
* あとは、ダブルクオートとシングルクオートとdelimiter以外のトークンはexpansionして得られた文字列を再度トークン化して、expansionした
* トークンの手前にそれらトークン(複数の場合もあれば一つの場合もある)を挿入(add_front_tokens)して、expansionする前のトークンは
* delete_one_tokenで削除した(下図)。
* expansionする必要のないトークンもexpansionして全く同じものを作ってから挿入した上で前の複製元のトークンを消すという謎操作をしている。
* 構造体に$を持っているかという情報を持たせればこれらは回避できると思うが、ひとまず動くので全体が動くようになってから時間
* がある時に改良したいと思う。
*
* 図
* A,B,Cはトークン　curがBの時の操作について簡単に説明する
* A->B->C   				Bを展開後B1->B2->B3に再トークン化
* A->B1->B2->B3->B->C   	Bの手前に再トークン化されたものを挿入。
* A->B1->B2->B3->C       	Bを削除してCに対して同様の操作をする。
*/
t_token	*expand_and_retokenize(t_mshell *mshell, t_token *head)
{
	t_token	*pre;
	t_token	*cur;
	t_token	*retoken;
	char	*expanded;

	cur = head;
	pre = NULL;
	while (cur && cur->type != T_END)
	{
		if (cur->type == T_DQUOTE)
			cur->token = expansion(mshell, cur->token);
		else if (cur->type != T_SQUOTE && cur->type != T_DELM)
		{
			expanded = expansion(mshell, ft_strdup(cur->token));
			retoken = verbose_tokenizer(expanded);
			cur = add_front_tokens(&head, retoken, pre, cur);
			delete_one_token(&head, cur, cur->next, cur->next->next);
			free(expanded);
		}
		pre = cur;
		cur = cur->next;
	}
	return (head);
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
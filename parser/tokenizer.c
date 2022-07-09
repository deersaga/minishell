/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:47:07 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:32:00 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_quote_token( \
	char *cmdline, size_t *start, size_t *i, t_token *cur)
{
	t_type_token	type;
	size_t			len;

	if (*start != *i)
		cur = new_token(cur, ft_substr(cmdline, *start, (*i) - (*start)),
				get_token_type(&cmdline[*start], start));
	*start = *i;
	get_quote_type_and_len(cmdline, *i, &len, &type);
	cur = new_token(cur, ft_substr(cmdline, (*i) + 1, len), type);
	*i += len + 1;
	*start = (*i) + 1;
	return (cur);
}

t_token	*create_non_quote_token( \
		char *cmdline, size_t *start, size_t *i, t_token *cur)
{
	t_type_token	type;

	if (*start != *i)
		cur = new_token(cur, ft_substr(cmdline, *start, (*i) - (*start)),
				get_token_type(&cmdline[*start], start));
	*start = *i;
	type = get_token_type(&cmdline[*i], i);
	cur = new_token(cur, ft_substr(cmdline, *start, (*i) - (*start) + 1), type);
	*start = (*i) + 1;
	return (cur);
}

void	delete_one_token( \
			t_token **head, t_token *pre, t_token *cur, t_token *next)
{
	if (!pre)
		*head = next;
	else
		pre->next = next;
	free(cur->token);
	free(cur);
}

t_token	*new_token(t_token *cur, char *token, t_type_token type)
{
	cur->token = token;
	cur->type = type;
	cur->next = (t_token *)ft_calloc(1, sizeof(t_token));
	return (cur->next);
}

/*
* "3>"と"3 >"を見分けるためにtokenizer関数中のwhile文では冗長な空文字列や
* デリミターのtokenも生成している(ただこれらのtypeはT_WORDになっている)
* その後にformat_tokensで不要なdelimiterTokenや空文字列tokenを削除しつつ、
* 3>みたいな時はリダイレクションのtokenのfdに値(3)を入れて数字のtokenは削除している。
* expansionに対応できるのかはまだ不明(おそらく多少の修正は必要になるかもしれないが
* 大きな修正にはならないと思われる)。
* あまり綺麗な方法でないので改善の余地あり?
* 
* 追記　3>みたいなケースには対応しなくていいとpedagoに記載があるので必須ではないが一応対応しようと思えばできる状態。
*/

t_token	*tokenizer(char	*cmdline)
{
	size_t	i;
	size_t	start;
	t_token	*head;
	t_token	*cur;

	i = 0;
	start = 0;
	head = ft_calloc(1, sizeof(t_token));
	cur = head;
	while (cmdline[i])
	{
		if (is_delimiter(cmdline[i]) || is_operator(cmdline[i]))
			cur = create_non_quote_token(cmdline, &start, &i, cur);
		else if (is_quote(cmdline[i]))
			cur = create_quote_token(cmdline, &start, &i, cur);
		i++;
	}
	if (start != i)
		cur = new_token(cur, ft_substr(cmdline, start, i - start),
				get_token_type(&cmdline[i], &i));
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
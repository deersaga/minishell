/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_concat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:54:02 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:31:54 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*subtoken(t_token *start, t_token *end)
{
	t_token	*cur;
	char	*ret;
	char	*tmp;

	cur = start;
	ret = ft_strdup("");
	while (cur != end)
	{
		tmp = ret;
		ret = ft_strjoin(ret, cur->token);
		free(tmp);
		cur = cur->next;
	}
	return (ret);
}

char	*concat_tokens(t_token *head)
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

t_token	*add_front_tokens( \
		t_token **head, t_token *retoken, t_token *pre, t_token *cur)
{
	t_token	*tmp;

	if (!pre)
		*head = retoken;
	else
		pre->next = retoken;
	tmp = retoken;
	while (tmp->next && tmp->next->token)
		tmp = tmp->next;
	free(tmp->next);
	tmp->next = cur;
	return (tmp);
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
			retoken = tokenizer(expanded);
			cur = add_front_tokens(&head, retoken, pre, cur);
			delete_one_token(&head, cur, cur->next, cur->next->next);
			free(expanded);
		}
		pre = cur;
		cur = cur->next;
	}
	return (head);
}

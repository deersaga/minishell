#include "../minishell.h"

char	*ft_strreplace(char *src, char *target, char *implant, size_t *start)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*tar;
	char	*ret;

	len = ft_strlen(src) + ft_strlen(implant) - ft_strlen(target);
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		exit(EXIT_FAILURE);
	i = 0;
	tar = ft_strnstr(src + (*start), target, ft_strlen(src));
	while (*src)
	{
		if (src == tar)
		{
			j = 0;
			while (implant[j])
				ret[i++] = implant[j++];
			src += ft_strlen(target);
		}
		ret[i++] = *src++;
	}
	ret[i] = '\0';
	(*start) += ft_strlen(implant);
	return (ret);
}

char	*search_param(char *str, size_t *start)
{
	size_t	j;

	j = 1;
	while (str[*start])
	{
		if (str[*start] != '$')
		{
			(*start)++;
			continue ;
		}
		while (ft_isalnum(str[(*start) + j]) || str[(*start) + j] == '_')
			j++;
		break ;
	}
	if ((*start) >= ft_strlen(str))
		return (NULL);
	return (ft_substr(str, *start, j));
}

char	*expansion(t_mshell *mshell, char *str)
{
	size_t	start;
	char	*target;
	char	*implant;
	char	*tmp;

	start = 0;
	target = search_param(str, &start);
	while (target)
	{
		if (!target[1])
			implant = "$";
		else
			implant = get_env(mshell, &target[1]);
		if (!implant)
			implant = "";
		tmp = str;
		str = ft_strreplace(str, target, implant, &start);
		free(tmp);
		free(target);
		target = search_param(str, &start);
	}
	return (str);
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
			retoken = verbose_tokenizer(expanded, retoken);
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
	char	**splited;
	t_mshell mshell;

	init_env(&mshell);
	register_or_update_env(&mshell, "test", "sekai");
	t_token *tmp1;
	tmp1 = verbose_tokenizer("$HOME-$test$- $a$test$", tmp1);
	print_tokens(tmp1);
	char	*tmp;
	tmp = concat_expanded_tokens(&mshell, tmp1);
	printf("%d \n", printf("%s\n",tmp));
	free_all_token(tmp1);
	free(tmp);
	delete_all_env(&mshell);
	return (0);
}
*/
/*
__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}
*/
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
	tar = ft_strnstr(src, target, ft_strlen(src));
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
	while (cur->token != NULL)
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
/*char	*expansion(t_mshell *mshell, char *str)
{
	char	**splited;
	char	**replace;
	char	*tmp;
	char	*val;
	char	*expanded;
	size_t i;


	if (!ft_strchr(str, '$'))
		return (ft_strdup(str));
	splited = ft_split(str, '$');
	i = 0;
	while(splited[i])
	{
		if (str[0] != '$' && i == 0)
		{
			i++;
			continue ;
		}
		replace = ft_split_extend(splited[i], " \t\n");
		tmp = replace[0];
		val = get_env(mshell, replace[0]);
		if (!val)
			val = "";
		replace[0] = ft_strdup(val);
		free(tmp);
		free(splited[i]);
		splited[i] = ft_multistrjoin(replace);
		free_array(replace);
		i++;
	}
	tmp = ft_multistrjoin(splited);
	free_array(splited);
	return tmp;
}*/
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/30 13:29:59 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/01 13:57:54 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strreplace(char *src, char *target, char *implant, size_t *start)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*tar;
	char	*ret;

	len = ft_strlen(src) + ft_strlen(implant) - ft_strlen(target);
	ret = ft_calloc(len + 1, sizeof(char));
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
		if (str[(*start) + j] == '?')
			j++;
		else
		{
			while (ft_isalnum(str[(*start) + j]) || str[(*start) + j] == '_')
				j++;
		}
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
		else if (target[1] == '?')
			implant = ft_itoa(mshell->exit_status);
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
#include "../minishell.h"

char	**ft_split_extend(char const *str, char *sep);

static char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	cat_start;

	i = 0;
	while (dest[i] != '\0')
		i++;
	cat_start = i;
	while (src[i - cat_start] != '\0')
	{
		dest[i] = src[i - cat_start];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*ft_multistrjoin(char **str_array)
{
	size_t	size;
	size_t	i;
	char	*ptr;

	size = 1;
	i = 0;
	while (str_array[i])
		size += ft_strlen(str_array[i++]);
	ptr = (char *)malloc(size * sizeof(char));
	if (!ptr)
		exit(1);
	ptr[0] = '\0';
	i = 0;
	while (str_array[i])
		ft_strcat(ptr, str_array[i++]);
	return (ptr);
}

/*
void	free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
*/

char	*ft_strreplace(char *src, char *target, char *implant, size_t *start)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*ret;

	len = ft_strlen(src) + ft_strlen(implant) - ft_strlen(target);
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		exit(EXIT_FAILURE);
	i = 0;
	while (*src)
	{
		if (src == ft_strnstr(src, target, ft_strlen(src)))
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
		while (!ft_strchr("$ \t\n", str[(*start) + j]))
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

int	main(int argc, char **argv)
{
	char	**splited;
	t_mshell mshell;

	init_env(&mshell);
	register_or_update_env(&mshell, "test", "sekai");
	char *tmp1 = ft_strdup("$HOME $test $a$test $a");
	char	*tmp;
	tmp = expansion(&mshell, tmp1);
	printf("%d \n", printf("%s\n",tmp));
	free(tmp);
	delete_all_env(&mshell);
	return (0);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}

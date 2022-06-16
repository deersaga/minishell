#include "../minishell.h"

char	*remove_dquote(char *src)
{
	size_t	i;
	size_t	len;
	char	*ret;

	len = 0;
	i = 0;
	while (src[i++])
	{
		if (src[i] == '\"')
			continue ;
		else
			len++;
	}
	i = 0;
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		exit(EXIT_FAILURE);
	while (*src)
	{
		if (*src != '\"')
			ret[i++] = *src;
		src++;
	}
	return (ret);
}



int	ft_export(int argc, char **argv, t_mshell *mshell)
{
	char		*trimed;
	char		*eq_pos;
	char		*key;
	char		*val;
	size_t		i;

	i = 0;
	while (argv[++i])
	{
		if (*argv[i] == '=')
		{
			perror("export");
			continue;
		}
		trimed = remove_dquote(argv[i]);
		eq_pos = ft_strchr(trimed, '=');
		if (!eq_pos)
		{
			key = trimed;
			val = NULL;
		}
		else
		{
			key = ft_substr(trimed, 0, eq_pos - trimed);
			val = ft_substr(eq_pos + 1, 0, ft_strlen(trimed));
		}
		register_or_update_env(mshell, key, val);
	}
	
	return (0);
}


int main(int argc, char **argv)
{
	t_mshell mshell;

	init_env(&mshell);
	printf("%s\n", argv[1]);
	char *trimed = remove_dquote(argv[1]);
	printf("%s\n", trimed);
	printf("%d\n", ft_export(1, argv, &mshell));
	print_env(mshell.env);
}
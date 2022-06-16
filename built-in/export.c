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
	ret[i] = '\0';
	return (ret);
}



int	ft_export(int argc, char **argv, t_mshell *mshell)
{
	char		*trimed;
	char		*eq_pos;
	char		*key;
	char		*tmp;
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
		printf("argv %s\n", argv[i]);
		trimed = remove_dquote(argv[i]);
		eq_pos = ft_strchr(trimed, '=');
		if (!eq_pos)
		{
			key = expansion(mshell, trimed);
			val = NULL;
		}
		else
		{
			
			tmp =  ft_substr(trimed, 0, eq_pos - trimed);
			key = expansion(mshell, tmp);
			tmp = ft_substr(eq_pos + 1, 0, ft_strlen(trimed));
			val = expansion(mshell, tmp);
		}
		free(trimed);
		register_or_update_env(mshell, key, val);
		free(key);
		free(val);
	}
	
	return (0);
}


int main(int argc, char **argv)
{
	t_mshell mshell;

	init_env(&mshell);
	char *tmp = ft_export(1, argv, &mshell);
	//printf("%s\n", tmp);
	free(tmp);
	print_env(mshell.env);
	delete_all_env(&mshell);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}
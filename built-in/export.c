#include "../minishell.h"


static size_t	calc_len(char *src)
{
	size_t	i;
	size_t	len;
	int		in_quote[2];

	in_quote[0] = 0;
	in_quote[1] = 0;
	i = -1;
	len = 0;
	while (src[++i])
	{
		if (src[i] == '\"' && !in_quote[0])
		{
			in_quote[1] = !in_quote[1];
			continue ;
		}
		else if (src[i] == '\'' && !in_quote[1])
		{
			in_quote[0] = !in_quote[0];
			continue ;
		}
		len++;
	}
	return (len);
}

static void	copy_quote_removed_str(char *src, char *ret)
{
	size_t	i;
	size_t	len;
	int		in_quote[2];

	in_quote[0] = 0;
	in_quote[1] = 0;
	i = 0;
	while (*src)
	{
		if (*src == '\"' && !in_quote[0])
		{
			src++;
			in_quote[1] = !in_quote[1];
			continue ;
		}
		else if (*src == '\'' && !in_quote[1])
		{
			src++;
			in_quote[0] = !in_quote[0];
			continue ;
		}
		ret[i++] = *src++;
	}
	ret[i] = '\0';
}

char	*remove_quote(char *src)
{
	size_t	i;
	size_t	len;
	char	*ret;
	int		in_quote[2];

	in_quote[0] = 0;
	in_quote[1] = 0;
	len = 0;
	i = 0;
	len = calc_len(src);
	i = 0;
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		exit(EXIT_FAILURE);
	i = 0;
	copy_quote_removed_str(src, ret);
	return (ret);
}

int	is_valid(char *argv)
{
	return (1);
}


int	ft_export(int argc, char **argv, t_mshell *mshell)
{
	t_token		*tokens;
	char		*trimed;
	char		*eq_pos;
	char		*key;
	char		*tmp;
	char		*val;
	size_t		i;

	i = 0;
	if (argc == 1)
		print_env(mshell->env);
	while (argv[++i])
	{
		printf("argv %s\n", argv[i]);
		tokens = verbose_tokenizer(argv[i], tokens);
		trimed = concat_expanded_tokens(mshell, tokens);
		free_all_token(tokens);
		if (!is_valid(trimed))
		{
			free(trimed);
			perror("export");
			continue;
		}
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
		free(trimed);
		register_or_update_env(mshell, key, val);
		free(key);
		free(val);
	}
	sort_env(mshell->env);
	print_env(mshell->env);
	return (0);
}

/*
int main(int argc, char **argv)
{
	t_mshell mshell;

	init_env(&mshell);
	ft_export(1, argv, &mshell);
	//printf("%s\n", tmp);
	//print_env(mshell.env);
	delete_all_env(&mshell);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}*/
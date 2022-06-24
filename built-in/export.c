#include "../minishell.h"

int	is_valid(char *s)
{
	char	*eq_pos;
	char	*cur;

	eq_pos = ft_strchr(s, '=');
	if (!eq_pos)
		eq_pos = ft_strchr(s, '\0');
	cur = s;
	while (cur != eq_pos)
	{
		if (!(ft_isalnum(*cur) || *cur == '_'))
			return (0);
		cur++;
	}
	return (1);
}


int	ft_export(int argc, char **argv, t_mshell *mshell)
{
	t_token		*tokens;
	char		*trimed;
	char		*eq_pos;
	char		*key;
	char		*val;
	size_t		i;

	i = 0;
	if (argc == 1)
	{
		sort_env(mshell->env);
		print_env(mshell->env);
	}
	while (argv[++i])
	{
		tokens = verbose_tokenizer(argv[i], tokens);
		trimed = concat_expanded_tokens(mshell, tokens);
		free_all_token(tokens);
		if (!is_valid(trimed))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			free(trimed);
			continue;
		}		
		eq_pos = ft_strchr(trimed, '=');
		if (!eq_pos)
		{
			key = ft_strdup(trimed);
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
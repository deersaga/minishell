#include "../minishell.h"

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
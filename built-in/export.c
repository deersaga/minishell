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


int	ft_export(t_mshell *mshell, t_command *cmd)
{
	char		*eq_pos;
	char		*key;
	char		*val;
	size_t		i;

	
	create_argv(mshell, cmd);
	if (cmd->argc == 1)
	{
		sort_env(mshell->env);
		print_env(mshell->env);
	}
	i = 0;
	while (cmd->argv[++i])
	{
		if (!is_valid(cmd->argv[i]))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			continue;
		}		
		eq_pos = ft_strchr(cmd->argv[i], '=');
		if (!eq_pos)
		{
			key = ft_strdup(cmd->argv[i]);
			val = NULL;
		}
		else
		{
			key = ft_substr(cmd->argv[i], 0, eq_pos - cmd->argv[i]);
			val = ft_substr(eq_pos + 1, 0, ft_strlen(cmd->argv[i]));
		}
		register_or_update_env(mshell, key, val);
		free(key);
		free(val);
	}
	//free_array(cmd->argv);
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
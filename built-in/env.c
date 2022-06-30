#include "../minishell.h"

int	ft_env(t_mshell *mshell, t_command *cmd)
{
	t_mshell	mshellcopy;
	size_t		i;
	pid_t		pid;
	char		*eq;
	char		*key_val[2];
	char		**env;

	create_argv(mshell, cmd);
	pid = fork();
	if (pid == 0)
	{
		mshellcopy.env = copy_env(mshell->env);
		mshellcopy.num_commands = 0;
		i = 1;
		while (cmd->argv[i] && ft_strchr(cmd->argv[i], '='))
		{
			eq = ft_strchr(cmd->argv[i], '=');
			key_val[0] = ft_substr(cmd->argv[i], 0, eq - cmd->argv[i]);
			key_val[1] = ft_substr(eq + 1, 0, ft_strlen(cmd->argv[i]));
			register_or_update_env(&mshellcopy, key_val[0], key_val[1]);
			free(key_val[0]);
			free(key_val[1]);
			i++;
		}
		env = make_environ(&mshellcopy);
		if (cmd->argv[i] == NULL)
		{
			print_env(mshellcopy.env);
			exit(0);
		}
		else
		{
			char *path = get_cmd_path(&mshellcopy, cmd->argv[i]);
			free(cmd->argv[i]);
			cmd->argv[i] = path;
			execve(cmd->argv[i], &(cmd->argv[i]), env);
			exit(127);
		}
		//delete_all_env(&mshellcopy);
	}
	return (0);
}
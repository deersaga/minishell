#include "minishell.h"

extern char **environ;

//int ft_cd(int argc, char **argv, t_mshell *mshell);
int ft_pwd(int argc, char **argv, t_mshell *mshell);

void	exec_cmd(char *cmd)
{
	char	*argv[5];

	if (strcmp(cmd, "ls") == 0)
	{
		argv[0] = "/bin/ls";
		argv[1] = "-a";
		argv[2] = NULL;
		if (execve(argv[0], argv, environ) < 0)
		{
			perror("execve");
			exit(1);
		}
	}
	else if (strcmp(cmd, "rm") == 0)
	{
		argv[0] = "/bin/rm";
		argv[1] = "-rf";
		argv[2] = "/Users/work/42Tokyo/42cursus/3-level/minishell/test";
		argv[3] = NULL;
		if (execve(argv[0], argv, environ) < 0)
		{
			perror("execve");
			exit(1);
		}
	}

}

void	free_array(char **array)
{
	size_t i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int main(int argc, char **argv)
{
	pid_t	child_pid;
	t_mshell mshell;
	char	*cmd;
	char	**splited_cmd;
	int		status;
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	//act.sa_handler = SIG_IGN;
	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, NULL);
	init_env(&mshell);
	while (1)
	{
		cmd = readline("minishell$>");
		splited_cmd = ft_split(cmd, ' ');
		add_history(cmd);
		if (!cmd)
			exit(0);
		if (!ft_strcmp(splited_cmd[0], "cd"))
		{
			ft_cd(2, splited_cmd, &mshell);
			free_array(splited_cmd);
			free(cmd);
			continue ;
		}
		if (!ft_strcmp(splited_cmd[0], "pwd"))
		{
			ft_pwd(1, splited_cmd, &mshell);
			free_array(splited_cmd);
			free(cmd);
			continue ;
		}
		child_pid = fork();
		if (child_pid < 0)
		{
			perror("fork");
			exit(1);
		}
		if (child_pid == 0)
			exec_cmd(cmd);
		else
		{
			if (waitpid(child_pid, &status, 0) < 0)
			{
				perror("waitpid");
				exit(1);
			}
		}
		free_array(splited_cmd);
		free(cmd);
	}
	return (0);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}
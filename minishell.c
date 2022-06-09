#include "minishell.h"

extern char **environ;

//int ft_cd(int argc, char **argv, t_mshell *mshell);
int ft_pwd(int argc, char **argv);

void	exec_cmd(char *cmd)
{
	char	*argv[3];

	argv[0] = "/bin/ls";
	argv[1] = "-a";
	argv[2] = NULL;
	if (strcmp(cmd, "ls") == 0)
	{
		if (execve(argv[0], argv, environ) < 0)
		{
			perror("execve");
			exit(1);
		}
	}
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
		cmd = readline("$>");
		splited_cmd = ft_split(cmd, ' ');
		add_history(cmd);
		if (!cmd)
			exit(0);
		if (!ft_strcmp(splited_cmd[0], "cd"))
			ft_cd(2, splited_cmd, &mshell);
		if (!ft_strcmp(splited_cmd[0], "pwd"))
			ft_pwd(1, splited_cmd);
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
		free(cmd);
	}
	return (0);
}



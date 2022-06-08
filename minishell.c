#include "minishell.h"
extern char **environ;

void	exec_cmd(char *cmd)
{
	char	*argv[2];

	argv[0] = "/bin/ls";
	argv[1] = NULL;
	sleep(5);
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
	char	*cmd;
	int		status;
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	//act.sa_handler = SIG_IGN;
	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, NULL);
	while (1)
	{
		cmd = readline("$>");
		add_history(cmd);
		printf("%s\n", cmd);
		if (!cmd)
			exit(0);
		/*child_pid = fork();
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
		}*/
		free(cmd);
	}
	return (0);
}



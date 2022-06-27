#include "minishell.h"

extern char **environ;

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


void	init_mshell(t_mshell *mshell)
{
	mshell->commands = NULL;
	mshell->env = NULL;
	mshell->num_commands = 0;
}

int main(int argc, char **argv)
{
	//pid_t	child_pid;
	t_mshell mshell;
	char	*cmdline;
	char	**splited_cmd;
	//int		status;
	struct sigaction	act;

	(void)argv;
	sigemptyset(&act.sa_mask);
	act.sa_handler = SIG_IGN;
	//act.sa = SIG_DFL;
	//printf("minishell\n");
	sigaction(SIGQUIT, &act, NULL);
	init_mshell(&mshell);
	init_env(&mshell);
	register_or_update_env(&mshell, "test", "sekai");
	register_or_update_env(&mshell, "test1", "sekai");
	while (1)
	{
		cmdline = readline("minishell$>");
		add_history(cmdline);
		mshell.num_commands = 0;
		if (!cmdline)
		{
			delete_all_env(&mshell);
			return(0);
		}
		parser(&mshell, cmdline);
		execute_commands(&mshell);
		free_commands(mshell.commands);
		free(cmdline);
	}
	return (0);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}



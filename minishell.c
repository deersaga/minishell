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


void	init_mshell(t_mshell *mshell)
{
	mshell->commands = NULL;
	mshell->env = NULL;
	mshell->num_commands = 0;
}

int main(int argc, char **argv)
{
	pid_t	child_pid;
	t_mshell mshell;
	char	*cmdline;
	char	**splited_cmd;
	int		status;
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	//act.sa_handler = SIG_IGN;
	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, NULL);
	init_mshell(&mshell);
	init_env(&mshell);
	register_or_update_env(&mshell, "test", "sekai");
	register_or_update_env(&mshell, "test1", "sekai");
	while (1)
	{
		cmdline = readline("minishell$>");
		add_history(cmdline);
		if (!cmdline)
		{
			delete_all_env(&mshell);
			return(0);
		}
		splited_cmd = ft_split(cmdline, ' ');
		parser(&mshell, cmdline);
		argv = create_argv(&mshell, &(mshell.commands->token));
		print_array(argv);
		free_array(argv);
		print_commands(&mshell);
		free_commands(mshell.commands);
		//tokenizer(&mshell, cmdline);
		if (!ft_strcmp(splited_cmd[0], "print_env"))
			print_env(mshell.env);
		if (!ft_strcmp(splited_cmd[0], "cd"))
		{
			ft_cd(2, splited_cmd, &mshell);
			free_array(splited_cmd);
			free(cmdline);
			continue ;
		}
		if (!ft_strcmp(splited_cmd[0], "pwd"))
		{
			ft_pwd(1, splited_cmd, &mshell);
			free_array(splited_cmd);
			free(cmdline);
			continue ;
		}
		if (!ft_strcmp(splited_cmd[0], "unset"))
		{
			ft_unset(argc, splited_cmd, &mshell);
			free_array(splited_cmd);
			free(cmdline);
			continue ;
		}
		if (!ft_strcmp(splited_cmd[0], "export"))
		{
			char *arg[3] = {"export", "test!=\"$PWD\"\'$PWD\'", NULL};
			ft_export(2, arg, &mshell);
			char *ar[3] = {"export", "hello!", NULL};
			ft_export(2, ar, &mshell);
			free_array(splited_cmd);
			free(cmdline);
			continue ;
		}
		/*child_pid = fork();
		if (child_pid < 0)
		{
			perror("fork");
			exit(1);
		}
		if (child_pid == 0)
			exec_cmd(cmdline);
		else
		{
			if (waitpid(child_pid, &status, 0) < 0)
			{
				perror("waitpid");
				exit(1);
			}
		}*/
		free_array(splited_cmd);
		free(cmdline);
	}
	return (0);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}
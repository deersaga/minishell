#include "minishell.h"

extern char **environ;

void	init_mshell(t_mshell *mshell)
{
	mshell->commands = NULL;
	mshell->env = NULL;
	mshell->num_commands = 0;
	mshell->exit_status = 0;
}

static int 	check_done(void)
{
	if (flag == 1)
	{
		rl_delete_text(0, rl_end);
		rl_done = 1;
	}
	return (0);
}
void do_heredoc()
{
	signal(SIGINT, signal_handle_heredoc);
	rl_event_hook = check_done;
	while(1)
	{

		if (flag == 1)
			break;
		//char *cmdline = readline(">");
		/*if (!ft_strcmp("eof", cmdline))
			break ;
		write();*/
	}
	rl_event_hook = NULL;

}

int main(int argc, char **argv)
{
	//pid_t	child_pid;
	t_mshell mshell;
	char	*cmdline;
	//int		status;
	//struct sigaction	act;

	(void)argv;
	(void)argc;
	//sigemptyset(&act.sa_mask);
	//act.sa_handler = SIG_IGN;
	//act.sa_sigaction = SIG_IGN;
	//act.sa_flags = SA_NODEFER;
    //act.sa_restorer = NULL;
	//act.sa = SIG_DFL;
	//printf("minishell\n");
	//sigaction(SIGQUIT, &act, NULL);
	//signal(SIGINT, signal_handle_int);
	signal(SIGQUIT, SIG_IGN);
	init_mshell(&mshell);
	init_env(&mshell);
	register_or_update_env(&mshell, "test", "sekai");
	register_or_update_env(&mshell, "test1", "sekai");
	//signal(SIGINT, signal_handle_int);
	while (1)
	{
		cmdline = readline("minishell$>");
		if (!cmdline)
		{
			delete_all_env(&mshell);
			return(0);
		}
		else if (!ft_strcmp(cmdline, ""))
		{
			free(cmdline);
			continue ;
		}
		if (!ft_strcmp("heredoc", cmdline))
		{
			do_heredoc();
			do_heredoc();
		}
		flag = 0;
		add_history(cmdline);
		mshell.num_commands = 0;
		parser(&mshell, cmdline);
		print_commands(&mshell);
		execute_commands(&mshell);
		free_commands(mshell.commands);
		free(cmdline);
	}
	return (0);
}

/*__attribute__((destructor)) static void destructor()
{
	system("leaks -q a.out");
}
*/
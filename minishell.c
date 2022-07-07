#include "minishell.h"

extern char **environ;

void	init_mshell(t_mshell *mshell)
{
	mshell->commands = NULL;
	mshell->env = NULL;
	mshell->num_commands = 0;
	mshell->exit_status = 0;
}
/*
static int 	check_done(void)
{
	if (g_flag == 1)
	{
		rl_delete_text(0, rl_end);
		rl_done = 1;
	}
	return (0);
}
void do_heredoc()
{
	signal(SIGINT, signal_handler_heredoc);
	rl_event_hook = check_done;
	while(1)
	{

		if (g_flag == 1)
			break;
		//char *cmdline = readline(">");
		//if (!ft_strcmp("eof", cmdline))
		//	break ;
		//write();
	}
	rl_event_hook = NULL;

}*/

int main(int argc, char **argv)
{
	//pid_t	child_pid;
	t_mshell mshell;
	char	*cmdline;
	int		status;

	(void)argv;
	(void)argc;
	init_mshell(&mshell);
	init_env(&mshell);
	while (1)
	{
		if (signal(SIGINT, signal_handler_int) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR)
			return (1);
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
		add_history(cmdline);
		status = parser(&mshell, cmdline);
		if (status)
			mshell.exit_status = status;
		else
		{
			execute_commands(&mshell);
			free_commands(mshell.commands);
		}
		free(cmdline);
	}
	return (0);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q minishell");
}


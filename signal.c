#include "minishell.h"


void	signal_handle_int(int sig)
{
	(void)sig;
	flag = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*static int 	check_done(void)
{
	rl_delete_text(0, rl_end);
	rl_done = 1;
	return (0);
}*/


void	signal_handle_heredoc(int sig)
{
	(void)sig;
	flag = 1;
	//write(STDOUT_FILENO, "\n", 1);
	//rl_on_new_line();
	//rl_replace_line("", 0);
	//rl_redisplay();

}

void	signal_handle_quit(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	write(STDOUT_FILENO, "\n", 1);
	exit(3);
}
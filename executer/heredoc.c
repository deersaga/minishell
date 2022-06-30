/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 19:24:51 by kaou              #+#    #+#             */
/*   Updated: 2022/07/01 02:33:57 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signal_handle_heredoc(int sig)
{
	g_heredoc_sigint = 1;
}

static int	check_heredoc_sigint(void)
{
	if (g_heredoc_sigint == 1)
	{
		rl_delete_text(0, rl_end);
		rl_done = 1;
	}
	return (0);
}

void create_heredoc_file(t_mshell *mshell, t_redir *heredoc)
{
	char	*cmdline;
	int		fd;

	fd = open(heredoc->file, O_CREAT | O_WRONLY | O_TRUNC, S_IWUSR | S_IRUSR);
	signal(SIGINT, signal_handle_heredoc);
	rl_event_hook = check_heredoc_sigint;
	printf("create_heredoc_file  eof is {%s}\n", heredoc->heredoc_eof);
	while(1)
	{
		if (g_heredoc_sigint == 1)
			break ;
		cmdline = readline(">");
		if (!ft_strcmp(heredoc->heredoc_eof, cmdline))
		{
			free(cmdline);
			break ;
		}
		if (!heredoc->has_quote)
			cmdline = expansion(mshell, cmdline);
		write(fd, cmdline, ft_strlen(cmdline));
		write(fd, "\n", 1);
		free(cmdline);
	}
	rl_event_hook = NULL;
	signal(SIGINT, signal_handle_int);
	close(fd);
}

void	delete_heredoc_files(t_mshell	*mshell)
{
	t_command	*cur;
	t_redir		*redir_in;
	size_t		i;

	cur = mshell->commands;
	i = 0;
	while (i < mshell->num_commands)
	{
		redir_in = cur->redir_in;
		while (redir_in && redir_in->file)
		{
			if (redir_in->type == T_HEREDOC)
				unlink(redir_in->file);
			redir_in = redir_in->next;
		}
		i++;
	}
}

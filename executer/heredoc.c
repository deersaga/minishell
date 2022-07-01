/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 19:24:51 by kaou              #+#    #+#             */
/*   Updated: 2022/07/01 21:37:19 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*make_heredoc_filename(size_t	heredoc_id)
{
	char	*file_prefix;
	char	*file_suffix;
	char	*file_name;
	size_t	i;

	file_prefix = ft_strdup(".minishell_tmp_heredoc_");
	if (file_prefix == NULL)
		exit(EXIT_FAILURE);
	file_suffix = ft_calloc(25, sizeof(char));
	if (file_suffix == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < 24)
	{
		file_suffix[i] = '0' + (heredoc_id % 10);
		heredoc_id /= 10;
		i++;
	}
	file_suffix[i] = '\0';
	file_name = ft_strjoin(file_prefix, file_suffix);
	if (file_name == NULL)
		exit(EXIT_FAILURE);
	free(file_prefix);
	free(file_suffix);
	return (file_name);
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
	signal(SIGINT, signal_handler_heredoc);
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
	signal(SIGINT, signal_handler_int);
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

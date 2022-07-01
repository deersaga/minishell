/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 17:10:56 by kaou              #+#    #+#             */
/*   Updated: 2022/07/01 12:23:14 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	openfile (char *filename, t_type_token mode)
{
	int	fd;

	if (mode == T_REDIR_IN)
	{
		//標準エラーに出さないとダメかも
		if (access(filename, F_OK))
		{
			printf("%s: No such file or directory\n", filename);
			exit(EXIT_FAILURE);
		}
		fd = (open(filename, O_RDONLY));
	}
	else if (mode == T_REDIR_OUT)
		fd = (open(filename, O_CREAT | O_WRONLY | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	else if (mode == T_APPEND)
		fd = (open(filename, O_CREAT | O_WRONLY | O_APPEND, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	else
	{
		printf("jissou miss : openfile mode error\n");
		exit(EXIT_FAILURE);
	}
	if (fd < 0)
		fprintf(stderr, "%s: No such file or directory\n", filename);
	return (fd);
}

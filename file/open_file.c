/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaou <kaou@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 17:10:56 by kaou              #+#    #+#             */
/*   Updated: 2022/07/07 17:58:14 by kaou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	openfile(char *filename, t_type_token mode)
{
	int	fd;

	if (mode == T_REDIR_IN)
	{
		if (access(filename, F_OK))
		{
			fprintf(stderr, "%s: No such file or directory\n", filename);
			exit(EXIT_FAILURE);
		}
		fd = (open(filename, O_RDONLY));
	}
	else if (mode == T_REDIR_OUT)
		fd = (open(filename, O_CREAT | O_WRONLY | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	else
		fd = (open(filename, O_CREAT | O_WRONLY | O_APPEND, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	if (fd < 0)
		fprintf(stderr, "%s: No such file or directory\n", filename);
	return (fd);
}

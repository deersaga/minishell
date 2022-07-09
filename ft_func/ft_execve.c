/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 13:56:47 by katakagi          #+#    #+#             */
/*   Updated: 2022/07/09 18:30:58 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_execve(char *file, char **argv, char **env)
{
	if (execve(file, argv, env) == -1)
	{
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}
}

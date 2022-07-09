/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 19:54:46 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:44:33 by katakagi         ###   ########.fr       */
=======
/*   By: ktada <ktada@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 19:54:46 by ktada             #+#    #+#             */
/*   Updated: 2022/07/09 18:35:01 by ktada            ###   ########.fr       */
>>>>>>> 334f157f5c1137db7fb1e380be1e52c241ce5b36
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_signal(int sig, void (*func)(int))
{
	if (signal(sig, func) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
}

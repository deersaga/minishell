/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katakagi <katakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada             #+#    #+#             */
/*   Updated: 2022/07/10 15:42:55 by katakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_storage(char **storage, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(storage[i]);
		i++;
	}
	free(storage);
	exit(1);
}

static char	**make_storage(char **storage, const char *str, char sep)
{
	size_t	i;
	size_t	j;
	int		count;

	i = 0;
	count = 0;
	while (str[i] != '\0')
	{
		if (str[i] != sep)
		{
			j = 0;
			while (str[i + j] != sep && str[i + j])
				j++;
			storage[count] = (char *)ft_malloc((j + 1) * sizeof(char));
			if (!storage[count])
				return (free_storage(storage, count));
			ft_strlcpy(storage[count], &str[i], j + 1);
			count++;
			i += j;
		}
		else
			i++;
	}
	storage[count] = 0;
	return (storage);
}

char	**ft_split(char const *str, char sep)
{
	char		**storage;
	size_t		i;
	size_t		count;

	if (!str)
		return (NULL);
	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != sep && (str[i + 1] == sep || str[i + 1] == '\0'))
			count++;
		i++;
	}
	storage = (char **)ft_malloc((count + 1) * sizeof(char *));
	return (make_storage(storage, str, sep));
}

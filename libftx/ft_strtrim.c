/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktada <ktada@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 17:01:00 by ktada              #+#    #+#             */
/*   Updated: 2022/07/07 17:03:15 by ktada             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s, char const *set)
{
	size_t	start;
	size_t	end;
	char	*ret;

	if (!s)
		return (NULL);
	if (*s == '\0' || !set)
		return (ft_strdup(s));
	start = 0;
	end = ft_strlen(s);
	while (ft_strchr(set, s[start]) != NULL)
		start++;
	while (ft_strchr(set, s[end]) != NULL && end > start)
		end--;
	if (end == start)
	{
		return (ft_substr(s, start, 1));
	}
	ret = ft_substr(s, start, end - start + 1);
	return (ret);
}

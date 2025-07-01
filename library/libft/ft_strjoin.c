/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:37:08 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/03/23 15:15:24 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		len;
	char	*joined;

	if (!s1 && !s2)
		return (ft_strdup(""));
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len = ft_strlen(s1) + ft_strlen(s2);
	joined = (char *)malloc((len + 1) * sizeof(char));
	if (joined == NULL)
		return (NULL);
	ft_memcpy(joined, s1, ft_strlen(s1));
	ft_memcpy((joined + ft_strlen(s1)), s2, ft_strlen(s2));
	joined[len] = '\0';
	return (joined);
}

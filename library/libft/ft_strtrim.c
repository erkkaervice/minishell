/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:31:25 by thi-mngu          #+#    #+#             */
/*   Updated: 2024/11/23 15:32:34 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_str(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		j;
	char	*newstr;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (s1[start] && is_in_str((char *)set, s1[start]))
		start++;
	while (end >= start && is_in_str((char *)set, s1[end]))
		end--;
	newstr = (char *)malloc((end - start + 2) * sizeof(char));
	if (newstr == NULL)
		return (NULL);
	j = 0;
	while (start <= end)
	{
		newstr[j] = s1[start];
		j++;
		start++;
	}
	newstr[j] = '\0';
	return (newstr);
}

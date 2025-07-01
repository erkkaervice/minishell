/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: core <core@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:21:49 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/08 14:06:36 by core             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(const char *s, char delimiter)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != delimiter && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == delimiter)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

static void	free_result(char **result, int count)
{
	while (count >= 0)
	{
		free(result[count]);
		count--;
	}
	free(result);
}

static int	populate_words(char **result, const char *s, char delimiter)
{
	int			word_index;
	const char	*start;

	word_index = 0;
	while (*s)
	{
		while (*s == delimiter)
			s++;
		if (*s)
		{
			start = s;
			while (*s && *s != delimiter)
				s++;
			result[word_index] = ft_substr(start, 0, s - start);
			if (!result[word_index++])
			{
				free_result(result, word_index - 1);
				return (0);
			}
		}
	}
	result[word_index] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		word_count;

	if (!s || !*s)
		return (NULL);
	word_count = count_word(s, c);
	if (word_count == 0)
		return (NULL);
	result = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!populate_words(result, s, c))
		return (NULL);
	result[word_count] = NULL;
	return (result);
}

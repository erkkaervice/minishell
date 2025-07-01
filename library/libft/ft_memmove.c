/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:12:16 by thi-mngu          #+#    #+#             */
/*   Updated: 2024/11/23 15:26:27 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	*move;

	if (n == 0 || (dest == NULL && src == NULL))
		return (dest);
	str = (unsigned char *)src;
	move = (unsigned char *)dest;
	i = -1;
	if (move < str)
	{
		while (++i < n)
			move[i] = str[i];
	}
	else
	{
		while (n > 0)
		{
			move[n - 1] = str[n - 1];
			n--;
		}
	}
	return (dest);
}

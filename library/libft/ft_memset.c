/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:15:19 by thi-mngu          #+#    #+#             */
/*   Updated: 2024/11/23 15:26:39 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*newptr;

	newptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		newptr[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

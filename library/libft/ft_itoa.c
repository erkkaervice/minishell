/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:29:27 by thi-mngu          #+#    #+#             */
/*   Updated: 2024/11/23 15:24:08 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	is_negative(long n)
{
	if (n < 0)
		return (1);
	return (0);
}

static	int	num_len(long n)
{
	int		len;

	len = 0;
	if (n == 0)
		return (1);
	if (is_negative(n))
		n = -n;
	while (n > 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	long	num;
	char	*str;

	len = num_len(n);
	num = n;
	if (num == 0)
		return (ft_strdup("0"));
	if (is_negative(num) == 1)
	{
		num = -num;
		len++;
	}
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	while (num > 0)
	{
		str[len--] = (num % 10) + '0';
		num = num / 10;
	}
	if (is_negative(n))
		str[0] = '-';
	return (str);
}

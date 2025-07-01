/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:47:00 by thi-mngu          #+#    #+#             */
/*   Updated: 2024/11/23 15:25:38 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*current_node;

	current_node = malloc(sizeof(t_list));
	if (current_node == NULL)
		return (NULL);
	current_node->content = content;
	current_node->next = NULL;
	return (current_node);
}

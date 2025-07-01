/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:24:59 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/07/01 16:55:53 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Signal handler for SIGINT in non-interactive mode.
 * Simply moves cursor to new line and sets status 130.
 */
void	signal_sigint(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
	g_sigstatus = 130;
}

/*
 * Signal handler for SIGQUIT in non-interactive mode.
 * Prints "Quit: 3" to stderr and sets status 131.
 */
void	signal_sigquit(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	g_sigstatus = 131;
}

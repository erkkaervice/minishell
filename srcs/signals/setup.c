/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:36:18 by eala-lah          #+#    #+#             */
/*   Updated: 2025/07/01 16:55:55 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_sigstatus = 0;

/*
 * Handle errors during sigaction calls.
 * Prints perror message and exits with failure.
 */
static void	handle_sigaction_error(const char *func_name)
{
	perror(func_name);
	exit(EXIT_FAILURE);
}

/*
 * Signal handler for SIGINT in interactive mode.
 * Prints newline, clears input line, redraws prompt,
 * and sets global status to 130.
 */
static void	reset_prompt(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_sigstatus = 130;
}

/*
 * Set up signal handlers for interactive mode.
 * SIGINT is handled by reset_prompt.
 * SIGQUIT is ignored.
 * Exits on sigaction error.
 */
void	signal_interactive(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &reset_prompt;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		handle_sigaction_error("sigaction SIGINT interactive");
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	handle_sigaction_error("sigaction SIGQUIT interactive");
}

/*
 * Set up signal handlers for non-interactive mode.
 * SIGINT and SIGQUIT have custom handlers.
 * Exits on sigaction error.
 */
void	signal_noninteractive(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = &signal_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		handle_sigaction_error("sigaction SIGINT noninteractive");
	memset(&sa_quit, 0, sizeof(sa_quit));
	sa_quit.sa_handler = &signal_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		handle_sigaction_error("sigaction SIGQUIT noninteractive");
}

/*
 * Updates shell exit_code from global signal status,
 * then resets the global signal status.
 */
void	update_shell_exit_code_from_signal(t_shell *shell)
{
	if (!shell)
		return;
	if (g_sigstatus != 0)
	{
		shell->exit_code = g_sigstatus;
		g_sigstatus = 0;
	}
}

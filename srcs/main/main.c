/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:05:53 by eala-lah          #+#    #+#             */
/*   Updated: 2025/07/01 18:59:43 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Program entry point: initializes shell and runs REPL loop.
 */
int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)ac;
	(void)av;
	g_sigstatus = 0;
	signal_interactive();
	initialize_shell(&shell, envp);
	while (1)
	{
		line = read_and_prepare_input();
		update_shell_exit_code_from_signal(&shell);
		if (!line)
			break ;
		process_input_line(line, &shell);
	}
	exit_shell(&shell, shell.exit_code);
}

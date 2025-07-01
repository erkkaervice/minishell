/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:02:24 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/24 18:02:26 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Prints syntax error message for the given token to stderr.
 */
void	print_syntax_error(char *token)
{
	ft_putstr_fd(
		"minishell: syntax error near unexpected token ", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

/*
 * Returns 1 if line is a lone redirection operator (>, >>, <, <<)
 * without any following tokens, else 0.
 */
int	is_lone_redirection(char *line)
{
	char	c;

	while (*line && ft_isspace(*line))
		line++;
	if (*line == '>' || *line == '<')
	{
		c = *line;
		while (*line == c)
			line++;
		if (*line == c)
			line++;
		while (*line && ft_isspace(*line))
			line++;
		if (!*line)
			return (1);
	}
	return (0);
}

/*
 * Returns 1 if the input line is "." or ".." commands,
 * which are considered invalid for this shell.
 */
int	is_invalid_command(char *line)
{
	while (*line && ft_isspace(*line))
		line++;
	if (!ft_strncmp(line, ".", 2) || !ft_strncmp(line, "..", 3))
		return (1);
	return (0);
}

/*
 * Checks input line for syntax errors or invalid commands.
 * Sets shell exit_code accordingly.
 * Returns 1 if an error was found, else 0.
 */
int	check_input_errors(char *line, t_shell *shell)
{
	if (line_is_syntax_error(line))
	{
		print_syntax_error("newline");
		shell->exit_code = 2;
		return (1);
	}
	if (is_lone_redirection(line))
	{
		print_syntax_error("newline");
		shell->exit_code = 126;
		return (1);
	}
	if (is_invalid_command(line))
	{
		ft_putstr_fd("minishell: command not found\n", STDERR_FILENO);
		shell->exit_code = 127;
		return (1);
	}
	return (0);
}

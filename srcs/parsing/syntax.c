/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:04:53 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/24 17:08:34 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Check redirection syntax starting at *i in line:
 * - Counts consecutive redirection chars (max 2 allowed).
 * - Checks invalid sequences like redirection followed by pipe or 
 *   different redirection.
 * - Updates *i to last redirection char position.
 * Returns 1 on syntax error, 0 otherwise.
 */
static int	check_redirection(char *line, int *i)
{
	int		count;
	char	c;
	int		j;

	count = 1;
	c = line[*i];
	j = *i + 1;
	while (line[j] && line[j] == c)
	{
		count++;
		j++;
	}
	if (count > 2)
		return (1);
	if ((c == '<' || c == '>') && (line[j] == '|'
			|| (line[j] && (line[j] == '<' || line[j] == '>') && line[j] != c)))
		return (1);
	*i = j - 1;
	return (0);
}

/*
 * Check pipe and pipe sequencing at *i in line:
 * - If pipe found and previous char was pipe, error.
 * - Otherwise, update pipe_prev flag.
 * Returns 1 on syntax error, 0 otherwise.
 */
static int	check_pipe_and_end(char *line, int *i, int *pipe_prev)
{
	if (line[*i] == '|')
	{
		if (*pipe_prev)
			return (1);
		*pipe_prev = 1;
	}
	else if (!ft_isspace(line[*i]))
		*pipe_prev = 0;
	return (0);
}

/*
 * Process syntax at position *i in line:
 * - Handle quoted strings by skipping to closing quote.
 * - Handle pipes and check sequencing errors.
 * - Handle redirection syntax.
 * Updates *i and pipe_prev accordingly.
 * Returns 1 on syntax error, 0 otherwise.
 */
static int	process_syntax_pos(char *line, int *i, int *pipe_prev)
{
	char	quote;
	int		j;

	if (line[*i] == '\'' || line[*i] == '"')
	{
		quote = line[*i];
		j = *i + 1;
		while (line[j] && line[j] != quote)
			j++;
		if (!line[j])
			return (1);
		*i = j;
		*pipe_prev = 0;
	}
	else if (check_pipe_and_end(line, i, pipe_prev))
		return (1);
	else if (line[*i] == '<' || line[*i] == '>')
	{
		if (check_redirection(line, i))
			return (1);
		*pipe_prev = 0;
	}
	return (0);
}

/*
 * Check if line ends with invalid syntax:
 * - Trailing pipe or redirection causes error.
 * - pipe_prev flag indicates unfinished pipe.
 * Returns 1 on syntax error, 0 otherwise.
 */
static int	check_line_end(char *line, int pipe_prev)
{
	int	i;

	i = ft_strlen(line) - 1;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (i >= 0 && (line[i] == '|' || line[i] == '<' || line[i] == '>'))
		return (1);
	if (pipe_prev)
		return (1);
	return (0);
}

/*
 * Check if input line contains syntax errors:
 * - Iterates through line chars and processes syntax positions.
 * - Returns 1 if any syntax error found, 0 if line is valid.
 */
int	line_is_syntax_error(char *line)
{
	int	i;
	int	pipe_prev;

	if (!line)
		return (0);
	i = 0;
	pipe_prev = 0;
	while (line[i])
	{
		if (process_syntax_pos(line, &i, &pipe_prev))
			return (1);
		i++;
	}
	return (check_line_end(line, pipe_prev));
}

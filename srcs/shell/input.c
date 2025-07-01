/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:24:06 by eala-lah          #+#    #+#             */
/*   Updated: 2025/07/01 19:46:08 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Removes leading and trailing whitespace.
 * Returns newly allocated trimmed string.
 */
char	*trim_whitespace(char *line)
{
	int		start;
	int		end;

	if (!line)
		return (NULL);
	start = 0;
	while (line[start] && ft_isspace(line[start]))
		start++;
	end = ft_strlen(line) - 1;
	while (end >= start && ft_isspace(line[end]))
		end--;
	if (end < start)
		return (ft_strdup(""));
	return (ft_substr(line, start, end - start + 1));
}

/*
 * Parses input line into commands and tokens,
 * performs environment and status expansion.
 * Returns parsed commands or NULL on error.
 */
static t_cmd	*parse_input_line(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;

	shell->hd_cnt = 0;
	tokens = ft_tokenize(line, shell);
	free(line);
	if (!tokens || shell->hd_cnt > 1)
	{
		if (shell->hd_cnt > 1)
			ft_putendl_fd("Do not support multiple heredoc", 2);
		shell->exit_code = 127;
		if (tokens)
			ft_free_tokens(tokens);
		return (NULL);
	}
	expand_env_and_status(tokens, shell);
	cmds = ft_parse(tokens, shell);
	ft_free_tokens(tokens);
	if (!cmds)
		shell->exit_code = 2;
	return (cmds);
}

/*
 * Executes the parsed commands pipeline.
 * Frees commands after execution.
 */
static void	parse_and_execute(char *line, t_shell *shell)
{
	t_cmd	*cmds;

	cmds = parse_input_line(line, shell);
	if (!cmds)
		return ;
	shell->cmds = cmds;
	execute_pipeline(shell, cmds);
	ft_free_cmds(cmds);
	shell->cmds = NULL;
}

/*
 * Processes the input line with syntax and command checks,
 * executes commands if valid.
 */
void	process_input_line(char *line, t_shell *shell)
{
	int	err;

	if (!line)
		return ;
	if (line[0] == '\0')
	{
		free(line);
		return ;
	}
	err = check_input_errors(line, shell);
	if (err)
	{
		free(line);
		return ;
	}
	parse_and_execute(line, shell);
}


/*
 * Reads input line from user, trims whitespace,
 * adds to history if non-empty.
 * Returns allocated trimmed string or NULL on EOF.
 */
char	*read_and_prepare_input(void)
{
	char	*line;
	char	*trimmed;

	if (isatty(STDIN_FILENO))
		line = readline("minishell$ ");
	else
		line = readline(NULL);
	if (!line)
		return (NULL);
	trimmed = trim_whitespace(line);
	free(line);
	if (trimmed && trimmed[0] != '\0' && isatty(STDIN_FILENO))
		add_history(trimmed);
	return (trimmed);
}


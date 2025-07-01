/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:32:50 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/19 17:36:37 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Extract an alphanumeric or underscore variable name starting at input[*i].
 * Used inside single quotes to handle '$' literally.
 * Returns a new token of type T_WORD with qtype set to '\''.
 */
t_token	*ft_handle_dollar_in_quotes(char *input, int *i, t_shell *shell)
{
	int		start;
	char	*value;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (handle_alloc_failure(shell));
	return (ft_check_token_creation(value, T_WORD, '\''));
}

/*
 * Handle quoted strings (single or double quotes).
 * If quote is unmatched, prints syntax error and returns NULL.
 * Returns a T_WORD token with the content inside quotes.
 */
t_token	*ft_handle_quote(char *input, int *i, t_shell *shell)
{
	int		start;
	char	quote;
	char	*value;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd(
			"minishell: line 1: unexpected EOF while "
			"looking for matching `", 2);
		write(2, &quote, 1);
		ft_putstr_fd("'\n", 2);
		shell->exit_code = 2;
		return (NULL);
	}
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (handle_alloc_failure(shell));
	(*i)++;
	return (ft_check_token_creation(value, T_WORD, quote));
}

/*
 * Helper for ft_handle_dollar when input[*i] == '?'.
 * Consumes the '?' and any trailing alnum or underscore chars.
 */
static t_token	*handle_dollar_question(t_tokdata *data,
	int start, char qtype)
{
	char	*value;

	(*data->i)++;
	while (data->input[*data->i]
		&& (ft_isalnum(data->input[*data->i]) || data->input[*data->i] == '_'))
		(*data->i)++;
	value = ft_substr(data->input, start, *data->i - start);
	if (!value)
		return (handle_alloc_failure(data->shell));
	return (ft_check_token_creation(value, T_WORD, qtype));
}

/*
 * Helper for ft_handle_dollar when character after '$' is invalid.
 * Returns a T_WORD token containing "$".
 */
static t_token	*handle_dollar_invalid(t_shell *shell)
{
	char	*value;

	value = ft_strdup("$");
	if (!value)
		return (handle_alloc_failure(shell));
	return (ft_check_token_creation(value, T_WORD, '\0'));
}

/*
 * Handle dollar expansions starting with '$'.
 * Handles special case '$?' for exit status.
 * Returns T_EXIT_STATUS or T_ENV_VAR tokens accordingly.
 */
t_token	*ft_handle_dollar(char *input, int *i, char qtype, t_shell *shell)
{
	t_tokdata	td;
	int			start;
	char		*value;

	if (qtype == '\'')
		return (ft_handle_dollar_in_quotes(input, i, shell));
	start = *i;
	(*i)++;
	if (input[*i] == '?')
	{
		td.input = input;
		td.i = i;
		td.shell = shell;
		return (handle_dollar_question(&td, start, qtype));
	}
	if (!input[*i] || (!ft_isalnum(input[*i]) && input[*i] != '_'))
		return (handle_dollar_invalid(shell));
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (handle_alloc_failure(shell));
	return (ft_check_token_creation(value, T_ENV_VAR, qtype));
}

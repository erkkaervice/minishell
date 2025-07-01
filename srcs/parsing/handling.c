/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 15:27:40 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/19 17:37:34 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Handle allocation failure by setting global status, printing perror,
 * and returning NULL to signal failure.
 */
t_token	*handle_alloc_failure(t_shell *shell)
{
	shell->exit_code = 1;
	g_sigstatus = 1;
	perror("minishell");
	return (NULL);
}

/*
 * Handle pipe token '|'.
 * Advances index and returns a T_PIPE token containing "|".
 */
t_token	*ft_handle_pipe(int *i, t_shell *shell)
{
	char	*value;
	t_token	*token;

	value = ft_strdup("|");
	if (!value)
	{
		shell->exit_code = 1;
		return (handle_alloc_failure(shell));
	}
	*i = *i + 1;
	token = ft_check_token_creation(value, T_PIPE, '\0');
	return (token);
}

/*
 * Handle redirection operators: '<', '>', '<<', '>>'.
 * Advances index accordingly and returns a T_REDIR token with the operator.
 */
t_token	*ft_handle_operator(char *input, int *i, t_shell *shell)
{
	char	*value;
	t_token	*token;

	if (input[*i + 1] && input[*i + 1] == input[*i])
	{
		value = ft_substr(input, *i, 2);
		if (!value)
			return (handle_alloc_failure(shell));
		*i = *i + 2;
	}
	else
	{
		value = ft_substr(input, *i, 1);
		if (!value)
			return (handle_alloc_failure(shell));
		*i = *i + 1;
	}
	token = ft_check_token_creation(value, T_REDIR, '\0');
	return (token);
}

/*
 * Helper to process a non-quote, non-dollar part of a word.
 * Collects characters until whitespace, operator, quote, or dollar.
 */
static t_token	*handle_word_part(char *input, int *i,
	char qtype, t_shell *shell)
{
	int		start;
	char	*part;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
		&& !ft_isquote(input[*i]) && input[*i] != '$')
		(*i)++;
	part = ft_substr(input, start, *i - start);
	if (!part)
		return (handle_alloc_failure(shell));
	return (ft_check_token_creation(part, T_WORD, qtype));
}

/*
 * Handle unquoted word tokens.
 * Collect characters until whitespace, operator, quote, or dollar.
 * Returns a T_WORD token with the collected substring.
 */
t_token	*ft_handle_word(char *input, int *i, char qtype, t_shell *shell)
{
	t_token	*final;
	t_token	*tok;

	final = NULL;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		if (qtype != '\'' && ft_isquote(input[*i]))
			tok = ft_handle_quote(input, i, shell);
		else if (qtype != '\'' && input[*i] == '$')
			tok = ft_handle_dollar(input, i, qtype, shell);
		else
			tok = handle_word_part(input, i, qtype, shell);
		if (!tok)
		{
			ft_free_tokens(final);
			return (NULL);
		}
		if (!final)
			final = tok;
		else
			final = ft_merge_tokens(final, tok);
	}
	return (final);
}

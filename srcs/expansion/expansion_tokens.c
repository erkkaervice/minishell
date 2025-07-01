/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 20:30:00 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/24 17:06:42 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Print a syntax error message to stderr:
 * - Displays token->value if available,
 *   otherwise prints "newline".
 */
void	ft_print_syntax_error_token(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near `", 2);
	if (token && token->value)
		ft_putstr_fd(token->value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
}

/*
 * Replace a T_ENV_VAR token's value by looking it up in shell->envp.
 * Convert the token to T_WORD.
 */
void	expand_env_var(t_token *tok, t_shell *shell)
{
	char	*val;

	val = get_env_val(shell, tok->value);
	free(tok->value);
	if (val)
		tok->value = ft_strdup(val);
	else
		tok->value = ft_strdup("");
	if (!tok->value)
		shell->exit_code = 1;
	tok->type = T_WORD;
}

/*
 * Replace a T_EXIT_STATUS token's value with the shell's
 * exit_code string. Convert the token to T_WORD.
 */
void	expand_exit_status(t_token *tok, t_shell *shell)
{
	char	*status;

	status = ft_itoa(shell->exit_code);
	free(tok->value);
	tok->value = status;
	tok->type = T_WORD;
	if (!status)
		shell->exit_code = 1;
}

/*
 * Fully expand a T_WORD token (which may contain '$')
 * by invoking expand_word on its value.
 */
void	expand_word_token(t_token *tok, t_shell *shell)
{
	char	*new;

	new = expand_word(tok->value, shell);
	free(tok->value);
	if (!new)
	{
		tok->value = ft_strdup("");
		shell->exit_code = 1;
	}
	else
		tok->value = new;
}

/*
 * Walk the linked list of tokens. For each token not
 * single-quoted, dispatch to the appropriate expander.
 */
void	expand_env_and_status(t_token *token, t_shell *shell)
{
	while (token)
	{
		if (token->qtype != '\'')
		{
			if (token->type == T_ENV_VAR)
				expand_env_var(token, shell);
			else if (token->type == T_EXIT_STATUS)
				expand_exit_status(token, shell);
			else if (token->type == T_WORD)
				expand_word_token(token, shell);
		}
		token = token->next;
	}
}

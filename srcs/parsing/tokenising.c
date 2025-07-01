/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenising.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:10:41 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/25 14:23:58 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Merge two tokens by concatenating their values into the first token.
 * Frees the second token.
 */
t_token	*ft_merge_tokens(t_token *a, t_token *b)
{
	char	*merged_value;
	size_t	len_a;
	size_t	len_b;

	if (!a)
		return (b);
	if (!b)
		return (a);
	len_a = ft_strlen(a->value);
	len_b = ft_strlen(b->value);
	merged_value = malloc(len_a + len_b + 1);
	if (!merged_value)
	{
		ft_free_tokens(a);
		ft_free_tokens(b);
		return (NULL);
	}
	ft_strcpy(merged_value, a->value);
	ft_strcpy(merged_value + len_a, b->value);
	free(a->value);
	a->value = merged_value;
	a->next = b->next;
	free(b->value);
	free(b);
	return (a);
}

/*
 * Create a token with the specified value, type, and quote type.
 * If allocation fails, frees the provided value and returns NULL.
 */
t_token	*ft_check_token_creation(char *value, t_token_type type, char qtype)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
	{
		free(value);
		return (NULL);
	}
	token->value = value;
	token->type = type;
	token->qtype = qtype;
	token->next = NULL;
	return (token);
}

/*
 * Parse the next token from the input string, advancing the index.
 * Supports pipes, redirections, quotes, environment variables, and words.
 * Passes shell context to all token handlers.
 */
static t_token	*ft_next_token(char *in, int *i, t_shell *shell)
{
	while (ft_isspace(in[*i]))
		(*i)++;
	if (!in[*i])
		return (NULL);
	if ((in[*i] == '|' && in[*i + 1] == '|')
		|| (in[*i] == '&' && in[*i + 1] == '&')
		|| in[*i] == ';')
	{
		ft_putstr_fd("minishell: syntax error near `", 2);
		write(2, &in[*i], (in[*i + 1] == in[*i]) ? 2 : 1);
		ft_putstr_fd("`\n", 2);
		shell->exit_code = 2;
		return (NULL);
	}
	if (in[*i] == '|')
		return (ft_handle_pipe(i, shell));
	if (in[*i] == '<' || in[*i] == '>')
		return (ft_handle_operator(in, i, shell));
	if (in[*i] == '$')
		return (ft_handle_dollar(in, i, '\0', shell));
	return (ft_handle_word(in, i, '\0', shell));
}

/*
 * Retrieve the next token and append it to the token list.
 * On failure, frees the current list and returns 0, else returns 1.
 */
static int	ft_add_next_token(char *input, int *i,
	t_token_list *list, t_shell *shell)
{
	t_token	*token;
	t_token	*iter;

	token = ft_next_token(input, i, shell);
	if (!token)
	{
		ft_free_tokens(*(list->first));
		*(list->first) = NULL;
		return (0);
	}
	if (!*(list->first))
		*(list->first) = token;
	iter = token;
	while (iter)
	{
		if (*(list->current))
			(*(list->current))->next = iter;
		*(list->current) = iter;
		iter = iter->next;
	}
	return (1);
}

/*
 * Convert the input string into a linked list of tokens.
 * Frees and returns NULL on error, else returns head of token list.
 * Requires shell context to be passed.
 */
t_token	*ft_tokenize(char *input, t_shell *shell)
{
	t_token_list	list;
	t_token			*first;
	t_token			*current;
	int				i;

	if (!input)
		return (NULL);
	first = NULL;
	current = NULL;
	list.first = &first;
	list.current = &current;
	i = 0;
	while (input[i])
		if (!ft_add_next_token(input, &i, &list, shell))
			return (NULL);
	return (first);
}

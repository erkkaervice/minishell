/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:15:04 by eala-lah          #+#    #+#             */
/*   Updated: 2025/07/01 17:29:40 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Look up NAME in shell->envp (format KEY=VALUE).
 * Returns pointer to VALUE or NULL if not found.
 */
char	*get_env_val(t_shell *shell, const char *name)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (shell->envp[i])
	{
		if (!ft_strncmp(shell->envp[i], name, len)
			&& shell->envp[i][len] == '=')
			return (&shell->envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

/*
 * Parse a variable name or '?' at in[*i], advancing *i past it, and return it.
 */
static char	*get_var_name(const char *in, int *i)
{
	int	start;

	start = *i;
	if (in[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (!(ft_isalpha(in[*i]) || in[*i] == '_'))
		return (NULL);
	(*i)++;
	while (ft_isalnum(in[*i]) || in[*i] == '_')
		(*i)++;
	return (ft_substr(in, start, *i - start));
}

/*
 * Expand one $-reference: either $? or $KEY.
 * Advances *i past the name and returns the expansion.
 */
static char	*expand_var(const char *in, int *i, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*found;

	(*i)++;
	name = get_var_name(in, i);
	if (!name)
		return (ft_strdup("$"));
	if (name[0] == '?' && name[1] == '\0')
		value = ft_itoa(shell->exit_code);
	else
	{
		found = get_env_val(shell, name);
		if (found)
			value = ft_strdup(found);
		else
			value = ft_strdup("");
	}
	free(name);
	return (value);
}

/*
 * Return next chunk: if at '$' do expand_var(), else
 * return literal up to next '$'. Advances *i accordingly.
 */
static char	*get_next_chunk(const char *in, int *i, t_shell *shell)
{
	int	start;

	start = *i;
	if (in[*i] == '$')
		return (expand_var(in, i, shell));
	while (in[*i] && in[*i] != '$')
		(*i)++;
	return (ft_substr(in, start, *i - start));
}

/*
 * Build the fully expanded string by concatenating chunks
 * until the end of input.
 */
char	*expand_word(const char *in, t_shell *shell)
{
	char	*res;
	char	*chunk;
	char	*tmp;
	int		i;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (in[i])
	{
		chunk = get_next_chunk(in, &i, shell);
		if (!chunk)
		{
			free(res);
			return (NULL);
		}
		tmp = res;
		res = ft_strjoin(tmp, chunk);
		free(tmp);
		free(chunk);
		if (!res)
			return (NULL);
	}
	return (res);
}

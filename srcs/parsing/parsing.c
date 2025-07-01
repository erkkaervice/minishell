/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:56:52 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/24 17:08:11 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Count how many arguments should be in cmd_table.
 * Counts tokens of types T_WORD, T_ENV_VAR, and T_EXIT_STATUS
 * before a pipe token, skipping redirection operators and their targets.
 */
static int	ft_count_args(t_token *token)
{
	int	count;

	if (!token)
		return (0);
	count = 0;
	while (token && token->type != T_PIPE)
	{
		if ((token->type == T_WORD || token->type == T_ENV_VAR
				|| token->type == T_EXIT_STATUS)
			&& token->value && token->value[0] != '\0')
		{
			count++;
			token = token->next;
		}
		else if (token->type == T_REDIR)
		{
			token = token->next;
			if (token && token->type == T_WORD)
				token = token->next;
		}
		else
			token = token->next;
	}
	return (count);
}

/*
 * Fill av array with copies of token values for n tokens.
 * Includes only tokens of type T_WORD, T_ENV_VAR, or T_EXIT_STATUS.
 * Skips redirection tokens and their targets.
 * Returns number of arguments added, or -1 on allocation failure.
 */
int	ft_add_args(char **av, int n, t_token **t)
{
	int	i;

	i = 0;
	while (*t && (*t)->type != T_PIPE && i < n)
	{
		if (((*t)->type == T_WORD || (*t)->type == T_ENV_VAR
				|| (*t)->type == T_EXIT_STATUS)
			&& (*t)->value && (*t)->value[0])
		{
			av[i] = ft_substr((*t)->value, 0, ft_strlen((*t)->value));
			if (!av[i])
				return (-1);
			i++;
		}
		if ((*t)->type == T_REDIR && (*t)->next)
			*t = (*t)->next->next;
		else
			*t = (*t)->next;
	}
	return (i);
}

/*
 * Initialize a t_cmd struct fields:
 * - Set redir, pipe, and next to defaults.
 * - Build cmd_table from tokens.
 * - Set cmd_name to first argument string.
 * - Mark is_builtin according to cmd_name.
 * On failure, cleans up partially allocated resources.
 */
static void	ft_init_cmd(t_cmd *cmd, int n, t_token *token)
{
	if (!cmd || n < 0 || !token)
		return ;
	cmd->redir = NULL;
	cmd->pipe = 0;
	cmd->next = NULL;
	cmd->cmd_table = ft_build_av(token, n);
	if (cmd->cmd_table)
	{
		cmd->cmd_name = ft_strdup(cmd->cmd_table[0]);
		if (!cmd->cmd_name)
		{
			ft_free_av(cmd->cmd_table, n);
			cmd->cmd_table = NULL;
			cmd->is_builtin = 0;
			return ;
		}
		cmd->is_builtin = is_builtin(cmd->cmd_name);
	}
	else
	{
		cmd->cmd_name = NULL;
		cmd->is_builtin = 0;
	}
}

/*
 * Allocate and fill a t_cmd struct from token list.
 * Checks for syntax errors (empty command or pipe at start).
 * Returns NULL on syntax error or allocation failure.
 * Updates token pointer past processed tokens.
 */
static t_cmd	*ft_alloc_cmd(t_token **token, t_shell *shell)
{
	t_cmd	*cmd;
	int		n;

	n = ft_count_args(*token);
	if (n == 0)
	{
		if (!*token || (*token)->type == T_PIPE)
		{
			ft_print_syntax_error_token(*token);
			shell->exit_code = 2;
			return (NULL);
		}
		return (NULL);
	}
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_init_cmd(cmd, n, *token);
	if (!cmd->cmd_table || ft_cmd_redirs_and_pipe(cmd, token))
	{
		shell->exit_code = 2;
		return (ft_free_cmds(cmd), NULL);
	}
	shell->exit_code = 0;
	return (cmd);
}

/*
 * Parse token list into linked list of commands.
 * Each command holds arguments, redirections, and pipe flag.
 * Frees all commands and returns NULL on failure.
 */
t_cmd	*ft_parse(t_token *tokens, t_shell *shell)
{
	t_cmd	*first;
	t_cmd	*current;
	t_cmd	*node;

	first = NULL;
	current = NULL;
	while (tokens)
	{
		node = ft_alloc_cmd(&tokens, shell);
		if (!node)
		{
			ft_free_cmds(first);
			return (NULL);
		}
		if (!first)
			first = node;
		else
			current->next = node;
		current = node;
	}
	return (first);
}

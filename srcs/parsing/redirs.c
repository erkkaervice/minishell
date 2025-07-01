/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:08:01 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/24 16:40:05 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Parse redirections and pipe for a single command.
 * Updates the cmd's redir list and pipe flag.
 * Advances the token pointer past redirs and optional pipe.
 * Returns 1 on failure, 0 on success.
 */
int	ft_cmd_redirs_and_pipe(t_cmd *cmd, t_token **token)
{
	if (!cmd || !token)
		return (0);
	if (ft_collect_redirs(cmd, token))
		return (1);
	if (*token && (*token)->type == T_PIPE)
	{
		cmd->pipe = 1;
		(*token) = (*token)->next;
	}
	return (0);
}

/*
 * Print a syntax error related to a redirection token.
 * Frees the given redir list and sets global exit status to 258.
 * Returns 1 to signal error.
 */
static int	ft_redir_error(t_token *token, t_redir *head)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!token || !token->next)
		ft_putstr_fd("newline", 2);
	else if (token->next->value)
		ft_putstr_fd(token->next->value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	g_sigstatus = 2;
	ft_free_redirs(head);
	return (1);
}

/*
 * Create and initialize a new t_redir node from the current redir token.
 * Duplicates the target filename from the next token.
 * Returns the new node or NULL on allocation failure.
 */
static t_redir	*ft_redir_new(t_token *token)
{
	t_redir	*node;

	if (!token || !token->next)
		return (NULL);
	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	if (ft_strncmp(token->value, "<<", 2) == 0)
		node->type = REDIR_HEREDOC;
	else if (ft_strncmp(token->value, ">>", 2) == 0)
		node->type = REDIR_APPEND;
	else if (ft_strncmp(token->value, "<", 1) == 0)
		node->type = REDIR_IN;
	else
		node->type = REDIR_OUT;
	node->filename = ft_strdup(token->next->value);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

/*
 * Append a redirection node to the end of the redir list.
 * Updates both head and tail pointers accordingly.
 */
void	ft_append_redir_node(t_redir **head, t_redir **tail, t_redir *node)
{
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
}

/*
 * Extract all redirection tokens from the current command segment.
 * Builds and stores a linked list of t_redir nodes in cmd->redir.
 * Validates redir syntax and advances the token pointer.
 * Returns 1 on error, 0 on success.
 */
int	ft_collect_redirs(t_cmd *cmd, t_token **token)
{
	t_redir	*head;
	t_redir	*tail;
	t_redir	*node;

	if (!cmd || !token)
		return (0);
	head = NULL;
	tail = NULL;
	while (*token && (*token)->type != T_PIPE)
	{
		if ((*token)->type == T_REDIR)
		{
			if (!(*token)->next || (*token)->next->type != T_WORD)
				return (ft_redir_error(*token, head));
			node = ft_redir_new(*token);
			if (!node)
				return (ft_free_redirs(head), 1);
			ft_append_redir_node(&head, &tail, node);
			(*token) = (*token)->next;
		}
		(*token) = (*token)->next;
	}
	cmd->redir = head;
	return (0);
}

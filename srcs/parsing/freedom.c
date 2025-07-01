/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freedom.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:25:27 by eala-lah          #+#    #+#             */
/*   Updated: 2025/07/01 17:20:04 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Allocate and fill a NULL-terminated array of strings (cmd_table)
 * from tokens, including only command arguments (T_WORD, T_ENV_VAR,
 * T_EXIT_STATUS). Skips redirection operators and their following
 * filenames.
 */
char	**ft_build_av(t_token *t, int n)
{
	char	**av;
	int		i;

	if (!t || n < 0)
		return (NULL);
	av = malloc(sizeof(char *) * (n + 1));
	if (!av)
		return (NULL);
	i = ft_add_args(av, n, &t);
	if (i == -1)
		return (ft_free_av(av, i), NULL);
	av[i] = NULL;
	return (av);
}

/*
 * Free an entire linked list of tokens and their stored strings.
 */
void	ft_free_tokens(t_token *token)
{
	t_token	*next;

	if (!token)
		return ;
	while (token)
	{
		next = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = next;
	}
}

/*
 * Free a partially filled argv array of strings.
 */
void	ft_free_av(char **av, int filled)
{
	int	i;

	if (!av || filled <= 0)
	{
		free(av);
		return ;
	}
	i = 0;
	while (i < filled)
	{
		if (av[i])
			free(av[i]);
		i++;
	}
	free(av);
}

/*
 * Free an entire linked list of redirection structures and their stored
 * filenames.
 */
void	ft_free_redirs(t_redir *redir)
{
	t_redir	*next;

	if (!redir)
		return ;
	while (redir)
	{
		next = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = next;
	}
}

/*
 * Free an entire linked list of command structures, including their command 
 * tables and redirections.
 */
void	ft_free_cmds(t_cmd *cmd)
{
	t_cmd	*next;
	int		i;

	if (!cmd)
		return ;
	while (cmd)
	{
		next = cmd->next;
		if (cmd->cmd_name)
			free(cmd->cmd_name);
		if (cmd->cmd_table)
		{
			i = 0;
			while (cmd->cmd_table[i])
			{
				free(cmd->cmd_table[i]);
				i++;
			}
			free(cmd->cmd_table);
		}
		ft_free_redirs(cmd->redir);
		free(cmd);
		cmd = next;
	}
}

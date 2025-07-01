/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:50:24 by eala-lah          #+#    #+#             */
/*   Updated: 2025/07/01 17:35:21 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Duplicates the environment variable array.
 * Returns newly allocated array or NULL on failure.
 */
char	**ft_copy_envp(char **arr)
{
	int		i;
	char	**copy;

	i = 0;
	if (!arr || !*arr)
		return (NULL);
	while (arr[i])
		i++;
	copy = (char **)malloc(sizeof(char *) * (i + 1));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		copy[i] = ft_strdup(arr[i]);
		if (!copy[i])
		{
			ft_free_arr_2d(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

/*
 * Initializes the shell struct:
 * - copies environment variables
 * - sets command list to NULL
 * - sets exit code to 0
 * - sets path_dirs to NULL
 */
void	initialize_shell(t_shell *shell, char **envp)
{
	if (!shell)
		return;
	if (!envp || !*envp)
	{
		shell->envp = malloc(sizeof(char *));
		if (!shell->envp)
		{
			ft_putendl_fd("minishell: fatal: malloc failed", 2);
			exit(shell->exit_code = 1);
		}
		shell->envp[0] = NULL;
	}
	else
	{
		shell->envp = ft_copy_envp(envp);
		if (!shell->envp)
		{
			ft_putendl_fd("minishell: fatal: failed to copy environment", 2);
			exit(shell->exit_code = 1);
		}
	}
	shell->cmds = NULL;
	shell->exit_code = 0;
	shell->path_dirs = NULL;
}

/*
 * Frees all shell allocated resources:
 * environment, path directories, and command list.
 */
void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->envp)
		ft_free_arr_2d(shell->envp);
	if (shell->path_dirs)
		ft_free_arr_2d(shell->path_dirs);
	if (shell->cmds)
		ft_free_cmds(shell->cmds);
}

/*
 * Cleans up shell resources and exits
 * with the provided exit code.
 */
void	exit_shell(t_shell *shell, int exit_code)
{
	if (!shell)
		return ;
	cleanup_shell(shell);
	exit(exit_code);
}

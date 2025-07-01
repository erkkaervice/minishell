/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:53:31 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 18:08:23 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	*init_pids(int num_cmds)
{
	pid_t	*pids;
	int		i;

	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
		return (NULL);
	i = 0;
	while (i < num_cmds)
		pids[i++] = -1;
	return (pids);
}

static void	handle_fork_error(pid_t *pids, int **pipes, int i)
{
	cleanup_processes(pids, i);
	free(pids);
	cleanup_pipes(pipes, i);
	ft_free_pipes(pipes, i);
}

static int	create_helper(int num_cmds, t_shell *shell,
			t_cmd *current, int **pipes)
{
	execute_child_process(current, shell, pipes, num_cmds - 1);
	ft_free_cmds(current);
	return (0);
}

pid_t	*create_process(int num_cmds, t_shell *shell, t_cmd *cmd, int **pipes)
{
	pid_t	*pids;
	t_cmd	*current;
	int		i;

	if (num_cmds <= 0 || !cmd || !shell || !pipes || !*pipes)
		return (NULL);
	pids = init_pids(num_cmds);
	if (!pids)
		return (NULL);
	current = cmd;
	i = -1;
	while (++i < num_cmds && current)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (handle_fork_error(pids, pipes, i), NULL);
		if (pids[i] == 0)
		{
			free(pids);
			child_process(pipes, i, num_cmds);
			exit(create_helper(num_cmds, shell, current, pipes));
		}
		current = current->next;
	}
	return (cleanup_pipes(pipes, num_cmds - 1), pids);
}

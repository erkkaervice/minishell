/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 18:11:34 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 18:11:56 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_pipes(int **pipes, int i, int num_cmds)
{
	if (!pipes || num_cmds < 0 || i < 0)
		return ;
	if (i > 0)
	{
		close(pipes[i - 1][1]);
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			close(pipes[i - 1][0]);
			exit(1);
		}
		close(pipes[i - 1][0]);
	}
	if (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			close(pipes[i][1]);
			exit(1);
		}
		close(pipes[i][1]);
	}
}

static void	close_unused_pipes(int **pipes, int i, int num_cmds)
{
	int	j;

	if (!pipes || num_cmds < 0 || i < 0)
		return ;
	j = 0;
	while (j < num_cmds - 1)
	{
		if (j != i - 1 && j != i)
		{
			close(pipes[j][0]);
			close(pipes[j][1]);
		}
		j++;
	}
}

void	child_process(int **pipes, int i, int num_cmds)
{
	if (!pipes || i < 0 || num_cmds <= 0)
		return ;
	setup_child_pipes(pipes, i, num_cmds);
	close_unused_pipes(pipes, i, num_cmds);
}

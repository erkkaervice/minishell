/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:57:00 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 18:12:56 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_pipes(int **pipes, int num_pipes)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < num_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	cleanup_pipes(int **pipes, int num_pipes)
{
	int	i;

	if (!pipes || num_pipes <= 0)
		return ;
	i = 0;
	while (i < num_pipes)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
		i++;
	}
}

int	**create_pipes(int num_pipes)
{
	int	i;
	int	**pipes;

	if (num_pipes <= 0)
		return (NULL);
	pipes = malloc(sizeof(int *) * num_pipes);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < num_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			cleanup_pipes(pipes, i);
			return (ft_free_pipes(pipes, i), NULL);
		}
		if (pipe(pipes[i]) == -1)
		{
			cleanup_pipes(pipes, i);
			return (ft_free_pipes(pipes, i), NULL);
		}
		i++;
	}
	return (pipes);
}

void	cleanup_processes(pid_t *pids, int count)
{
	int	i;
	int	status;

	if (!pids || count <= 0)
		return ;
	i = 0;
	while (i < count)
	{
		if (pids[i] != -1)
		{
			waitpid(pids[i], &status, 0);
		}
		i++;
	}
}

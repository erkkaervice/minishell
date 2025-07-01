/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:37:25 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 15:44:53 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipe_cmds(t_cmd *cmd)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static void	wait_for_child(pid_t *pids, int num_cmds, t_shell *shell)
{
	int		status;
	int		i;

	if (!pids || !shell || num_cmds < 1)
		return ;
	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		i++;
	}
}

static void	handle_pipes(t_shell *shell, t_cmd *cmd, int num_cmds)
{
	int		**pipes;
	pid_t	*pids;

	if (!shell || !cmd || num_cmds < 1)
		return ;
	if (shell->hd_cnt >= 1 && shell->cmds->pipe == 1)
		return (ft_putendl_fd("Do not support heredoc with pipes", 2), (void)0);
	pipes = create_pipes(num_cmds - 1);
	if (!pipes)
		return ;
	pids = create_process(num_cmds, shell, cmd, pipes);
	if (!pids)
	{
		cleanup_pipes(pipes, num_cmds - 1);
		ft_free_cmds(cmd);
		ft_free_pipes(pipes, num_cmds - 1);
		return ;
	}
	wait_for_child(pids, num_cmds, shell);
	cleanup_pipes(pipes, num_cmds - 1);
	cleanup_processes(pids, num_cmds);
	ft_free_pipes(pipes, num_cmds - 1);
	free(pids);
}

void	execute_pipeline(t_shell *shell, t_cmd *cmd)
{
	int	num_cmds;

	if (!shell || !cmd)
		return ;
	num_cmds = count_pipe_cmds(cmd);
	if (num_cmds <= 0)
		exit_shell(shell, 1);
	else if (num_cmds == 1)
		handle_no_pipe(shell, cmd);
	else if (num_cmds > 1)
		handle_pipes(shell, cmd, num_cmds);
}

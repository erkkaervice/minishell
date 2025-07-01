/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:10:40 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 17:11:33 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_parent_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset"));
}

static void	restore_stdio_and_close(int save_stdin, int save_stdout)
{
	if (save_stdin != -1)
		dup2(save_stdin, STDIN_FILENO);
	if (save_stdout != -1)
		dup2(save_stdout, STDOUT_FILENO);
	if (save_stdin != -1)
		close(save_stdin);
	if (save_stdout != -1 && save_stdout != save_stdin)
		close(save_stdout);
}

static void	handle_parent_builtin(t_shell *shell, t_cmd *cmd)
{
	int	save_stdin;
	int	save_stdout;

	if (!shell || !cmd)
		return ;
	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	if (save_stdin == -1 || save_stdout == -1)
	{
		if (save_stdin != -1)
			close(save_stdin);
		if (save_stdout != -1)
			close(save_stdout);
		ft_putstr_fd("minishell: dup error\n", STDERR_FILENO);
		shell->exit_code = 1;
		return ;
	}
	if (setup_redirections(cmd->redir) == -1)
	{
		shell->exit_code = 1;
		restore_stdio_and_close(save_stdin, save_stdout);
		return ;
	}
	shell->exit_code = execute_builtins(shell, cmd);
	restore_stdio_and_close(save_stdin, save_stdout);
}

void	handle_no_pipe(t_shell *shell, t_cmd *cmd)
{
	int	status;
	int	pid;

	status = 0;
	if (!ft_strcmp(cmd->cmd_table[0], "exit"))
		shell->exit_code = exit_builtin(shell, cmd->cmd_table);
	if (is_parent_builtin(cmd->cmd_table[0]))
		handle_parent_builtin(shell, cmd);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			execute_child_process(cmd, shell, NULL, 0);
			ft_free_cmds(cmd);
			exit_shell(shell, 0);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
	}
}

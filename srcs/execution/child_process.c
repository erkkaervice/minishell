/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:08:15 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 18:11:06 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Tries to execute a command that contains a '/' as a direct path.
 * Validates it first, then runs execve.
 */
static void	try_absolute_path(t_cmd *cmd, t_shell *shell, int **pipes, int i)
{
	int	ret;

	if (!cmd || !shell)
		return ;
	ret = check_exec_path(cmd->cmd_table[0]);
	if (ret != 0)
	{
		cleanup_shell(shell);
		ft_free_pipes(pipes, i);
		exit(ret);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(cmd->cmd_table[0], cmd->cmd_table, shell->envp);
	if (pipes)
		ft_free_pipes(pipes, i);
	ft_free_cmds(cmd);
	exit_shell(shell, ret);
}

static void	exit_n_free_pipes(int **pipes, t_shell *shell, int i, int exit_code)
{
	if (pipes)
		ft_free_pipes(pipes, i);
	exit_shell(shell, exit_code);
}

/*
 * Attempts to resolve the binary by searching PATH and executing it.
 * If not found or invalid, prints error and exits with appropriate code.
 */
static void	try_env_lookup(t_cmd *cmd, t_shell *shell, int **pipes, int i)
{
	char	*cmd_path;
	int		ret;

	if (!cmd || !shell)
		return ;
	cmd_path = get_valid_cmd(cmd->cmd_table[0], shell);
	if (!cmd_path)
	{
		print_err_msg(cmd->cmd_table[0]);
		exit_n_free_pipes(pipes, shell, i, 127);
	}
	ret = check_exec_path(cmd_path);
	if (ret != 0)
	{
		free(cmd_path);
		ft_free_pipes(pipes, i);
		exit_shell(shell, ret);
	}
	signal(SIGQUIT, SIG_DFL);
	execve(cmd_path, cmd->cmd_table, shell->envp);
	if (pipes)
		ft_free_pipes(pipes, i);
	free(cmd_path);
	ft_free_cmds(cmd);
	exit_shell(shell, 1);
}

static void	exec_builtins(t_shell *shell, t_cmd *cmd)
{
	shell->exit_code = execute_builtins(shell, cmd);
	exit_shell(shell, shell->exit_code);
}

/*
//  * Entry point for execution after forking a child.
//  * Runs redirections, builtins, or resolves and executes the binary.
//  * Exits the process on completion or error.
//  */
void	execute_child_process(t_cmd *cmd, t_shell *shell, int **pipes, int i)
{
	if (!cmd || !shell)
		exit_n_free_pipes(pipes, shell, i, 1);
	if (!cmd->cmd_table || !cmd->cmd_table[0])
		exit_n_free_pipes(pipes, shell, i, 1);
	if (setup_redirections(cmd->redir) == -1)
		exit_n_free_pipes(pipes, shell, i, 1);
	if (is_builtin(cmd->cmd_table[0]) && shell->cmds->pipe == 0)
		exec_builtins(shell, cmd);
	if (is_builtin(cmd->cmd_table[0]) && shell->cmds->pipe != 0)
	{
		if (!ft_strcmp(cmd->cmd_table[0], "exit"))
			ft_free_pipes(pipes, i);
		shell->exit_code = execute_builtins(shell, cmd);
		ft_free_pipes(pipes, i);
		exit_shell(shell, shell->exit_code);
	}
	if (ft_strchr(cmd->cmd_table[0], '/'))
		try_absolute_path(cmd, shell, pipes, i);
	else
		try_env_lookup(cmd, shell, pipes, i);
	if (pipes)
		ft_free_pipes(pipes, i);
	ft_free_cmds(cmd);
	perror("minishell");
	exit_shell(shell, 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:53:10 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/11 14:24:40 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	else if (!ft_strcmp(cmd, "echo"))
		return (1);
	else if (!ft_strcmp(cmd, "env"))
		return (1);
	else if (!ft_strcmp(cmd, "exit"))
		return (1);
	else if (!ft_strcmp(cmd, "export"))
		return (1);
	else if (!ft_strcmp(cmd, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd, "unset"))
		return (1);
	return (0);
}

int	execute_builtins(t_shell *shell, t_cmd *cmd)
{
	if (!shell || !cmd || !cmd->cmd_table || !cmd->cmd_table[0])
		return (1);
	if (!ft_strcmp(cmd->cmd_table[0], "cd"))
		return (cd_builtin(cmd->cmd_table, shell));
	else if (!ft_strcmp(cmd->cmd_table[0], "echo"))
		return (echo_builtin(cmd->cmd_table));
	else if (!ft_strcmp(cmd->cmd_table[0], "env"))
		return (env_builtin(shell, cmd->cmd_table));
	else if (!ft_strcmp(cmd->cmd_table[0], "exit"))
		return (exit_builtin(shell, cmd->cmd_table));
	else if (!ft_strcmp(cmd->cmd_table[0], "export"))
		return (export_builtin(shell, cmd->cmd_table));
	else if (!ft_strcmp(cmd->cmd_table[0], "pwd"))
		return (pwd_builtin());
	else if (!ft_strcmp(cmd->cmd_table[0], "unset"))
		return (unset_builtin(shell, cmd->cmd_table));
	return (1);
}

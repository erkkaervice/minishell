/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 17:58:44 by eala-lah          #+#    #+#             */
/*   Updated: 2025/06/28 13:32:58 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Verifies if the given path is a valid executable file:
 * - Returns 127 if stat fails,
 * - Returns 126 if it's a directory or not executable,
 * - Returns 0 if it's valid.
 */
int	check_exec_path(char *path)
{
	struct stat	st;

	if (!path)
		return (127);
	if (stat(path, &st) == -1)
		return (print_path_error(path, strerror(errno), 127));
	if (S_ISDIR(st.st_mode))
		return (print_path_error(path, "Is a directory", 126));
	if (access(path, X_OK) != 0)
		return (print_path_error(path, "Permission denied", 126));
	return (0);
}

/*
 * Helper that prints a standardized error message for exec path failures,
 * including the path, message, and error code to return.
 */
int	print_path_error(char *path, char *msg, int code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (code);
}

void	exit_with_msg(char *msg, t_shell *shell, int exit_code)
{
	write(2, msg, ft_strlen(msg));
	exit_shell(shell, exit_code);
}

void	check_failure(char **check, char *cmd, t_shell *shell)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	ft_free_arr_2d(check);
	exit_shell(shell, 127);
}

void	print_err_msg(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
}

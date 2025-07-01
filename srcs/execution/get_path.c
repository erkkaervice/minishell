/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:41:36 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 12:20:10 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_cmd_arr(t_shell *shell)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(shell->envp[i] + 5);
			if (!path)
				ft_free_arr_2d(shell->envp);
			break ;
		}
		i++;
	}
	if (!path)
		exit_with_msg("Error: PATH not found\n", shell, 127);
	shell->path_dirs = ft_split(path, ':');
	free(path);
	if (!shell->path_dirs)
		exit_with_msg("Error: Memory allocation failed\n", shell, 127);
}

static char	*get_temp_cmd(char *path)
{
	char	*temp_cmd;

	if (!path)
		return (NULL);
	temp_cmd = ft_strjoin(path, "/");
	if (!temp_cmd)
	{
		write(2, "Error: Memory allocation failed\n", 32);
		exit(1);
	}
	return (temp_cmd);
}

static char	*build_valid_cmd(char *path, char *cmd_part)
{
	char	*temp;
	char	*valid_cmd;

	if (!path || !cmd_part)
		return (NULL);
	temp = get_temp_cmd(path);
	if (!temp)
		return (NULL);
	valid_cmd = ft_strjoin(temp, cmd_part);
	free(temp);
	if (!valid_cmd)
	{
		write(2, "Error: Memory allocation failed\n", 32);
		exit(1);
	}
	if (access(valid_cmd, X_OK) == 0)
		return (valid_cmd);
	free(valid_cmd);
	return (NULL);
}

static void	free_resources(char **cmd_arr, char **check)
{
	ft_free_arr_2d(cmd_arr);
	ft_free_arr_2d(check);
}

char	*get_valid_cmd(char *cmd, t_shell *shell)
{
	int		i;
	char	**check;
	char	*valid_cmd;

	if (!cmd || !shell)
		return (NULL);
	check = ft_split(cmd, ' ');
	if (!check || !check[0])
		check_failure(check, cmd, shell);
	i = 0;
	get_cmd_arr(shell);
	while (shell->path_dirs[i])
	{
		valid_cmd = build_valid_cmd(shell->path_dirs[i], check[0]);
		if (valid_cmd)
			return (free_resources(shell->path_dirs, check), valid_cmd);
		i++;
	}
	ft_free_arr_2d(check);
	return (NULL);
}

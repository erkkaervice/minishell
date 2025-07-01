/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:20:19 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/25 12:11:30 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_free_pwds(char *pwd, char *old_pwd)
{
	free(old_pwd);
	free(pwd);
	return ;
}

static int	print_error_msg(char *msg, int status)
{
	ft_putstr_fd(msg, 2);
	return (status);
}

static	void	ft_strdup_process(char **envp, int i, char *str)
{
	if (!envp || !*envp || !str || i < 0)
		return ;
	free(envp[i]);
	envp[i] = ft_strdup(str);
	if (!envp[i])
		free(str);
}

static void	update_pwd_env(t_shell *shell)
{
	char	*pwd;
	char	*old_pwd;
	char	cwd[1024];
	int		i;

	if (!shell)
		return ;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("minishell: cd"), (void)0);
	old_pwd = ft_strjoin("OLDPWD=", getenv("PWD"));
	pwd = ft_strjoin("PWD=", cwd);
	if (!old_pwd || !pwd)
		ft_free_pwds(pwd, old_pwd);
	i = -1;
	while (shell->envp[++i])
	{
		if (ft_strncmp(shell->envp[i], "PWD=", 4) == 0)
			ft_strdup_process(shell->envp, i, pwd);
		else if (ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0)
			ft_strdup_process(shell->envp, i, old_pwd);
		if ((ft_strncmp(shell->envp[i], "OLDPWD=", 7) == 0)
			&& (ft_strncmp(shell->envp[i], "PWD=", 4) == 0))
			ft_free_arr_2d(shell->envp);
	}
	ft_free_pwds(pwd, old_pwd);
}

int	cd_builtin(char **args, t_shell *shell)
{
	char	*path;

	if (!args || !shell)
		return (1);
	if (args[1] == NULL)
		return (print_error_msg("minishell: cd : missing arguments\n", 1));
	if (args[2] != NULL)
		return (print_error_msg("minishell: cd: too many arguments\n", 1));
	else
		path = args[1];
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	update_pwd_env(shell);
	return (0);
}

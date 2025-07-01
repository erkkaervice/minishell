/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:21:06 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/25 11:54:12 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_existing_var(char **envp, char *var)
{
	int		i;
	char	*name;
	char	*equal_sign;

	if (!envp || !var)
		return ;
	i = 0;
	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return ;
	name = ft_substr(var, 0, equal_sign - var);
	if (!name)
		return (free(var), (void)0);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, ft_strlen(name)) == 0
			&& envp[i][ft_strlen(name)] == '=')
		{
			free(envp[i]);
			envp[i] = ft_strdup(var);
			break ;
		}
		i++;
	}
	free(name);
}

static char	*get_temp_var(char *new_var)
{
	char	*temp;
	char	*substr;

	if (!new_var)
		return (NULL);
	if (!ft_strchr(new_var, '='))
		temp = ft_strjoin(new_var, "=");
	else if (new_var[ft_strlen(new_var) - 2] == '='
		&& new_var[ft_strlen(new_var) - 1] == '='
		&& new_var[ft_strlen(new_var)] == '\0')
	{
		substr = ft_substr(new_var, 0, ft_strlen(new_var) - 1);
		if (!substr)
			return (free(new_var), NULL);
		temp = ft_strjoin(substr, "'='");
		if (!temp)
			return (free(substr), NULL);
	}
	else if (new_var[ft_strlen(new_var) - 1] == '='
		&& new_var[ft_strlen(new_var)] == '\0')
		temp = ft_strjoin(new_var, "");
	else
		temp = ft_strdup(new_var);
	return (temp);
}

static char	**update_new_env(char **envp, char *new_var)
{
	int		i;
	char	**new_envp;
	char	*temp;

	if (!envp || !*envp || !new_var)
		return (NULL);
	i = 0;
	while (envp[i])
		i++;
	new_envp = ft_copy_2d(envp);
	if (!new_envp)
		return (NULL);
	temp = get_temp_var(new_var);
	if (!temp)
		return (ft_free_resources(new_envp, new_var));
	new_envp[i] = ft_strdup(temp);
	if (!new_envp[i])
		return (ft_free_resources(new_envp, temp));
	new_envp[i + 1] = NULL;
	free(temp);
	return (new_envp);
}

static int	handle_export(t_shell *shell, char *var)
{
	char	**new_envp;
	char	**old_envp;

	if (!shell || !var)
		return (1);
	if (!is_valid_identifier(var))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(var, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	update_existing_var(shell->envp, var);
	if (!is_existing_var(shell->envp, var))
	{
		old_envp = shell->envp;
		new_envp = update_new_env(shell->envp, var);
		if (!new_envp)
			return (1);
		shell->envp = new_envp;
		ft_free_arr_2d(old_envp);
	}
	return (0);
}

int	export_builtin(t_shell *shell, char **args)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
	{
		print_exported_vars(shell->envp);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (handle_export(shell, args[i]))
			status = 1;
		i++;
	}
	return (status);
}

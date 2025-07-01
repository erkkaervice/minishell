/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:01:18 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/25 12:12:57 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || ft_isdigit(str[0]) || str[0] == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

static	int	free_w_status(char *str, int status)
{
	free(str);
	return (status);
}

int	is_existing_var(char **envp, char *var)
{
	int		i;
	char	*name;
	char	*equal_sign;

	if (!envp || !var)
		return (0);
	i = 0;
	equal_sign = ft_strchr(var, '=');
	if (!equal_sign)
		return (0);
	name = ft_substr(var, 0, equal_sign - var);
	if (!name)
		return (free_w_status(var, 0));
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, ft_strlen(name)) == 0
			&& envp[i][ft_strlen(name)] == '=')
			return (free_w_status(name, 1));
		i++;
	}
	free(name);
	return (0);
}

void	*ft_free_resources(char **envp, char *str)
{
	free(str);
	ft_free_arr_2d(envp);
	return (NULL);
}

void	print_exported_vars(char **envp)
{
	int		i;
	char	*name;
	char	*value;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		name = ft_substr(envp[i], 0, ft_strchr(envp[i], '=') - envp[i]);
		value = ft_strchr(envp[i], '=') + 1;
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(name, 1);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(value, 1);
		ft_putstr_fd("\"\n", 1);
		free(name);
		i++;
	}
}

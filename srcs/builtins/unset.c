/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:21:27 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/07/01 16:23:53 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_envp_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

static int	copy_vars(char **envp, char **new_envp, char *var_with_eq)
{
	int		i;
	int		j;
	size_t	len;

	len = ft_strlen(var_with_eq);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_with_eq, len) != 0)
		{
			new_envp[j] = ft_strdup(envp[i]);
			if (!new_envp[j])
			{
				free(envp[i]);
				return (-1);
			}
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	return (0);
}

static char	**remove_var(char **envp, char *var)
{
	int		i;
	char	**new_envp;
	char	*var_with_eq;

	var_with_eq = ft_strjoin(var, "=");
	if (!var_with_eq || !envp || !*envp)
		return (NULL);
	i = count_envp_vars(envp);
	new_envp = (char **)malloc((i + 1) * sizeof(char *));
	if (!new_envp)
		return (free(var_with_eq), NULL);
	if (copy_vars(envp, new_envp, var_with_eq) == -1)
	{
		ft_free_resources(new_envp, var_with_eq);
		return (NULL);
	}
	free(var_with_eq);
	return (new_envp);
}

int	unset_builtin(t_shell *shell, char **args)
{
	char	**new_envp;
	char	**old_envp;
	int		i;

	if (!shell || !args)
		return (1);
	i = 1;
	while (args[i])
	{
		old_envp = shell->envp;
		new_envp = remove_var(shell->envp, args[i]);
		if (!new_envp)
		{
			ft_free_arr_2d(old_envp);
			return (1);
		}
		shell->envp = new_envp;
		ft_free_arr_2d(old_envp);
		i++;
	}
	return (0);
}

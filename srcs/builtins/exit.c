/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:22:05 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/07/01 16:23:54 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric_overflow(char *str)
{
	int			i;
	int			sign;
	long long	res;
	long long	prev;

	i = 0;
	sign = 1;
	res = 0;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		prev = res;
		res = res * 10 + (str[i++] - '0');
		if ((res < prev) || (res * sign > LLONG_MAX)
			|| (res * sign < LLONG_MIN))
			return (0);
	}
	return (1);
}

static long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	exit_w_error(t_shell *shell, char **args, int exit_code)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(args[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	if (shell->cmds)
	{
		ft_free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	ft_free_arr_2d(shell->envp);
	return (exit_code);
}

int	exit_builtin(t_shell *shell, char **args)
{
	long long	exit_code;

	if (!shell || !args)
		exit_code = 1;
	else if (!args[1])
		exit_code = shell->exit_code;
	else if (args[2] && is_numeric_overflow(args[1]))
		return (ft_putendl_fd("minishell: exit: too many arguments", 2), 1);
	else if (!is_numeric_overflow(args[1]))
		exit(exit_w_error(shell, args, 2));
	else
		exit_code = ft_atoll(args[1]) & 255;
	if (!shell->cmds)
		ft_putendl_fd("exit", 1);
	if (shell->cmds)
	{
		ft_free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	exit_shell(shell, exit_code);
	return (exit_code);
}

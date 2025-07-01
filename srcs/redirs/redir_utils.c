/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:23:12 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 14:48:34 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg(char *filename, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(msg, 2);
}

int	handle_redir_error(t_redir *redir, int error)
{
	if (error == EACCES)
		error_msg(redir->filename, ": Permission denied\n");
	else if (error == ENOENT || error == ENOTDIR || error == EISDIR)
		error_msg(redir->filename, ": No such file or directory\n");
	else
		error_msg(redir->filename, ": Redirection error\n");
	return (1);
}

static int	validate_single_redir(t_redir *redir)
{
	int	fd;
	int	result;

	fd = -1;
	result = 0;
	if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIR_HEREDOC)
		result = handle_heredoc(redir);
	if ((fd == -1 && redir->type != REDIR_HEREDOC)
		|| (redir->type == REDIR_HEREDOC && result != 0))
		return (handle_redir_error(redir, errno));
	if (fd != -1)
		close(fd);
	return (0);
}

int	validate_redirections(t_redir *redir)
{
	while (redir)
	{
		if (validate_single_redir(redir) != 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}

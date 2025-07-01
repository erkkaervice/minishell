/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-mngu <thi-mngu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:30:58 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/06/28 13:26:17 by thi-mngu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	find_last_redirs(t_redir *redir, t_redir **last_in,
	t_redir **last_out)
{
	*last_in = NULL;
	*last_out = NULL;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			*last_in = redir;
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
			*last_out = redir;
		redir = redir->next;
	}
}

static int	apply_input_redir(t_redir *last_in)
{
	int	fd;

	fd = open(last_in->filename, O_RDONLY);
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		if (fd != -1)
			close(fd);
		return (handle_redir_error(last_in, errno));
	}
	close(fd);
	return (0);
}

static int	apply_output_redir(t_redir *ls_out)
{
	int	fd;

	if (ls_out->type == REDIR_OUT)
		fd = open(ls_out->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(ls_out->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
	{
		if (fd != -1)
			close(fd);
		return (handle_redir_error(ls_out, errno));
	}
	close(fd);
	return (0);
}

static int	apply_redirections(t_redir *redir)
{
	t_redir	*last_in;
	t_redir	*last_out;

	last_in = NULL;
	last_out = NULL;
	find_last_redirs(redir, &last_in, &last_out);
	if (last_in && apply_input_redir(last_in) != 0)
		return (-1);
	if (last_out && apply_output_redir(last_out) != 0)
		return (-1);
	return (0);
}

int	setup_redirections(t_redir *redir)
{
	if (validate_redirections(redir) != 0)
		return (-1);
	if (apply_redirections(redir) != 0)
		return (-1);
	return (0);
}

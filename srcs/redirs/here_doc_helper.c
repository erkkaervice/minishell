/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 18:13:31 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/07/01 20:17:35 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hd_abort(int hd_check)
{
	static int	hd_stop;

	if (hd_check == 1)
		hd_stop = 1;
	if (hd_check == 2)
		hd_stop = 0;
	return (hd_stop);
}

static int	sigint_handle(char *line, int fd, char *temp_file)
{
	free(line);
	close(fd);
	unlink(temp_file);
	hd_abort(2);
	return (-2);
}

int	open_heredoc_file(char *temp_file)
{
	int	fd;

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	write_heredoc_lines(int fd, char *delimiter, char *temp_file)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (hd_abort(0) == 1)
			return (sigint_handle(line, fd, temp_file));
		if (!line)
		{
			ft_putstr_fd(
				"minishell: warning: here-document delimited by end-of-file: ",
				2);
			ft_putendl_fd(delimiter, 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

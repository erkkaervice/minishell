/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:26:10 by thi-mngu          #+#    #+#             */
/*   Updated: 2025/07/01 20:17:57 by eala-lah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h> // for getpid()

static int	event_hook(void)
{
	return (0);
}

static void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
	hd_abort(1);
}

static void	set_hd_signal(void)
{
	struct sigaction	sa_hd;

	sigemptyset(&sa_hd.sa_mask);
	sa_hd.sa_handler = handle_sigint_heredoc;
	sa_hd.sa_flags = 0;
	sigaction(SIGINT, &sa_hd, NULL);
	rl_event_hook = event_hook;
}

static void	get_temp_filename(char *buf, size_t size)
{
	static int	heredoc_index = 0;
	pid_t		pid;

	pid = getpid();
	ft_strlcpy(buf, "/tmp/heredoc_", size);
	ft_strlcat(buf, ft_itoa(pid), size);
	ft_strlcat(buf, "_", size);
	ft_strlcat(buf, ft_itoa(heredoc_index), size);
	heredoc_index++;
}

static int	create_heredoc(char *delimiter, char *temp_file)
{
	int		fd;

	if (!delimiter)
		return (-1);
	fd = open_heredoc_file(temp_file);
	if (fd == -1)
		return (-1);
	hd_abort(2);
	set_hd_signal();
	write_heredoc_lines(fd, delimiter, temp_file);
	close(fd);
	hd_abort(2);
	return (open(temp_file, O_RDONLY));
}

int	handle_heredoc(t_redir *redir)
{
	char	temp_file[64];
	int		fd;

	get_temp_filename(temp_file, sizeof(temp_file));
	fd = create_heredoc(redir->filename, temp_file);
	if (fd == -2)
		return (130);
	if (fd == -1)
		return (1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		unlink(temp_file);
		return (1);
	}
	close(fd);
	unlink(temp_file);
	return (0);
}

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <signal.h>
# include <limits.h>
# include "libft.h"

extern int	g_sigstatus;

/* Data Structures */
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR,
	T_ENV_VAR,
	T_EXIT_STATUS
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			qtype;
	struct s_token	*next;
}	t_token;

typedef struct s_token_list
{
	t_token	**first;
	t_token	**current;
}	t_token_list;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**cmd_table;
	t_redir			*redir;
	int				pipe;
	int				is_builtin;
	char			*cmd_name;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_cmd	*cmds;
	char	**envp;
	char	**path_dirs;
	int		exit_code;
	int		hd_cnt;
}	t_shell;

typedef struct s_tokdata
{
	char		*input;
	int			*i;
	t_shell		*shell;
}	t_tokdata;

/* Signal Handling */
void	signal_interactive(void);
void	signal_noninteractive(void);
void	update_shell_exit_code_from_signal(t_shell *shell);
void	signal_sigint(int sig);
void	signal_sigquit(int sig);

/* Shell Initialization and Cleanup */
void	initialize_shell(t_shell *shell, char **envp);
void	exit_shell(t_shell *shell, int exit_code);
void	cleanup_shell(t_shell *shell);

/* Prompt & Input Handling */
void	write_prompt_to_terminal(void);
char	*read_and_prepare_input(void);
void	process_input_line(char *line, t_shell *shell);

/* Input Validation & Syntax Checks */
int		is_lone_redirection(char *line);
int		is_invalid_command(char *line);
int		check_input_errors(char *line, t_shell *shell);
int		line_is_syntax_error(char *line);
void	print_syntax_error(char *token);
void	ft_print_syntax_error_token(t_token *token);

/* Tokenization & Lexing */
t_token	*ft_tokenize(char *input, t_shell *shell);
t_token	*ft_handle_word(char *input, int *i, char qtype, t_shell *shell);
t_token	*ft_handle_quote(char *input, int *i, t_shell *shell);
t_token	*ft_handle_operator(char *input, int *i, t_shell *shell);
t_token	*ft_handle_pipe(int *i, t_shell *shell);
t_token	*ft_handle_dollar(char *input, int *i, char qtype, t_shell *shell);
t_token	*ft_handle_dollar_in_quotes(char *input, int *i, t_shell *shell);
t_token	*ft_check_token_creation(char *value, t_token_type type, char qtype);
t_token	*ft_merge_tokens(t_token *a, t_token *b);
t_token	*handle_alloc_failure(t_shell *shell);

/* Parsing & Command Construction */
t_cmd	*ft_parse_input(char *input);
t_cmd	*ft_parse(t_token *tokens, t_shell *shell);
int		ft_cmd_redirs_and_pipe(t_cmd *cmd, t_token **token);
int		ft_collect_redirs(t_cmd *cmd, t_token **token);
char	**ft_build_av(t_token *t, int n);
int		ft_add_args(char **av, int n, t_token **t);

/* Expansion */
void	expand_env_and_status(t_token *token, t_shell *shell);
char	*expand_word(const char *in, t_shell *shell);
char	*get_env_val(t_shell *shell, const char *name);

/* Built-in Commands */
int		is_builtin(char *cmd);
int		execute_builtins(t_shell *shell, t_cmd *cmd);
int		echo_builtin(char **args);
int		cd_builtin(char **args, t_shell *shell);
int		env_builtin(t_shell *shell, char **args);
int		pwd_builtin(void);
int		exit_builtin(t_shell *shell, char **args);
int		export_builtin(t_shell *shell, char **args);
int		unset_builtin(t_shell *shell, char **args);
int		is_parent_builtin(char *cmd);

/* Builtin Helpers */
int		is_valid_identifier(char *str);
int		is_existing_var(char **envp, char *var);
void	print_exported_vars(char **envp);
void	*ft_free_resources(char **envp, char *str);

/* Redirection Setup & Utilities */
int		handle_heredoc(t_redir *redir);
int		setup_redirections(t_redir *redir);
int		handle_redir_error(t_redir *redir, int error);
int		validate_redirections(t_redir *redir);
int		hd_abort(int hd_check);
int		open_heredoc_file(char *temp_file);
int		write_heredoc_lines(int fd, char *delimiter, char *temp_file);
void	error_msg(char *filename, char *msg);

/* Pipe Management */
int		count_pipe_cmds(t_cmd *cmd);
int		**create_pipes(int num_pipes);
pid_t	*create_process(int num_cmds, t_shell *shell, t_cmd *cmd, int **pipes);
void	cleanup_pipes(int **pipes, int num_pipes);
void	cleanup_processes(pid_t *pids, int count);

/* Execution */
void	handle_no_pipe(t_shell *shell, t_cmd *cmd);
void	execute_pipeline(t_shell *shell, t_cmd *cmd);
void	child_process(int **pipes, int i, int num_cmds);
void	execute_child_process(t_cmd *cmd, t_shell *shell, int **pipes, int i);
char	*get_valid_cmd(char *cmd, t_shell *shell);

/* Memory Management */
void	ft_free_av(char **av, int filled);
void	ft_free_tokens(t_token *token);
void	ft_free_redirs(t_redir *redir);
void	ft_free_cmds(t_cmd *cmd);
void	ft_free_pipes(int **pipes, int num_pipes);

/* Misc Helpers */
char	*trim_whitespace(char *line);
int		check_exec_path(char *path);
int		print_path_error(char *path, char *msg, int code);
void	print_err_msg(char *cmd);
void	exit_with_msg(char *msg, t_shell *shell, int exit_code);
void	check_failure(char **check, char *cmd, t_shell *shell);

#endif

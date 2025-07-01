# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eala-lah <eala-lah@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/19 15:00:11 by eala-lah          #+#    #+#              #
#    Updated: 2025/07/01 18:01:22 by eala-lah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

INCS		= -I ./includes/ -I ./library/libft

LIBFT_DIR	= ./library/libft
LIBFT		= $(LIBFT_DIR)/libft.a

CC		= cc
CFLAGS		= -Wall -Wextra -Werror $(INCS)
LDFLAGS		= -lreadline

SRC_DIR		= srcs/
MAIN_DIR	= $(SRC_DIR)main
SIGNALS_DIR	= $(SRC_DIR)signals
SHELL_DIR	= $(SRC_DIR)shell
PARSING_DIR	= $(SRC_DIR)parsing
EXPANSION_DIR	= $(SRC_DIR)expansion
REDIRS_DIR	= $(SRC_DIR)redirs
BUILTINS_DIR	= $(SRC_DIR)builtins
EXEC_DIR	= $(SRC_DIR)execution

MAIN_SRC	= main.c
SIGNALS_SRC	= handlers.c setup.c
SHELL_SRC	= input.c control.c
PARSING_SRC	= tokenising.c handling.c handling2.c freedom.c parsing.c redirs.c syntax.c syntax2.c
EXPANSION_SRC	= expansion.c expansion_tokens.c
REDIRS_SRC	= redirection.c here_doc.c redir_utils.c here_doc_helper.c
BUILTINS_SRC	= cd.c echo.c env.c exit.c export.c pwd.c unset.c execute_builtins.c builtin_utils.c
EXEC_SRC	= setup_child.c child_process.c get_path.c pipeline.c pipeline_creation.c exec_utils.c pipeline_helper.c no_pipe.c

SRC			= $(addprefix $(MAIN_DIR)/, $(MAIN_SRC)) \
			  $(addprefix $(SIGNALS_DIR)/, $(SIGNALS_SRC)) \
			  $(addprefix $(SHELL_DIR)/, $(SHELL_SRC)) \
			  $(addprefix $(PARSING_DIR)/, $(PARSING_SRC)) \
			  $(addprefix $(EXPANSION_DIR)/, $(EXPANSION_SRC)) \
			  $(addprefix $(REDIRS_DIR)/, $(REDIRS_SRC)) \
			  $(addprefix $(BUILTINS_DIR)/, $(BUILTINS_SRC)) \
			  $(addprefix $(EXEC_DIR)/, $(EXEC_SRC))

OBJ_DIR		= obj/
OBJS		= $(SRC:srcs/%.c=$(OBJ_DIR)%.o)

all: $(OBJ_DIR) $(LIBFT) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# $(OBJ_DIR)%.o: srcs/%.c
# 	@mkdir -p $(dir $@)
# 	@$(CC) $(CFLAGS) -c $< -o $@ 2> /dev/null || { echo "Failed to compile $<." >&2; exit 1; }

# $(NAME): $(OBJS) $(LIBFT)
# 	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(LDFLAGS) 2> /dev/null || { echo "Failed to create executable $(NAME)." >&2; exit 1; }

# $(LIBFT):
# 	@$(MAKE) -C $(LIBFT_DIR) > /dev/null 2>&1 || { echo "Failed to build libft." >&2; exit 1; }

$(OBJ_DIR)%.o: srcs/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ || { echo "Failed to compile $<." >&2; exit 1; }

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(LDFLAGS) || { echo "Failed to create executable $(NAME)." >&2; exit 1; }

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) || { echo "Failed to build libft." >&2; exit 1; }

clean:
	@rm -rf $(OBJ_DIR) 2> /dev/null || { echo "Failed to clean object files." >&2; exit 1; }
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1 || { echo "Failed to clean libft." >&2; exit 1; }

fclean: clean
	@rm -f $(NAME) 2> /dev/null || { echo "Failed to remove executable." >&2; exit 1; }
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1 || { echo "Failed to fclean libft." >&2; exit 1; }

re: fclean all

.PHONY: all clean fclean re

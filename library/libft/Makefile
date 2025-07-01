NAME := libft.a

CC := cc
CFLAGS := -Wall -Wextra -Werror

SOURCES :=	count_words.c \
			ft_putnbr_fd.c    \
            ft_putendl_fd.c    \
            ft_putstr_fd.c    \
            ft_putchar_fd.c    \
            ft_striteri.c    \
            ft_strmapi.c    \
            ft_itoa.c    \
            ft_split.c    \
            ft_strtrim.c    \
            ft_strjoin.c    \
            ft_substr.c    \
            ft_strdup.c    \
            ft_calloc.c    \
            ft_atoi.c    \
            ft_bzero.c    \
            ft_isalnum.c    \
            ft_isalpha.c    \
            ft_isascii.c    \
            ft_isdigit.c    \
            ft_isprint.c    \
            ft_isquote.c    \
            ft_isspace.c    \
            ft_isoperator.c \
            ft_memchr.c    \
            ft_memcmp.c    \
            ft_memcpy.c    \
            ft_memmove.c    \
            ft_memset.c    \
            ft_strchr.c    \
            ft_strlcat.c    \
			ft_strcpy.c     \
            ft_strlcpy.c    \
            ft_strlen.c    \
            ft_strcmp.c    \
            ft_strncmp.c    \
            ft_strnstr.c    \
            ft_strrchr.c    \
			ft_strspn.c 	\
            ft_tolower.c    \
            ft_toupper.c    \
            ft_free_arr_2d.c \
            ft_free_arr_3d.c  \
			ft_free_process_2d.c \
			ft_free_process_3d.c \
            ft_copy_2d.c \

BONUS    :=    		ft_lstnew_bonus.c    \
            		ft_lstadd_front_bonus.c    \
            		ft_lstsize_bonus.c    \
            		ft_lstlast_bonus.c    \
			ft_lstadd_back_bonus.c    \
			ft_lstdelone_bonus.c    \
			ft_lstclear_bonus.c    \
			ft_lstiter_bonus.c

OBJECTS    :=    $(SOURCES:.c=.o)

OBJECTS_BONUS    :=    $(BONUS:.c=.o)

all: $(NAME)

bonus:    .bonus

.bonus:    $(NAME)		$(OBJECTS_BONUS)
	ar	rsc	$(NAME)	$(OBJECTS_BONUS)
	@touch    .bonus

$(NAME):	$(OBJECTS)	$(OBJECTS_BONUS)
	ar	rcs	$(NAME)	$(OBJECTS)	$(OBJECTS_BONUS)

%.o:	%.c
	$(CC)	$(CFLAGS)	-c	$<	-o	$@

clean:
	rm	-f	$(OBJECTS)	$(OBJECTS_BONUS)	.bonus

fclean:	clean
	rm	-f	$(NAME)

re:    fclean    all

.PHONY:    all    clean    fclean    re

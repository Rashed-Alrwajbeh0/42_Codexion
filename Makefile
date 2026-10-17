NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

C_FILES = init_coders.c \
			priority_types.c \
			arguments.c \
			init_common.c \
			make_threads.c \
			start_threads.c \
			coder_work.c \
			free.c \
			init_dongles.c \
			monitor.c \
			priority_queue.c \
			main.c \
			helper_init_functions.c \
			queu_functions.c \
			free_2.c \
			help_with_coder_work.c

all : $(NAME)
$(NAME) : $(C_FILES)
	$(CC) $(CFLAGS) $(C_FILES) -o $(NAME)
clean :
	rm -f  $(NAME)
fclean : clean
re : fclean all
.PHONY : clean fclean all re
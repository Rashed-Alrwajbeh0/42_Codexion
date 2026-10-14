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
			helper_init_functions.c

all : $(NAME)
$(NAME) : codexion.h structs.h
	$(CC) $(CFLAGS) $(C_FILES) -o $(NAME)
clean :
	rm -f *.o
fclean : clean
	rm -f  $(NAME)
re : fclean all
.PHONY : clean fclean all re
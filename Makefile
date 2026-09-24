flages = -Wall -Wextra -Werror -pthread

all:
	@cc $(flages) test.c
	@./a.out

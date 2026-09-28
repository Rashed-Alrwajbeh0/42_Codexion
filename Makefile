flages = -Wall -Wextra -Werror -pthread

all:
	@cc $(flages) test.c
	@./a.out
clean:
<<<<<<< HEAD
	rm -rf vgcore.*
=======
	@rm -rf vgcore.*
>>>>>>> 07b1d98 (finish)

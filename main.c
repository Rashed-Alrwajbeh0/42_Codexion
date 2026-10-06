
#include "codexion.h"

int	main(int argc, char *argv[])
{
	t_arguments *args;
	t_dongle	*dongles;
	t_queue		*my_queue;

	args = check_arg(argv, argc);
	if (!args)
		return (0);
	dongles = init_dongles(*args);
	if (!dongles)
		return (free(args), 0);
	my_queue = init_queue(*args);
	if (!my_queue)
		return (free(args), free(dongles), 0);
	if (!make_threads(*args, dongles, my_queue))
		return (free(args), free(dongles), free(my_queue), 0);
}

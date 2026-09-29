#include "codexion.h"


int	edf(t_thread_vars t1, t_thread_vars t2)
{
	if (calctime(t1.last_compilation_time) + t1.arguments.time_to_burnout
		<= calctime(t2.last_compilation_time) + t2.arguments.time_to_burnout)
		return (1);
	return (0);
}

int	calctime(struct timeval t)
{
	return ((1000 * t.tv_sec) + (t.tv_usec / 1000));
}

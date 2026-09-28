#include "codexion.h"


int	edf(t_thread_vars t1, t_thread_vars t2)
{
	if (t1.last_compilation_time.tv_usec + t1.arguments.time_to_burnout
		< t2.last_compilation_time.tv_usec + t2.arguments.time_to_burnout)
		return (1);
	return (0);
}

int	fifo(t_priority_queue *q, t_thread_vars t)
{
	q->size++;
	q->elemets[q->size - 1] = t;
}

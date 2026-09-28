#include "codexion.h"

void	add_to_queue(t_priority_queue *q, t_thread_vars t, char *priority_type)
{
	q->elemets[q->size - 1] = t;
	q->size++;
	sort_to_top(q, priority_type);
}

t_thread_vars	*top_priority(t_priority_queue *q)
{
	t_thread_vars	*answer;

	if (!q->size)
		return (NULL);
	answer = &(q->elemets[0]);

}

static void	sort_to_top(t_priority_queue *q, char *priority_type)
{
	int				left;
	int				right;
	int 			parent;
	int				current;
	t_thread_vars	temp;

	current = q->size;
	left = 2 * current + 2;
	right = left - 1;
	parent = (current - 1) / 2; 
	while (current)
	{
		if (!strcmp(priority_type, "edf"))
		{
			if (edf(q -> elemets[current], q -> elemets[parent]))
			{
				temp = q->elemets[parent];
				q->elemets[parent] = q->elemets[current];
				q->elemets[current] = temp;
				current = parent;
				parent = (current - 1) / 2;
			}
			else
				break ;
		}
		else
		{
			fifo(&q, q->elemets[current]);
			break ;
		}
	}
}
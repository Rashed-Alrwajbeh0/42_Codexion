#include "codexion.h"


char	*determaine_child(t_priority_queue *q, int q_size,
							int left, int right)
{
	if (right < q_size && left < q_size
		&& edf(*(q->elemets[right]), *(q->elemets[left])))
		return ("right");
	else if (left < q_size && right < q_size
		&& edf(*(q->elemets[left]), *(q->elemets[right])))
		return ("left");
	else if (left < q_size && !(right < q_size))
		return ("left");
	else if (right < q_size && !(left < q_size))
		return ("right");
	return (NULL);
}


void	swap(t_priority_queue *q, int *current, int another_idx)
{
	t_thread_vars	*temp;

	temp = q->elemets[*current];
	q->elemets[*current] = q->elemets[another_idx];
	q->elemets[another_idx] = temp;
	*current = another_idx;
}

void sort_to_down(t_priority_queue *q, int q_size)
{
	int				current;
	int				left;
	int				right;
	char			*child;

	current = 0;
	left = 2 * current + 1;
	right = 2 * current + 2;
	child = determaine_child(q, q_size, left, right);
	while (child)
	{
		if (!strcmp(child, "right"))
		{
			swap(q, &current, right);
			left = 2 * current + 1;
			right = 2 * current + 2;
			child = determaine_child(q, q_size, left, right);
		}
		else if (!strcmp(child, "left"))
		{
			swap(q, &current, left);
			left = 2 * current + 1;
			right = 2 * current + 2;
			child = determaine_child(q, q_size, left, right, current);
		}
		else
			break ;
	}
}

void	sort_to_top(t_priority_queue *q)
{
	int				parent;
	int				current;
	t_thread_vars	*temp;

	current = q->size - 1;
	if (!current)
		return ;
	parent = (current - 1) / 2;
	while (1)
	{
		if (edf(*(q->elemets[current]), *(q->elemets[parent])))
		{
			temp = q->elemets[parent];
			q->elemets[parent] = q->elemets[current];
			q->elemets[current] = temp;
			current = parent;
			parent = (current - 1) / 2;
		}
		else
			break ;
		if (!current)
			break ;
	}
}

void	add_to_queue(t_priority_queue *q, t_thread_vars *t)
{
	q->size++;
	q->elemets[q->size - 1] = t;
	if (!strcmp(q->priority_type, "edf"))
		sort_to_top(q);
}

t_thread_vars	*top_priority(t_priority_queue *q)
{
	t_thread_vars	*answer;

	if (q->size)
	{
		answer = q->elemets[0];
		q->size--;
		if (!q->size)
			return (answer);
		q->elemets[0] = q->elemets[q->size];
		q->elemets[q->size] = NULL;
		if (!strcmp(q->priority_type, "edf"))
			sort_to_down(q, q->size);
	}
	else
		answer = NULL;
	return (answer);
}

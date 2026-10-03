#include "codexion.h"

void	free_all_coders(t_thread_vars	**coders, int n)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		free(coders[i]->thread);
		free(coders[i]);
	}
	free(coders);
}


void	free_all_dongels(t_mutex_info	**dongles, int n)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		free(dongles[i]->mutex);
		free(dongles[i]);
	}
	free(dongles);
}

void	free_queue(t_priority_queue *my_queue)
{
	int				i;

	i = 0;
	if (!my_queue->size)
		free(my_queue);
	else
	{
		free(my_queue->elemets[0]->cond);
		free(my_queue->elemets[0]->queue_control);
		while (i < my_queue->size)
		{
			free(my_queue->elemets[i]->thread);
			free(my_queue->elemets[i]);
			i++;
		}
		free(my_queue);
	}
}
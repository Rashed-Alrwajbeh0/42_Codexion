#include "codexion.h"

long	calctime(struct timeval t)
{
	return ((1000 * t.tv_sec) + (t.tv_usec / 1000));
}

static int	init_one_dongle(t_dongle *dongle)
{
	dongle->dongle = malloc(sizeof(pthread_mutex_t));
	if (!dongle->dongle)
		return (0);
	if (pthread_mutex_init(dongle->dongle, NULL))
	{
		free(dongle->dongle);
		return (0);
	}
	dongle->is_in_use = 0;
	dongle->last_use = 0;
	return (1);
}

t_dongle	*init_dongles(t_arguments args)
{
	t_dongle	*dongles;
	int			i;

	dongles = malloc(args.number_of_coders * sizeof(t_dongle));
	if (!dongles)
		return (NULL);
	i = -1;
	while (++i < args.number_of_coders)
	{
		if (!init_one_dongle(&dongles[i]))
		{
			free_all_dongels(dongles, i);
			return (NULL);
		}
	}
	return (dongles);
}

t_queue	*init_queue(t_arguments args)
{
	t_queue	*my_queue;

	my_queue = malloc(sizeof(t_queue));
	if (!my_queue)
		return (NULL);
	my_queue->coders = malloc(args.number_of_coders * sizeof(t_coder *));
	if (!my_queue->coders)
		return (free(my_queue), NULL);
	my_queue->size = 0;
	return (my_queue);
}

#include "codexion.h"

static pthread_mutex_t	*new_mutex(void)
{
	pthread_mutex_t	*m;

	m = malloc(sizeof(pthread_mutex_t));
	if (!m)
		return (NULL);
	if (pthread_mutex_init(m, NULL))
		return (free(m), NULL);
	return (m);
}

static int	fill_common_vars(t_common_vars *common)
{
	common->stop = malloc(sizeof(int));
	common->print = new_mutex();
	common->queue_controler = new_mutex();
	if (!common->stop || !common->print || !common->queue_controler)
		return (0);
	*common->stop = 0;
	return (1);
}

static void	free_partial(t_common_vars *common)
{
	free(common->stop);
	free(common->print);
	free(common->queue_controler);
	free(common);
}

t_common_vars	*init_common_vars(t_dongle *dongles, t_arguments args,
					t_queue *my_queue)
{
	t_common_vars	*common;

	common = malloc(sizeof(t_common_vars));
	if (!common)
		return (NULL);
	common->stop = NULL;
	common->print = NULL;
	common->queue_controler = NULL;
	if (!fill_common_vars(common))
		return (free_partial(common), NULL);
	common->args = args;
	common->dongles = dongles;
	common->mutexes = dongles;
	common->my_queue = my_queue;
	return (common);
}

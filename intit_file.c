#include "codexion.h"



t_mutex_info	**dongles_init(int number_of_dongels)
{
	pthread_mutex_t	*m;
	t_mutex_info	**dongles;
	t_mutex_info	*one_dongle;
	int				i;

	dongles = malloc(number_of_dongels * sizeof(t_mutex_info *) + sizeof(void *));
	i = 0;
	while (i < number_of_dongels)
	{
		m = malloc(sizeof(pthread_mutex_t));
		if (!m)
			return (free_all_dongels(dongles, i), NULL);
		one_dongle = malloc(sizeof(t_mutex_info));
		if (!one_dongle)
			return (free(m), free_all_dongels(dongles, i), NULL);
		one_dongle->mutex = m;
		if (pthread_mutex_init(m, NULL))
			return (free(m), free(one_dongle),
			free_all_dongels(dongles, i), NULL);
		one_dongle->activaited = 1;
		one_dongle->in_use = 0;
		dongles[i] = one_dongle;
		i++;
	}
	dongles[i] = NULL;
	return (dongles);
}



t_thread_vars	**coders_init(t_arguments argumetns,
								t_mutex_info **dongels,
								int *burn_out,
								pthread_mutex_t *print)
{
	t_thread_vars	**coders;
	t_thread_vars	*one_coder;
	pthread_t		*th;
	pthread_mutex_t	*m;
	pthread_cond_t	*cond;
	int				i;
	int				n;

	m = malloc(sizeof(pthread_mutex_t));
	if (!m)
		return (NULL);
	if (pthread_mutex_init(m, NULL))
		return (free(m), NULL);
	cond = malloc(sizeof(pthread_cond_t));
	if (!cond)
		return (free(m), NULL);
	if (pthread_cond_init(cond, NULL))
		return (free(cond), free(m), NULL);
	n = argumetns.number_of_coders;
	coders = malloc(sizeof(t_thread_vars*) * n + sizeof(void*));
	if (!coders)
		return (free(cond), free(m), NULL);
	i = 0;
	while (i < n)
	{
		th = malloc(sizeof(pthread_t));
		if (!th)
			return (free_all_coders(coders, i), NULL);
		one_coder = malloc(sizeof(t_thread_vars));
		if (!one_coder)
			return (free(th), free_all_coders(coders, i), NULL);
		one_coder->thread = th;
		one_coder->id = i + 1;
		one_coder->arguments = argumetns;
		one_coder->burn_out = burn_out;
		one_coder->finished = 0;
		one_coder->print_mutex = print;
		one_coder->finish_first_compilation = 0;
		one_coder->left_dongle = dongels[(i - 1 + n) % n];
		one_coder->right_dongle = dongels[i % n];
		one_coder->queue_control = m;
		one_coder->cond = cond;
		coders[i] = one_coder;
		i++;
	}
	coders[i] = NULL;
	return (coders);
}

t_priority_queue	*queue_init(t_thread_vars **threads, char *scheduler)
{
	t_priority_queue	*my_queue;
	int					i;

	my_queue = malloc(sizeof(t_priority_queue)
			+ sizeof(t_thread_vars *)
			* (threads[0]->arguments.number_of_coders));
	if (!my_queue)
		return (NULL);
	i = -1;
	my_queue->size = 0;
	my_queue->priority_type = scheduler;
	while (threads[++i])
	{
		add_to_queue(my_queue, threads[i]);
	}
	return (my_queue);
}

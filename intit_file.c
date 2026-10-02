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
								pthread_mutex_t *print,
								struct timeval *start_program
							)
{
	t_thread_vars	**coders;
	t_thread_vars	*one_coder;
	pthread_t		*th;
	int				i;
	int				n;

	n = argumetns.number_of_coders;
	coders = malloc(sizeof(t_thread_vars*) * n + sizeof(void*));
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
		one_coder->left_dongle = dongels[(i - 1 + n) % n]->mutex;
		one_coder->right_dongle = dongels[i % n]->mutex;
		coders[i] = one_coder;
		i++;
	}
	coders[i] = NULL;
	return (coders);
}

t_priority_queue	*queue_init(t_thread_vars **threads, char *scheduler)
{

}
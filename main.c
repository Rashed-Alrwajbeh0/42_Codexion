/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 09:34:01 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/28 09:34:03 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void *test(void *Data){
	t_compose		*data;
	t_thread_vars	*t_threead;
	struct timeval	*current_time;
	struct timeval	*start;

	data = (t_compose *)Data;
	t_threead = data->thread;
	start = malloc(sizeof(struct timeval));
	current_time = malloc(sizeof(struct timeval));
	gettimeofday(start, NULL);
	while (t_threead->arguments.number_of_compiles_required)
	{

		gettimeofday(current_time, NULL);
		if (t_threead->id % 2)
		{
			pthread_mutex_lock(t_threead->right_dongle);
			printf("%d %d has taken a dongle\n",
				current_time->tv_usec - start->tv_usec, t_threead->id);
			pthread_mutex_lock(t_threead->left_dongle);
			printf("%d %d has taken a dongle\n",
				current_time->tv_usec - start->tv_usec, t_threead->id);

		}
		else
		{
			pthread_mutex_lock(t_threead->left_dongle);
			printf("%d %d has taken a dongle\n",
				current_time->tv_usec - start->tv_usec, t_threead->id);
			pthread_mutex_lock(t_threead->right_dongle);
			printf("%d %d has taken a dongle\n",
				current_time->tv_usec - start->tv_usec, t_threead->id);
		}
		current_time = malloc(sizeof(struct timeval));
		gettimeofday(current_time, NULL);
		usleep(200000);
		if (t_threead->id % 2)
		{
			pthread_mutex_unlock(t_threead->left_dongle);
			pthread_mutex_unlock(t_threead->right_dongle);

		}
		else
		{
			pthread_mutex_unlock(t_threead->right_dongle);
			pthread_mutex_unlock(t_threead->left_dongle);

		}
		pthread_mutex_lock(t_threead->print_mutex);
		printf("%d Test\n", t_threead->id);
		pthread_mutex_unlock(t_threead->print_mutex);
		return (NULL);
	}
}



int	main(int argc, char *argv[]){
	t_thread_vars		**coders;
	t_mutex_info		**dongles;
	t_arguments			*checked_arg;
	t_thread_vars		*temp_coder;
	t_priority_queue	*my_queue;
	pthread_mutex_t		*print_mutex;
	char				*scheduler;
	t_compose			*Data;
	int					burn_out;
	int					i;


	if (argc != 9)
		return (printf("Enter just 8 arguments, no more no less !!\n"), 0);
	checked_arg = check_arg(argv);
	if (!checked_arg)
		return (0);
		if (!strcmp("fifo", argv[8]) || !strcmp("edf", argv[8]))
		scheduler = argv[8];
	else
		return (free(checked_arg), printf("Error in the arguments !!\n"), 0);

	burn_out = 0;
	print_mutex = malloc(sizeof(pthread_mutex_t));
	if (!print_mutex)
		return (free(checked_arg), free(scheduler), 0);
	dongles = dongles_init(checked_arg->number_of_coders);
	if (!dongles)
		return (free(print_mutex), free(checked_arg), free(scheduler), 0);
	coders = coders_init(*checked_arg, dongles, &burn_out, print_mutex);
	if (!coders)
		return (free(checked_arg),
			free(scheduler),
			free(print_mutex),
			free_all_dongels(dongles, checked_arg->number_of_coders),
			0);
	my_queue = queue_init(coders, scheduler);
	if (!my_queue)
		return (free(checked_arg),
			free(scheduler),
			free(print_mutex),
			free_all_dongels(dongles, checked_arg->number_of_coders),
			free_all_coders(coders, checked_arg->number_of_coders),
			0);
	while (my_queue->size)
	{
		Data = malloc(sizeof(t_compose));
		if (!Data)
			return (free(checked_arg),
				free(scheduler),
				free(print_mutex),
				free_all_dongels(dongles, checked_arg->number_of_coders),
				free_all_coders(coders, checked_arg->number_of_coders),
				free_queue(my_queue),
				0);
		temp_coder = top_priority(my_queue);
		Data->queue = my_queue;
		Data->thread = temp_coder;
		Data->top_priority_in_the_queue = temp_coder;
		if (pthread_create(temp_coder->thread, NULL, &test, Data))
			return (free(checked_arg),
				free(scheduler),
				free(print_mutex),
				free_all_dongels(dongles, checked_arg->number_of_coders),
				free_all_coders(coders, checked_arg->number_of_coders),
				free_queue(my_queue),
				0);	}
	i = -1;
	while (++i < checked_arg->number_of_coders)
		pthread_join(*coders[i]->thread, NULL);
}

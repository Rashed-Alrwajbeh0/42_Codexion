// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/09/28 09:34:01 by ralrawaj          #+#    #+#             */
// /*   Updated: 2026/09/28 09:34:03 by ralrawaj         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "codexion.h"


// void	print(t_thread_vars *th, char *txt, int time)
// {
// 	pthread_mutex_lock(th->print_mutex);
// 	printf("%d %d %s\n", time, th->id, txt);
// 	pthread_mutex_unlock(th->print_mutex);
// }

// int	check_burnout(t_priority_queue *my_queue, t_thread_vars *my_thread)
// {
// 	struct timeval	*current_time;

// 	current_time = malloc(sizeof(struct timeval));
// 	if (!current_time)
// 		return (-1);
// 	gettimeofday(current_time, NULL);
// 	if (calctime(*current_time) - calctime(my_thread->last_compilation_time)
// 		< my_thread->arguments.time_to_burnout)
// 		return (0);
// 	my_queue->burnout_thread = malloc(sizeof(t_thread_vars));
// 	if (!my_queue->burnout_thread)
// 		return (0);
// 	my_queue->burnout_thread = my_thread;
// 	*(my_thread->burn_out) = 1;
// 	free(current_time);
// 	return (1);
// }

// void	even_threads(t_compose *data)
// {
// 	struct timeval	*current_time;
// 	struct timeval	*start_time;
// 	t_thread_vars	*my_thread;
// 	int				l;

// 	current_time = malloc(sizeof(struct timeval));
// 	if (!current_time)
// 		return ;
// 	my_thread = data->thread;
// 	start_time = data->start_time;
// 	while (my_thread->arguments.number_of_compiles_required && !(*my_thread->burn_out))
// 	{
		
// 		pthread_mutex_lock(my_thread->queue_control);
// 		while (get_top_priority(data->queue)->id != my_thread->id)
// 		{
// 			pthread_cond_wait(my_thread->cond, my_thread->queue_control);
// 		}
// 		//pthread_mutex_unlock(my_thread->queue_control);
// 		if (my_thread->right_dongle->activaited || my_thread->left_dongle->activaited)
// 		{
// 			// printf("ddfdfdfdfd: %d\n", my_thread->arguments.dongle_cooldown * 1000);
// 			gettimeofday(current_time, NULL);
// 			usleep(my_thread->arguments.dongle_cooldown * 1000);
// 			my_thread->right_dongle->activaited = 0;
// 			my_thread->left_dongle->activaited = 0;
// 		}
// 		if (*my_thread->burn_out)
// 		{
// 			break;
// 		}
// 		gettimeofday(current_time, NULL);
// 		pthread_mutex_unlock(my_thread->queue_control);
// 		if (*my_thread->burn_out)
// 		{
// 			pthread_mutex_unlock(my_thread->queue_control);
// 			break;
// 		}
// 		pthread_mutex_lock(my_thread->right_dongle->mutex);
// 		print(my_thread, "has taken a dongle", calctime(*current_time) - calctime(*start_time));
// 		pthread_mutex_lock(my_thread->left_dongle->mutex);
// 		gettimeofday(current_time, NULL);
// 		print(my_thread, "has taken a dongle", calctime(*current_time) - calctime(*start_time));
// 		if (*my_thread->burn_out)
// 			break;
// 		gettimeofday(current_time, NULL);
// 		print(my_thread, "is compiling", calctime(*current_time) - calctime(*start_time));
// 		usleep(my_thread->arguments.time_to_compile * 1000);
// 		my_thread->right_dongle->activaited = 1;
// 		my_thread->left_dongle->activaited = 1;
// 		my_thread->last_compilation_time = *current_time;
// 		if (!my_thread->finish_first_compilation)
// 			my_thread->finish_first_compilation = 1;
// 		pthread_mutex_unlock(my_thread->right_dongle->mutex);
// 		pthread_mutex_unlock(my_thread->left_dongle->mutex);
// 		pthread_mutex_lock(my_thread->queue_control);
// 		top_priority(data->queue);
// 		pthread_mutex_unlock(my_thread->queue_control);
// 		pthread_cond_broadcast(my_thread->cond);
// 		if (*my_thread->burn_out)
// 			break;
// 		gettimeofday(current_time, NULL);
// 		print(my_thread, "is debugging", calctime(*current_time) - calctime(*start_time));
// 		if (*my_thread->burn_out)
// 			break;
// 		usleep(my_thread->arguments.time_to_debug * 1000);
// 		if (*my_thread->burn_out)
// 			break;
// 		gettimeofday(current_time, NULL);
// 		print(my_thread, "is refactoring", calctime(*current_time) - calctime(*start_time));
// 		if (*my_thread->burn_out)
// 			break;
// 		usleep(my_thread->arguments.time_to_refactor * 1000);
// 		my_thread->arguments.number_of_compiles_required--;
// 		if (my_thread->arguments.number_of_compiles_required)
// 		{
// 			pthread_mutex_lock(my_thread->queue_control);
// 			add_to_queue(data->queue, my_thread);
// 			pthread_mutex_unlock(my_thread->queue_control);
// 			pthread_cond_broadcast(my_thread->cond);
// 		}
// 		l = check_burnout(data->queue, my_thread);
// 		if (l == -1)
// 			return (pthread_cond_broadcast(my_thread->cond), free(current_time));
// 	}
// 	pthread_cond_broadcast(my_thread->cond);
// 	*(data->finished_for_now )+= 1;
// }


// void	*monitor_coder_function(void *Data)
// {
// 	t_compose		*data;
// 	struct timeval	*current_time;

// 	data = malloc(sizeof(t_compose));
// 	if (!data)
// 		return (NULL);
// 	current_time = malloc(sizeof(struct timeval));
// 	if (!current_time)
// 		return (free(data), NULL);
// 	data = (t_compose *)Data;
// 	while (*(data->finished_for_now) < *(data->finished_threads))
// 	{
// 		if (data->queue->burnout_thread)
// 		{
// 			gettimeofday(current_time, NULL);
// 			print(data->queue->burnout_thread, "burned out", calctime(*current_time) - calctime(*(data->start_time)));
// 			return (NULL);
// 		}
// 	}
// 	return (NULL);
// }


// void *test(void *Data)
// {
// 	t_compose		*data;
// 	//t_thread_vars	*t_threead;


// 	data = (t_compose *)Data;
// 	//t_threead = data->thread;
// 	//if (t_threead->id % 2)
// 	//	even_threads(data);
// 	//else
// 	//	even_threads(data);
// 	even_threads(data);
// }



// int	main(int argc, char *argv[]){
// 	t_thread_vars		**coders;
// 	pthread_t			*monitor_coder;
// 	t_mutex_info		**dongles;
// 	t_arguments			*checked_arg;
// 	t_priority_queue	*my_queue;
// 	pthread_mutex_t		*print_mutex;
// 	char				*scheduler;
// 	t_compose			*Data;
// 	struct timeval		*start_time;
// 	int					*k;
// 	int					*h;
// 	int					burn_out;
// 	int					i;


// 	if (argc != 9)
// 		return (printf("Enter just 8 arguments, no more no less !!\n"), 0);
// 	checked_arg = check_arg(argv);
// 	if (!checked_arg)
// 		return (0);
// 		if (!strcmp("fifo", argv[8]) || !strcmp("edf", argv[8]))
// 		scheduler = argv[8];
// 	else
// 		return (free(checked_arg), printf("Error in the arguments !!\n"), 0);
// 	monitor_coder = malloc(sizeof(t_thread_vars));
// 	if (!monitor_coder)
// 		return (free(checked_arg), 0);
// 	k = malloc(sizeof(int));
// 	if (!k)
// 		return (free(monitor_coder), free(checked_arg), 0);
// 	*k = 0;
// 	h = malloc(sizeof(int));
// 	if (!h)
// 		return (free(monitor_coder), free(checked_arg), free(k), 0);
// 	*h = checked_arg->number_of_coders;
// 	burn_out = 0;
// 	print_mutex = malloc(sizeof(pthread_mutex_t));
// 	if (!print_mutex)
// 		return (free(monitor_coder), free(k), free(h), free(checked_arg), free(scheduler), 0);
// 	dongles = dongles_init(checked_arg->number_of_coders);
// 	if (!dongles)
// 		return (free(monitor_coder), free(k), free(h), free(print_mutex), free(checked_arg), free(scheduler), 0);
// 	if (checked_arg->number_of_coders == 1){
// 		printf("0 1 burned out\n");
// 		return(0);
// 	}
// 	coders = coders_init(*checked_arg, dongles, &burn_out, print_mutex);
// 	if (!coders)
// 		return (free(monitor_coder), free(k), free(h), free(checked_arg),
// 			free(scheduler),
// 			free(print_mutex),
// 			free_all_dongels(dongles, checked_arg->number_of_coders),
// 			0);
// 	my_queue = queue_init(coders, scheduler);
// 	if (!my_queue)
// 		return (free(monitor_coder), free(k), free(h), free(checked_arg),
// 			free(scheduler),
// 			free(print_mutex),
// 			free_all_dongels(dongles, checked_arg->number_of_coders),
// 			free_all_coders(coders, checked_arg->number_of_coders),
// 			0);
// 	i = -1;
// 	start_time = malloc(sizeof(struct timeval));
// 	if (!start_time)
// 		return (free(monitor_coder), free(k), free(h), free(checked_arg),
// 			free(scheduler),
// 			free_queue(my_queue),
// 			free(print_mutex),
// 			free_all_dongels(dongles, checked_arg->number_of_coders),
// 			free_all_coders(coders, checked_arg->number_of_coders),
// 			0);
// 	gettimeofday(start_time, NULL);
// 	while (coders[++i])
// 	{
		
// 		Data = malloc(sizeof(t_compose));
// 		if (!Data)
// 			return (free(monitor_coder), free(k), free(h), free(checked_arg),
// 				free(scheduler),
// 				free(print_mutex),
// 				free_all_dongels(dongles, checked_arg->number_of_coders),
// 				free_all_coders(coders, checked_arg->number_of_coders),
// 				free_queue(my_queue),
// 				0);
// 		Data->queue = my_queue;
// 		Data->start_time = start_time;
// 		Data->thread = coders[i];
// 		Data->finished_for_now = k;
// 		Data->finished_threads = h;
// 		if (pthread_create(coders[i]->thread, NULL, &test, Data))
// 			return (free(monitor_coder), free(k), free(h), free(checked_arg),
// 				free(scheduler),
// 				free(print_mutex),
// 				free_all_dongels(dongles, checked_arg->number_of_coders),
// 				free_all_coders(coders, checked_arg->number_of_coders),
// 				free_queue(my_queue), 0);
// 	}
// 	// printf("%d %d\n", my_queue->elemets[0]->id, my_queue->elemets[1]->id);
// 	if (pthread_create(monitor_coder, NULL, &monitor_coder_function, Data))
// 		return (free(checked_arg),
// 			free(scheduler),
// 			free(print_mutex),
// 			free_all_dongels(dongles, checked_arg->number_of_coders),
// 			free_all_coders(coders, checked_arg->number_of_coders),
// 			free_queue(my_queue), 0);	
// 	i = -1;
// 	while (++i < checked_arg->number_of_coders && !burn_out)
// 	{	
		
// 		pthread_join(*coders[i]->thread, NULL);
// 	}
// 	pthread_join(*monitor_coder, NULL);
// }

int	main()
{
	
}

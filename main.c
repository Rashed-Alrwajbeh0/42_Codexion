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
	t_thread_vars	*data;
	struct timeval	*current_time;

	data = (t_thread_vars*) Data;
	if (data->id % 2)
	{
		pthread_mutex_lock(data->right_dongle);
		pthread_mutex_lock(data->left_dongle);

	}
	else
	{
		pthread_mutex_lock(data->left_dongle);
		pthread_mutex_lock(data->right_dongle);
	}
	current_time = malloc(sizeof(struct timeval));
	gettimeofday(current_time, NULL);
	usleep(200000);
	if (data->id % 2)
	{
		pthread_mutex_unlock(data->left_dongle);
		pthread_mutex_unlock(data->right_dongle);

	}
	else
	{
		pthread_mutex_unlock(data->right_dongle);
		pthread_mutex_unlock(data->left_dongle);

	}
	pthread_mutex_lock(data->print_mutex);
	printf("%d %d Test\n", calctime(*current_time) -  calctime(*(data->strart_program)),data->id);
	pthread_mutex_unlock(data->print_mutex);
	return (NULL);
}

void	fill(t_thread_vars **_threads, t_mutex_info **_mutex, t_arguments arguments)
{
	int				i;
	int				n;
	t_mutex_info	*m;
	pthread_mutex_t	*mm;
	pthread_t		*th;
	t_thread_vars	*temp;

	i = -1;
	n = arguments.number_of_coders;
	while (++i < n)
	{
		m = malloc(sizeof(t_mutex_info));
		mm = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(mm, NULL);
		m->mutex = mm;
		m->activaited = 0;
		_mutex[i] = m;
	}
	i = -1;
	while (++i < n)
	{
		th = malloc(sizeof(pthread_t));
		temp = malloc(sizeof(t_thread_vars));
		temp->id = i + 1;
		temp->arguments = arguments;
		temp->right_dongle = _mutex[i % n]->mutex;
		temp->left_dongle = _mutex[(i - 1 + n) % n]->mutex;
		temp->thread = th;
		temp->finished = 0;
		_threads[i] = temp;
	}
	th = malloc(sizeof(pthread_t));
	temp = malloc(sizeof(t_thread_vars));
	temp->id = i + 1;
	temp->arguments = arguments;
	temp->right_dongle = _mutex[i % n]->mutex;
	temp->left_dongle = _mutex[(i - 1 + n) % n]->mutex;
	temp->thread = th;
	temp->finished = 0;
	_threads[i] = temp;
	i++;
	_threads[i] = NULL;
}

void	stop_now(t_mutex_info **_mutex, t_thread_vars **_threads)
{
	int		i;

	i = -1;
	while (_mutex[++i])
	{
		free(_threads[i]->thread);
		free(_threads[i]->strart_program);
		free(_threads[i]->print_mutex);
		free(_threads[i]);
		free(_mutex[i]);
	}
}

int	len(t_thread_vars **threads){
	int	i;

	i = -1;
	while (threads[++i])
		;
	return (i);
}

void	*moniter(void *data){
	int					con;
	t_thread_vars		**_threads;

	_threads = (t_thread_vars **)data;
	while (1)
	{
		if (_threads[i].)
	}
	
}

int	main(int argc, char *argv[])
{
	char				*scheduler;
	t_arguments			*checked_arg;
	t_thread_vars		**_threads;
	t_mutex_info		**_mutex;
	pthread_mutex_t		*print_mutex;
	struct timeval		*time;
	t_priority_queue	*q;

	if (argc != 9)
	{
		printf("Please enter just 8 arguments, no more no less !!\n");
		return (0);
	}
	checked_arg = check_arg(argv);
	if (!checked_arg)
		return (0);
	if (!strcmp("fifo", argv[8]) || !strcmp("edf", argv[8]))
		scheduler = argv[8];
	else
		return (free(checked_arg), printf("Error in the arguments !!\n"), 0);
	_threads = malloc(sizeof(t_thread_vars*) * (checked_arg->number_of_coders + 2));
	_mutex = malloc(sizeof(t_mutex_info*) * checked_arg->number_of_coders);
	print_mutex = malloc(sizeof(pthread_mutex_t));
	time = malloc(sizeof(struct timeval));
	q = malloc(sizeof(t_priority_queue) + sizeof(t_thread_vars*) * checked_arg->number_of_coders);
	q->priority_type = scheduler;
	pthread_mutex_init(print_mutex, NULL);
	fill(_threads, _mutex, *checked_arg);
	gettimeofday(time, NULL);
	for (int i = 0; i < checked_arg->number_of_coders; i++){
		_threads[i]->print_mutex = print_mutex;
		_threads[i]->strart_program = time;
		_threads[i]->last_compilation_time = *time;
		pthread_create(_threads[i]->thread, NULL, &test, _threads[i]);
		add_to_queue(q, _threads[i]);
	}
	for (int i = 0; i < checked_arg->number_of_coders; i++){
		pthread_join(*(_threads[i]->thread), NULL);
	}
	return (free(checked_arg), 0);
}

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
	current_time = malloc(sizeof(struct timeval));
	gettimeofday(current_time, NULL);
	
	printf("%d %d Test\n", calctime(*current_time) -  calctime(*(data->strart_program)),data->id);
	return NULL;
}

void	fill(t_thread_vars **_threads, pthread_mutex_t **_mutex, t_arguments arguments)
{
	int				i;
	int				n;
	pthread_mutex_t	*m;
	pthread_t		*th;
	t_thread_vars	*temp;

	i = -1;
	n = arguments.number_of_coders;
	while (++i < n)
	{
		m = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(m, NULL);
		_mutex[i] = m;
	}
	i = -1;
	while (++i < n)
	{
		th = malloc(sizeof(pthread_t));
		temp = malloc(sizeof(t_thread_vars));
		temp->id = i + 1;
		temp->arguments = arguments;
		temp->right_dongle = _mutex[i % n];
		temp->left_dongle = _mutex[(i - 1 + n) % n];
		temp->thread = th;
		_threads[i] = temp;
	}
}

int	main(int argc, char *argv[])
{
	char			*scheduler;
	t_arguments		*checked_arg;
	t_thread_vars	**_threads;
	pthread_mutex_t	**_mutex;
	pthread_mutex_t	*print_mutex;
	struct timeval	*time;

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
	_threads = malloc(sizeof(t_thread_vars*) * checked_arg->number_of_coders);
	_mutex = malloc(sizeof(pthread_mutex_t*) * checked_arg->number_of_coders);
	print_mutex = malloc(sizeof(pthread_mutex_t));
	time = malloc(sizeof(struct timeval));
	pthread_mutex_init(print_mutex, NULL);
	fill(_threads, _mutex, *checked_arg);
	gettimeofday(time, NULL);
	for (int i = 0; i < checked_arg->number_of_coders; i++){
		_threads[i]->print_mutex = print_mutex;
		_threads[i]->strart_program = time;
		pthread_create(_threads[i]->thread, NULL, &test, _threads[i]);
	}

	for (int i = 0; i < checked_arg->number_of_coders; i++){
		pthread_join(*(_threads[i]->thread), NULL);
	}
	return (free(checked_arg), 0);
}

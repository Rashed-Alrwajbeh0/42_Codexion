/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 12:28:15 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/23 12:28:18 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = {2, 3, 5, 7, 9, 11, 13, 17, 19, 23};

typedef struct	data{
	pthread_mutex_t	*mutex;
	int				*sum;
	int				start_idx;
	int				end_idx;
} data;


void*	calc_sum(void* Data){
	int				*sum;
	int				start;
	int				end;
	data			*data_;
	pthread_mutex_t	*m;

	data_ = (data*) Data;
	start = data_->start_idx;
	end = data_->end_idx;
	printf("Start: %d\n", start);
	sum = data_->sum;
	m = data_->mutex;
	for (; start < end; start++){
		pthread_mutex_lock(m);
		*sum += primes[start];
		printf("sum: %d\n", *sum);
		pthread_mutex_unlock(m);
	}
	return NULL;
}


int	main(void){
	pthread_mutex_t m;
	pthread_t		thread1;
	pthread_t		thread2;
	data			Data1;
	data			Data2;
	int				sum;

	pthread_mutex_init(&m, NULL);
	sum = 0;
	Data1.mutex = &m;
	Data1.sum = &sum;
	Data1.start_idx = 0;
	Data1.end_idx = 5;
	Data2.mutex = &m;
	Data2.sum = &sum;
	Data2.start_idx = 5;
	Data2.end_idx = 10;
	if (pthread_create(&thread1, NULL, &calc_sum, &Data1))
		return 1;
	if (pthread_create(&thread2, NULL, &calc_sum, &Data2))
		return 1;
	printf("%d\n", sum);
	if (pthread_join(thread1, NULL))
		return 0;
	if (pthread_join(thread2, NULL))
		return 0;
	printf("%d\n", sum);
	pthread_mutex_destroy(&m);
}

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

typedef	struct	argv{
	pthread_mutex_t	*mutrex;
	int				x;	
}	argv;

void	*do_something(void *Data)
{
	argv	*data;
	data = Data;
	data->x++;
	return (NULL);
}

int	main(void)
{
	pthread_mutex_t		m;
	pthread_t			Threads[4];
	argv 				Data;

	Data.mutrex = &m;
	Data.x = 0;
	pthread_mutex_init(&m, NULL);
	for (int i  = 0; i < 4; i++)
	{
		Data.x++;
		if (pthread_create(&Threads[i], NULL, &do_something, &Data))
			return (1);
		printf("Thread %d is start !\n", i);
	}
	sleep(2);
	for (int i  = 0; i < 4; i++)
	{
		if (pthread_join(Threads[i], NULL))
			return (1);
		printf("Thread %d is stop !\n", i);
	}
	pthread_mutex_destroy(&m);
	printf("X: %d\n", Data.x);
	return (0);

}

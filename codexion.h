/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 09:19:13 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/28 09:19:14 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h> 

typedef struct arguments
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
}	t_arguments;

typedef struct thread_vars
{
	t_arguments		arguments;
	pthread_t		thread;
	struct timeval	last_compilation_time;
}	t_thread_vars;

typedef struct queue
{
	int				size;
	t_thread_vars	elemets[];
}	t_priority_queue;
t_arguments	*check_arg(char *args[]);
int	edf(t_thread_vars t1, t_thread_vars t2);
int	fifo(t_priority_queue *q, t_thread_vars t);
#endif

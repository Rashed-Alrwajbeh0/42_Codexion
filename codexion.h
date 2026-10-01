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
# include <unistd.h>

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
	pthread_t		*thread;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
	struct timeval	*strart_program;
	struct timeval	last_compilation_time;
	t_arguments		arguments;
	int				finished;
	int				id;

}	t_thread_vars;

typedef struct queue
{
	int				size;
	char			*priority_type;
	t_thread_vars	*elemets[];
}	t_priority_queue;

typedef struct	mutex_info
{
	pthread_mutex_t	*mutex;
	int				activaited;
}	t_mutex_info;

t_arguments		*check_arg(char *args[]);
t_thread_vars	*top_priority(t_priority_queue *q);
int				edf(t_thread_vars *t1, t_thread_vars *t2);
void			swap(t_priority_queue *q, int *current, char dir);
void			add_to_queue(t_priority_queue *q, t_thread_vars *t);
int				calctime(struct timeval t);

#endif

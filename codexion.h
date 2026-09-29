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
	int				id;
	pthread_t		*thread;
	struct timeval	last_compilation_time;
	t_arguments		arguments;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
	struct timeval	*strart_program;

}	t_thread_vars;

typedef struct queue
{
	char			*priority_type;
	int				size;
	t_thread_vars	*elemets[];
}	t_priority_queue;

t_arguments	*check_arg(char *args[]);
int			edf(t_thread_vars t1, t_thread_vars t2);
int			calctime(struct timeval t);

#endif

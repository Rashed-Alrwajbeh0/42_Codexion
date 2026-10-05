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

typedef struct	mutex_info
{
	pthread_mutex_t	*mutex;
	struct timeval	last_use;
	int				activaited;
	int				in_use;
}	t_mutex_info;

typedef struct thread_vars
{
	int				*burn_out;
	pthread_t		*thread;
	pthread_mutex_t	*print_mutex;
	t_mutex_info	*left_dongle;
	t_mutex_info	*right_dongle;
	pthread_mutex_t	*queue_control;
	pthread_cond_t	*cond;
	struct timeval	last_compilation_time;
	t_arguments		arguments;
	int				finished;
	int				id;
	int				finish_first_compilation;

}	t_thread_vars;

typedef struct queue
{
	char			*priority_type;
	t_thread_vars	*burnout_thread;
	int				size;
	t_thread_vars	*elemets[];
}	t_priority_queue;

typedef struct compose
{
	t_thread_vars		*top_priority_in_the_queue;
	t_thread_vars		*thread;
	t_priority_queue	*queue;
	struct timeval		*start_time;
	int					*finished_threads;
	int					*finished_for_now;
}	t_compose;

t_arguments			*check_arg(char *args[]);
t_thread_vars		*top_priority(t_priority_queue *q);
t_thread_vars		*get_top_priority(t_priority_queue *q);
int					edf(t_thread_vars *t1, t_thread_vars *t2);
void				swap(t_priority_queue *q, int *current, char dir);
void				free_all_dongels(t_mutex_info	**dongles, int n);
void				free_all_coders(t_thread_vars	**coders, int n);
void				add_to_queue(t_priority_queue *q, t_thread_vars *t);
void				free_queue(t_priority_queue *my_queue);
int					calctime(struct timeval t);
t_mutex_info		**dongles_init(int number_of_dongels);
t_thread_vars		**coders_init(t_arguments argumetns,
						t_mutex_info **dongels,
						int *burn_out,
						pthread_mutex_t *print);
t_priority_queue	*queue_init(t_thread_vars **threads, char *scheduler);

#endif

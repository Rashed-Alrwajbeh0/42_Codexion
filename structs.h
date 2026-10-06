#ifndef STRUCTS
# define STRUCTS

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

typedef struct arguments
{
	char	*scheduler;
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
}	t_arguments;

typedef struct dongle
{
	pthread_mutex_t	*dongle;
	struct timeval	*last_use;
	int				is_in_use;
}	t_dongle;

typedef struct coder
{
	int				*burn_out;
	pthread_t		*thread;
	pthread_mutex_t	*print_mutex;
	t_dongle		left_dongle;
	t_dongle		right_dongle;
	pthread_mutex_t	*queue_control;
	pthread_cond_t	*cond;
	long	last_compilation_time;
	long			program_start_time;
	int				id;
}	t_coder;

typedef struct priority_queue
{
	t_coder	**coders;
	int		size;
}	t_queue;

typedef struct common_vars
{
	pthread_mutex_t	*print;
	pthread_mutex_t	*queue_controler;
	int				*stop;
	t_arguments		args;
	t_dongle		*dongles;
	t_queue			*my_queue;
	t_dongle		*mutexes;
}	t_common_vars;


typedef struct threads_arguments
{
	t_coder			**coders;
	t_coder			*burnout_coder;
	t_common_vars	*common;
	int				*finished_coders;
	int				coder_idx;

}	t_threads_args;
#endif
#include "codexion.h"

static void	take_dongles(t_coder *coder,
		int coder_idx,
		int number_of_coders)
{
	if (coder_idx == number_of_coders - 1){
		pthread_mutex_lock(coder->left_dongle->dongle);
		pthread_mutex_lock(coder->right_dongle->dongle);
	}
	else
	{
		pthread_mutex_lock(coder->right_dongle->dongle);
		pthread_mutex_lock(coder->left_dongle->dongle);
	}
	print(coder, "has taken a dongle");
	print(coder, "has taken a dongle");
}

static void	release_dongles(t_coder *coder, t_common_vars *common)
{
	struct timeval	now;

	pthread_mutex_unlock(coder->right_dongle->dongle);
	pthread_mutex_unlock(coder->left_dongle->dongle);
	gettimeofday(&now, NULL);
	coder->left_dongle->last_use = calctime(now);
	coder->right_dongle->last_use = calctime(now);
	pthread_mutex_lock(common->queue_controler);
	coder->right_dongle->is_in_use = 0;
	coder->left_dongle->is_in_use = 0;
	pthread_mutex_unlock(common->queue_controler);
}

static void	compiling(t_threads_args *args)
{
	struct timeval	now;
	t_coder			*coder;
	t_common_vars	*common;

	common = args->common;
	coder = args->coders[args->coder_idx];
	take_dongles(coder, args->coder_idx, args->common->args.number_of_coders);
	print(coder, "is compiling");
	gettimeofday(&now, NULL);
	pthread_mutex_lock(args->common->queue_controler);
	coder->last_compilation_time = calctime(now);
	pthread_mutex_unlock(args->common->queue_controler);
	usleep(common->args.time_to_compile * 1000);
	release_dongles(coder, common);
	print(coder, "is debugging");
	usleep(common->args.time_to_debug * 1000);
	print(coder, "is refactoring");
	usleep(common->args.time_to_refactor * 1000);
}

static void	wait_turn(t_threads_args *args, t_coder *coder)
{
	pthread_mutex_t	*lock;

	lock = args->common->queue_controler;
	pthread_mutex_lock(lock);
	while (!coder->is_ready)
		pthread_cond_wait(coder->cond, lock);
	coder->is_ready = 0;
	pthread_mutex_unlock(lock);
}

void	*coder_work(void *data)
{
	t_threads_args	*args;
	t_coder			*coder;
	int				i;

	args = (t_threads_args *)data;
	coder = args->coders[args->coder_idx];
	i = -1;
	while (++i < args->common->args.number_of_compiles_required
		&& !(*args->burnout_coder))
	{
		wait_turn(args, coder);
		pthread_mutex_lock(args->common->queue_controler);
		if (*args->burnout_coder)
		{
			pthread_mutex_unlock(args->common->queue_controler);
			return (free(args), NULL);
		}
		pthread_mutex_unlock(args->common->queue_controler);
		compiling(args);
		if (i + 1 != args->common->args.number_of_compiles_required)
		{
			pthread_mutex_lock(args->common->queue_controler);
			add_to_queue(args->common->my_queue, coder, *args->common);
			pthread_mutex_unlock(args->common->queue_controler);
		}
	}
	pthread_mutex_lock(args->common->queue_controler);
	(*args->finished_coders)++;
	coder->is_start = 0;
	pthread_mutex_unlock(args->common->queue_controler);
	return (free(args), NULL);
}

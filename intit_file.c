#include "codexion.h"

long	calctime(struct timeval t)
{
	return ((1000 * t.tv_sec) + (t.tv_usec / 1000));
}

t_dongle	*init_dongles(t_arguments args)
{
	t_dongle		*dongles;
	t_dongle		temp_dongle;
	pthread_mutex_t	*m;
	int				i;

	dongles = malloc(args.number_of_coders * sizeof(t_dongle));
	if (!dongles)
		return (NULL);
	i = -1;
	while (++i < args.number_of_coders)
	{
		m = malloc(sizeof(pthread_mutex_t));
		if (!m)
			return (free(dongles), free_all_dongels(dongles, i), NULL);
		temp_dongle.dongle = m;
		if (pthread_mutex_init(m, NULL))
			return (free(dongles), free_all_dongels(dongles, i), NULL);
		temp_dongle.is_in_use = 0;
		temp_dongle.last_use = 0;
		dongles[i] = temp_dongle;
	}
	return (dongles);
}

t_queue	*init_queue(t_arguments args)
{
	t_queue	*my_queue;

	my_queue = malloc(sizeof(t_queue)
		+ args.number_of_coders * sizeof(t_coder*));
	my_queue->coders = malloc(args.number_of_coders * sizeof(t_coder*));
	if (!my_queue)
		return (NULL);
	my_queue->size = 0;
	return (my_queue);
}


static t_common_vars	*init_common_vars(t_dongle *dongles,
							t_arguments args,
							t_queue *my_queue)
{
	t_common_vars	*common_vars;
	int				*stop;
	pthread_mutex_t	*print;
	pthread_mutex_t	*queue_controler;

	common_vars = malloc(sizeof(t_common_vars));
	if (!common_vars)
		return (NULL);
	stop = malloc(sizeof(int));
	if (!stop)
		return (free(common_vars), NULL);
	common_vars->stop = stop;
	print = malloc(sizeof(pthread_mutex_t));
	if (!print)
		return (free(common_vars), free(stop), NULL);
	queue_controler = malloc(sizeof(pthread_mutex_t));
	if (!queue_controler)
		return (free(common_vars), free(stop), free(print), NULL);
	pthread_mutex_init(print, NULL);
	pthread_mutex_init(queue_controler, NULL);
	common_vars->queue_controler = queue_controler;
	common_vars->args = args;
	common_vars->dongles = dongles;
	common_vars->mutexes = init_dongles(args);
	common_vars->my_queue = my_queue;
	common_vars->print = print;
	return (common_vars);
}





t_coder *help_with_intit_coders(int i, t_common_vars *common, long start_time)
{
	t_coder			*temp_coder;
	pthread_t		*th;
	int				n;

	temp_coder = malloc(sizeof(t_coder));
	if (!temp_coder)
		return (NULL);
	th = malloc(sizeof(pthread_t));
	if (!th)
		return (free(temp_coder), NULL);
	n = common->args.number_of_coders;
	temp_coder->thread = th;
	temp_coder->id = i;
	temp_coder->last_compilation_time = -1;
	temp_coder->program_start_time = start_time;
	temp_coder->left_dongle = &common->dongles[(i - 1 + n) % n];
	temp_coder->right_dongle = &common->dongles[i % n];
	temp_coder->print_mutex = common->print;
	temp_coder->queue_control = common->queue_controler;
	temp_coder->burn_out = common->stop;
	temp_coder->is_ready = 0;
	return (temp_coder);
}

t_coder	**intit_coders(t_common_vars *commn)
{
	t_coder			**coders;
	struct timeval	*now;
	t_coder			*temp_coder;
	
	int				i;

	now = malloc(sizeof(struct timeval));
	if (!now)
		return (NULL);
	coders = malloc(commn->args.number_of_coders * sizeof(t_coder*));
	if (!coders)
		return (free(now), NULL);
	gettimeofday(now, NULL);
	i = -1;
	while (++i < commn->args.number_of_coders)
	{
		temp_coder = help_with_intit_coders(i, commn, calctime(*now));
		if (!temp_coder)
			return (free_all_coders(coders, i), free(now), NULL);
		coders[i] = temp_coder;
	}
	return (coders);
}



int	compiling(t_threads_args *my_args, int last_compile)
{
	struct timeval *current_time;
	t_coder	*coder;

	current_time = malloc(sizeof(struct timeval));
	if (!current_time)
		return (0);
	coder = my_args->coders[my_args->coder_idx];
	pthread_mutex_lock(coder->right_dongle->dongle);
	pthread_mutex_lock(coder->left_dongle->dongle);
	if (my_args->burnout_coder)
		return (free(current_time), 0);
	if (!print(coder, "has taken a dongle"))
		return (free(current_time), 0);
	if (my_args->burnout_coder)
		return (free(current_time), 0);
	if (!print(coder, "has taken a dongle"))
		return (free(current_time), 0);
	if (my_args->burnout_coder)
		return (free(current_time), 0);
	if (!print(coder, "is compiling"))
		return (free(current_time), 0);
	gettimeofday(current_time, NULL);
	coder->last_compilation_time = calctime(*current_time);
	usleep(my_args->common->args.time_to_compile * 1000);
	pthread_mutex_unlock(coder->right_dongle->dongle);
	pthread_mutex_unlock(coder->left_dongle->dongle);
	gettimeofday(current_time, NULL);
	coder->left_dongle->last_use = calctime(*current_time);
	coder->right_dongle->last_use = calctime(*current_time);
	pthread_mutex_lock(my_args->common->queue_controler);
	coder->right_dongle->is_in_use = 0;
	coder->left_dongle->is_in_use = 0;
	pthread_mutex_unlock(my_args->common->queue_controler);
	if (my_args->burnout_coder)
		return (free(current_time), 0);
	if (!print(coder, "is debugging"))
		return (free(current_time), 0);
	usleep(my_args->common->args.time_to_debug * 1000);
	if (my_args->burnout_coder)
		return (free(current_time), 0);
	if (!print(coder, "is refactoring"))
		return (free(current_time), 0);
	usleep(my_args->common->args.time_to_refactor * 1000);
	if (!last_compile)
	{
		pthread_mutex_lock(my_args->common->queue_controler);
		add_to_queue(my_args->common->my_queue, coder, *my_args->common);
		pthread_mutex_unlock(my_args->common->queue_controler);
	}
	return (free(current_time), 1);
}

void	*coder_work(void* Data)
{
	int				i;
	t_threads_args *my_args;
	pthread_mutex_t	*temp;
	t_coder			*temp_coder;

	i = -1;
	my_args = (t_threads_args *)Data;
	//printf("", my_args-)
	while(++i < my_args->common->args.number_of_compiles_required
		&& !my_args->burnout_coder)
	{
		//printf("%d\n", i);
		temp = my_args->common->queue_controler;
		temp_coder = my_args->coders[my_args->coder_idx];
		pthread_mutex_lock(temp);
		while (!temp_coder->is_ready)
			pthread_cond_wait(temp_coder->cond, temp);
		temp_coder->is_ready = 0;
		pthread_mutex_unlock(temp);
		if (i + 1 == my_args->common->args.number_of_compiles_required)
			compiling(my_args, 1);
		else
			compiling(my_args, 0);
	}
	pthread_mutex_lock(my_args->common->queue_controler);
	(*my_args->finished_coders)++;
	pthread_mutex_unlock(my_args->common->queue_controler);
}


void	*monitor_function(void *Data)
{
	t_threads_args	*threads_args;
	struct timeval	*current_time;
	int				j;
	t_coder			*temp_coder;
	t_coder			**temp_coders;

	current_time = malloc(sizeof(struct timeval));
	if (!current_time)
		return (NULL);
	threads_args = (t_threads_args *)Data;
	temp_coders = malloc(threads_args->common->args.number_of_coders * sizeof(t_coder *));
	if (!temp_coders)
		return (free(threads_args), free(current_time), NULL);
	temp_coder = malloc(sizeof(t_coder));
	if (!temp_coder)
		return (free(temp_coders), free(threads_args), free(current_time), NULL);
	
	while (*threads_args->finished_coders < threads_args->common->args.number_of_coders)
	{
		//usleep(100000);
		j = -1;
		while (threads_args->common->my_queue->size)
		{
			pthread_mutex_lock(threads_args->common->queue_controler);
			temp_coder = top_priority(threads_args->common->my_queue, *threads_args->common);
			pthread_mutex_unlock(threads_args->common->queue_controler);
			if (!temp_coder)
				break;
			gettimeofday(current_time, NULL);
			if (is_available(temp_coder, calctime(*current_time), threads_args->common)
			)
			{
				pthread_mutex_lock(threads_args->common->queue_controler);
				temp_coder->left_dongle->is_in_use = 1;
				temp_coder->right_dongle->is_in_use = 1;
				temp_coder->is_ready = 1;
				pthread_mutex_unlock(threads_args->common->queue_controler);
				pthread_cond_signal(temp_coder->cond);

			}
			else
				temp_coders[++j] = temp_coder;
		}
		temp_coders[++j] = NULL;
		j = -1;
		while (temp_coders[++j])
		{
			pthread_mutex_lock(threads_args->common->queue_controler);
			add_to_queue(threads_args->common->my_queue, temp_coders[j], *threads_args->common);
			pthread_mutex_unlock(threads_args->common->queue_controler);
		}
	}
	
}

int start_coders(t_coder **coders, t_common_vars *commn)
{
	t_threads_args	*threads_args;
	t_coder			*bournout_coder;
	pthread_t		*monitor;
	int				*finished_coders;
	int				idx;

	monitor = malloc(sizeof(pthread_t));
	if (!monitor)
		return (0);
			bournout_coder = malloc(sizeof(t_coder));
	if (!bournout_coder)
		return (free(monitor), 0);
	finished_coders = malloc(sizeof(int));
	if (!finished_coders)
		return (free(monitor), free(bournout_coder), 0);
	bournout_coder = NULL;
	*finished_coders = 0;
	idx = -1;
	while (++idx < commn->args.number_of_coders)	
	{
		threads_args = malloc(sizeof(t_threads_args));
		if (!threads_args)
			return (free(monitor), free(bournout_coder), free(finished_coders), 0);
		threads_args->burnout_coder = bournout_coder;
		threads_args->coders = coders;
		threads_args->common = commn;
		threads_args->coder_idx = idx;
		threads_args->finished_coders = finished_coders;
		pthread_create(coders[idx]->thread, NULL, &coder_work, threads_args);

	}
	idx = -1;
	threads_args = malloc(sizeof(t_threads_args));
	
	if (!threads_args)
		return (free(monitor), free(bournout_coder), free(finished_coders), 0);
			threads_args->burnout_coder = bournout_coder;
	threads_args->coders = coders;
	threads_args->common = commn;
	threads_args->coder_idx = idx;
	threads_args->finished_coders = finished_coders;
	pthread_create(monitor, NULL, &monitor_function, threads_args);
	while (++idx < commn->args.number_of_coders)
		pthread_join(*coders[idx]->thread, NULL);
	pthread_join(*monitor, NULL);
	

}

int	make_threads(t_arguments args, t_dongle *dongles, t_queue *my_queue)
{
	t_common_vars	*common;
	t_coder			**coders;
	int				i;

	common = init_common_vars(dongles, args, my_queue);
	if (!common)
		return(0);
	coders = intit_coders(common);
	if (!put_cond_variables(coders, common))
		return(free_all_coders(coders,
						common->args.number_of_coders),
						free_common_variables(common), 0);
	i = -1;
	while (++i < common->args.number_of_coders)
		add_to_queue(my_queue, coders[i], *common);
	start_coders(coders, common);
		
}
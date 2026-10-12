#include "codexion.h"

void	free_all_cond(t_coder **coders, int end)
{
	int	start;

	start = -1;
	while (++start < end)
		free(coders[start]->cond);
	
}

void	free_common_variables(t_common_vars *common)
{
	free(common->print);
	free(common->stop);
	free(common->queue_controler);
	free(common);
}

void	free_after_finish(t_coder **coders, t_common_vars common)
{
	int	i;
	int	n_coders;

	i = -1;
	n_coders = common.args.number_of_coders;
	while (++i < n_coders)
	{
		if (coders[i]->is_start)
		{
			pthread_mutex_lock(common.queue_controler);
			coders[i]->is_ready = 1;
			pthread_mutex_unlock(common.queue_controler);
			pthread_cond_signal(coders[i]->cond);
		}
	}
	
}

void	print(t_coder *coder, char *txt)
{
	struct timeval current_time;

	if (*coder->burn_out)
		return ;
	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(coder->print_mutex);
	printf("%ld %d %s\n",
		calctime(current_time)
		- coder->program_start_time,
		coder->id,
		txt
	);
	pthread_mutex_unlock(coder->print_mutex);
}

void	burned_out_print(t_coder *coder, char *txt)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(coder->print_mutex);
	printf("%ld %d %s\n",
		calctime(current_time)
		- coder->program_start_time,
		coder->id,
		txt
	);
	pthread_mutex_unlock(coder->print_mutex);
}

int	is_available(t_coder *coder, t_common_vars common)
{
	struct timeval	time;
	long			current_time;

	gettimeofday(&time, NULL);
	current_time = calctime(time);
	if (!coder->left_dongle->is_in_use && !coder->right_dongle->is_in_use)
	{
		if (!coder->right_dongle->last_use && !coder->left_dongle->last_use)
			return (1);
		else if (!coder->right_dongle->last_use && coder->left_dongle->last_use
			&& current_time - coder->left_dongle->last_use >= common.args.dongle_cooldown)
			return (1);
		else if (!coder->left_dongle->last_use && coder->right_dongle->last_use
			&& current_time - coder->right_dongle->last_use >= common.args.dongle_cooldown)
			return (1);
		else if (coder->right_dongle->last_use && coder->left_dongle->last_use 
			&& current_time - coder->right_dongle->last_use >= common.args.dongle_cooldown 
			&& current_time - coder->left_dongle->last_use >= common.args.dongle_cooldown)
			return(1);
		return (0);
	}
	return (0);
}

int	put_cond_variables(t_coder **coders, t_common_vars *commom)
{
	int				i;
	pthread_cond_t	*cond;

	i = -1;
	while (++i < commom->args.number_of_coders)
	{
		cond = malloc(sizeof(pthread_cond_t));
		if (!cond)
			return (free_all_cond(coders, i), 0);
		pthread_cond_init(cond, NULL);
		coders[i]->cond = cond;
	}
	return (1);
}
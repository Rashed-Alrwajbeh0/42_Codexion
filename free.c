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
	free_all_dongels(common->dongles, common->args.number_of_coders);
	free(common);
}

int	print(t_coder *coder, char *txt)
{
	struct timeval *current_time;

	current_time = malloc(sizeof(struct timeval));
	if (!current_time)
		return (0);
	gettimeofday(current_time, NULL);
	printf("%d %d %s\n",
		calctime(*current_time)
		- coder->program_start_time,
		coder->id,
		txt
	);
	free(current_time);
	return (1);
}

int	is_available(t_coder *coder, long current_time, t_common_vars *common)
{
	if (!coder->left_dongle->is_in_use && !coder->right_dongle->is_in_use)
	{
		if (!coder->right_dongle->last_use && !coder->left_dongle->last_use)
			return (1);
		else if (!coder->right_dongle->last_use && coder->left_dongle->last_use
			&& current_time - coder->left_dongle->last_use >= common->args.dongle_cooldown)
			return (1);
		else if (!coder->left_dongle->last_use && coder->right_dongle->last_use
			&& current_time - coder->right_dongle->last_use >= common->args.dongle_cooldown)
			return (1);
		else if (current_time - coder->right_dongle->last_use >= common->args.dongle_cooldown 
		&& current_time - coder->left_dongle->last_use >= common->args.dongle_cooldown)
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
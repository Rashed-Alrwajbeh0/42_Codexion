/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 10:05:05 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 10:05:07 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print(t_coder *coder, char *txt)
{
	struct timeval	current_time;

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
	struct timeval	current_time;

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
	int				cooldown;

	gettimeofday(&time, NULL);
	current_time = calctime(time);
	cooldown = common.args.dongle_cooldown;
	if (!coder->left_dongle->is_in_use && !coder->right_dongle->is_in_use)
	{
		if (!coder->right_dongle->last_use && !coder->left_dongle->last_use)
			return (1);
		else if (!coder->right_dongle->last_use && coder->left_dongle->last_use
			&& current_time - coder->left_dongle->last_use >= cooldown)
			return (1);
		else if (!coder->left_dongle->last_use && coder->right_dongle->last_use
			&& current_time - coder->right_dongle->last_use >= cooldown)
			return (1);
		else if (coder->right_dongle->last_use && coder->left_dongle->last_use
			&& current_time - coder->right_dongle->last_use >= cooldown
			&& current_time - coder->left_dongle->last_use >= cooldown)
			return (1);
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

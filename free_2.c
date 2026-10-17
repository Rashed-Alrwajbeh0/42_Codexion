/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 09:59:55 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 09:59:55 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all_cond(t_coder **coders, int end)
{
	int	start;

	start = -1;
	while (++start < end)
	{
		pthread_cond_destroy(coders[start]->cond);
		free(coders[start]->cond);
	}
}

void	free_common_variables(t_common_vars *common)
{
	pthread_mutex_destroy(common->print);
	pthread_mutex_destroy(common->queue_controler);
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

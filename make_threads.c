/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 10:06:17 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 10:06:18 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	fill_queue(t_common_vars *common, t_coder **coders)
{
	int	i;

	i = -1;
	while (++i < common->args.number_of_coders)
	{
		pthread_mutex_lock(common->queue_controler);
		add_to_queue(common->my_queue, coders[i], *common);
		pthread_mutex_unlock(common->queue_controler);
	}
}

int	make_threads(t_arguments args, t_dongle *dongles, t_queue *my_queue)
{
	t_common_vars	*common;
	t_coder			**coders;
	int				ret;

	common = init_common_vars(dongles, args, my_queue);
	if (!common)
		return (0);
	coders = init_coders(common);
	if (!coders)
		return (free_common_variables(common), 0);
	if (!put_cond_variables(coders, common))
		return (free_all_coders(coders, args.number_of_coders),
			free_common_variables(common), 0);
	fill_queue(common, coders);
	ret = start_coders(coders, common);
	free_all_cond(coders, args.number_of_coders);
	free_all_coders(coders, args.number_of_coders);
	free_common_variables(common);
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 09:55:12 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 09:55:13 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_coder	*new_coder(int i, t_common_vars *common, long start_time)
{
	t_coder	*coder;
	int		n;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);
	coder->thread = malloc(sizeof(pthread_t));
	if (!coder->thread)
		return (free(coder), NULL);
	n = common->args.number_of_coders;
	coder->id = i;
	coder->is_start = 1;
	coder->last_compilation_time = -1;
	coder->program_start_time = start_time;
	coder->left_dongle = &common->dongles[(i - 1 + n) % n];
	coder->right_dongle = &common->dongles[i % n];
	coder->print_mutex = common->print;
	coder->queue_control = common->queue_controler;
	coder->burn_out = common->stop;
	coder->is_ready = 0;
	return (coder);
}

t_coder	**init_coders(t_common_vars *common)
{
	t_coder			**coders;
	struct timeval	now;
	int				i;

	coders = malloc(common->args.number_of_coders * sizeof(t_coder *));
	if (!coders)
		return (NULL);
	gettimeofday(&now, NULL);
	i = -1;
	while (++i < common->args.number_of_coders)
	{
		coders[i] = new_coder(i + 1, common, calctime(now));
		if (!coders[i])
			return (free_all_coders(coders, i), NULL);
	}
	return (coders);
}

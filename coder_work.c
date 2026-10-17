/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_work.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 09:54:25 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 09:54:26 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_work(void *data)
{
	t_threads_args	*args;
	t_coder			*coder;

	args = (t_threads_args *)data;
	coder = args->coders[args->coder_idx];
	if (!help_coder_work(args, coder))
		return (NULL);
	pthread_mutex_lock(args->common->queue_controler);
	(*args->finished_coders)++;
	coder->is_start = 0;
	coder->last_compilation_time = -1;
	pthread_mutex_unlock(args->common->queue_controler);
	return (free(args), NULL);
}

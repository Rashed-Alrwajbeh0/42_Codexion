/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 09:55:55 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 09:55:56 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_threads_args *args)
{
	struct timeval	now;
	t_coder			*coder;
	int				i;

	gettimeofday(&now, NULL);
	i = -1;
	pthread_mutex_lock(args->common->queue_controler);
	while (++i < args->common->args.number_of_coders)
	{
		coder = args->coders[i];
		if (coder->last_compilation_time != -1
			&& calctime(now) - coder->last_compilation_time
			>= args->common->args.time_to_burnout)
		{
			*args->burnout_coder = coder;
			*coder->burn_out = 1;
			return (pthread_mutex_unlock(args->common->queue_controler), 1);
		}
	}
	return (pthread_mutex_unlock(args->common->queue_controler), 0);
}

static void	grant_dongles(t_coder *coder, t_common_vars *common)
{
	pthread_mutex_lock(common->queue_controler);
	coder->left_dongle->is_in_use = 1;
	coder->right_dongle->is_in_use = 1;
	coder->is_ready = 1;
	pthread_cond_signal(coder->cond);
	pthread_mutex_unlock(common->queue_controler);
}

static int	serve_queue(t_threads_args *args, t_coder **skipped)
{
	t_coder	*coder;
	int		size;
	int		j;
	int		k;

	pthread_mutex_lock(args->common->queue_controler);
	size = args->common->my_queue->size;
	pthread_mutex_unlock(args->common->queue_controler);
	j = -1;
	k = 0;
	while (++j < size)
	{
		pthread_mutex_lock(args->common->queue_controler);
		coder = top_priority(args->common->my_queue, *args->common);
		pthread_mutex_unlock(args->common->queue_controler);
		if (!coder)
			break ;
		if (is_available(coder, *args->common))
			grant_dongles(coder, args->common);
		else
			skipped[k++] = coder;
	}
	return (k);
}

static int	check_finish_coders(t_threads_args *args)
{
	int	ans;

	pthread_mutex_lock(args->common->queue_controler);
	ans = *args->finished_coders < args->common->args.number_of_coders;
	pthread_mutex_unlock(args->common->queue_controler);
	return (ans);
}

void	*monitor_function(void *data)
{
	t_threads_args	*args;
	t_coder			**skipped;
	int				k;

	args = (t_threads_args *)data;
	skipped = malloc(args->common->args.number_of_coders * sizeof(t_coder *));
	if (!skipped)
		return (free(args), NULL);
	while (check_finish_coders(args))
	{
		if (check_burnout(args))
		{
			free_after_finish(args->coders, *args->common);
			burned_out_print(*args->burnout_coder, "burned out");
			break ;
		}
		k = serve_queue(args, skipped);
		pthread_mutex_lock(args->common->queue_controler);
		add_to_queue_front(args->common->my_queue, k, skipped, *args->common);
		pthread_mutex_unlock(args->common->queue_controler);
		usleep(1);
	}
	return (free(skipped), free(args), NULL);
}

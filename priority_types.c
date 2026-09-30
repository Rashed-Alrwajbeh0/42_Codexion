/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_types.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 09:22:30 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/30 09:22:32 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	edf(t_thread_vars *t1, t_thread_vars *t2)
{
	if (calctime(t1->last_compilation_time) + t1->arguments.time_to_burnout
		<= calctime(t2->last_compilation_time) + t2->arguments.time_to_burnout)
		return (1);
	return (0);
}

int	calctime(struct timeval t)
{
	return ((1000 * t.tv_sec) + (t.tv_usec / 1000));
}

void	swap(t_priority_queue *q, int *current, char dir)
{
	t_thread_vars	*temp;
	int				another_idx;

	if (dir == 'r')
		another_idx = 2 * *current + 2;
	else
		another_idx = 2 * *current + 1;
	temp = q->elemets[*current];
	q->elemets[*current] = q->elemets[another_idx];
	q->elemets[another_idx] = temp;
	*current = another_idx;
}
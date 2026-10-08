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


int	edf(t_coder *t1, t_coder *t2, t_common_vars common)
{
	if (t1->last_compilation_time != -1 && t2->last_compilation_time != -1)
	{
		if (t1->last_compilation_time + common.args.time_to_burnout
			< t2->last_compilation_time + common.args.time_to_burnout)
			return (1);
		if (t1->last_compilation_time + common.args.time_to_burnout
			> t2->last_compilation_time + common.args.time_to_burnout)
			return(-1);
		return (0);
	}
	else if (t1->last_compilation_time != -1)
		return (-1);
	else if (t2->last_compilation_time != -1)
		return (1);
	else
	{
		if (t1->id < t2->id)
			return (1);
		return (-1);
	}
}

//int	calctime(struct timeval t)
//{
//	return ((1000 * t.tv_sec) + (t.tv_usec / 1000));
//}

void	swap(t_queue *q, int *current, char dir)
{
	t_coder			*temp;
	int				another_idx;

	if (dir == 'r')
		another_idx = 2 * *current + 2;
	else
		another_idx = 2 * *current + 1;
	temp = q->coders[*current];
	q->coders[*current] = q->coders[another_idx];
	q->coders[another_idx] = temp;
	*current = another_idx;
}
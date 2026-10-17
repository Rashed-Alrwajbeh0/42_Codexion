/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queu_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 10:11:48 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 10:11:50 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	add_to_queue(t_queue *q, t_coder *t, t_common_vars common)
{
	q->size++;
	q->coders[q->size - 1] = t;
	if (!strcmp(common.args.scheduler, "edf") && q->size != 1)
		sort_to_top(q, common);
}

int	add_to_queue_front(t_queue *my_queue, int n,
		t_coder **coders, t_common_vars common)
{
	int			idx;
	int			j;
	t_coder		**new_coders;

	if (!strcmp(common.args.scheduler, "edf"))
	{
		j = -1;
		while (++j < n)
			add_to_queue(my_queue, coders[j], common);
		return (1);
	}
	new_coders = malloc(common.args.number_of_coders * sizeof(t_coder *));
	if (!new_coders)
		return (0);
	idx = 0;
	j = -1;
	while (++j < n)
		new_coders[idx++] = coders[j];
	j = -1;
	while (++j < my_queue->size)
		new_coders[idx++] = my_queue->coders[j];
	free(my_queue->coders);
	my_queue->coders = new_coders;
	my_queue->size += n;
	return (1);
}

static void	help_function(t_queue *q)
{
	int		i;

	i = -1;
	while (++i < q->size)
		q->coders[i] = q->coders[i + 1];
	q->coders[q->size] = NULL;
}

t_coder	*top_priority(t_queue *q, t_common_vars common)
{
	t_coder	*answer;

	if (q->size)
	{
		answer = q->coders[0];
		q->size--;
		if (!strcmp(common.args.scheduler, "edf"))
		{
			if (!q->size)
				return (answer);
			q->coders[0] = q->coders[q->size];
			q->coders[q->size] = NULL;
			sort_to_down(q, q->size, common);
		}
		else if (!strcmp(common.args.scheduler, "fifo"))
		{
			if (!q->size)
				return (q->coders[0] = NULL, answer);
			help_function(q);
		}
		return (answer);
	}
	return (NULL);
}

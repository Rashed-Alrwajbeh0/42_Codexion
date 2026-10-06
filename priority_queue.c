/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 09:24:11 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/30 09:24:12 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static char	*determaine_child(t_queue *q, int q_size, int current,
			t_common_vars common)
{
	int				left;
	int				right;

	left = 2 * current + 1;
	right = 2 * current + 2;
	if (right < q_size && left < q_size
		&& edf((q->coders[right]), (q->coders[left]), common)
		&& edf((q->coders[right]), (q->coders[current]), common))
		return ("right");
	else if (left < q_size && right < q_size
		&& edf((q->coders[left]), (q->coders[right]), common)
		&& edf((q->coders[left]), (q->coders[current]), common))
		return ("left");
	else if (left < q_size && !(right < q_size)
		&& edf((q->coders[left]), (q->coders[current]), common))
		return ("left");
	else if (right < q_size && !(left < q_size)
		&& edf((q->coders[right]), (q->coders[current]), common))
		return ("right");
	return (NULL);
}


static void	sort_to_down(t_queue *q, int q_size, t_common_vars common)
{
	int				current;
	char			*child;

	current = 0;
	child = determaine_child(q, q_size, current, common);
	while (child)
	{
		if (!strcmp(child, "right"))
		{
			swap(q, &current, 'r');
			child = determaine_child(q, q_size, current, common);
		}
		else if (!strcmp(child, "left"))
		{
			swap(q, &current, 'l');
			child = determaine_child(q, q_size, current, common);
		}
		else
			break ;
	}
}

static void	sort_to_top(t_queue *q, t_common_vars common)
{
	int				parent;
	int				current;
	t_coder	*temp;

	current = q->size - 1;
	if (!current)
		return ;
	parent = (current - 1) / 2;
	while (1)
	{
		if (edf((q->coders[current]), (q->coders[parent]), common))
		{
			temp = q->coders[parent];
			q->coders[parent] = q->coders[current];
			q->coders[current] = temp;
			current = parent;
			parent = (current - 1) / 2;
		}
		else
			break ;
		if (!current)
			break ;
	}
}

void	add_to_queue(t_queue *q, t_coder *t, t_common_vars common)
{
	q->size++;
	printf("%d\n", q->size);
	q->coders[q->size - 1] = t;
	if (!strcmp(common.args.scheduler, "edf"))
		sort_to_top(q, common);
}

t_coder	*top_priority(t_queue *q, t_common_vars common)
{
	t_coder	*answer;

	if (q->size)
	{
		answer = q->coders[0];
		q->size--;
		if (!q->size)
			return (answer);
		q->coders[0] = q->coders[q->size];
		q->coders[q->size] = NULL;
		if (!strcmp(common.args.scheduler, "edf"))
			sort_to_down(q, q->size, common);
	}
	else
		answer = NULL;
	return (answer);
}

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

char	*determaine_child(t_queue *q, int q_size, int current,
			t_common_vars common)
{
	int				left;
	int				right;

	left = 2 * current + 1;
	right = 2 * current + 2;
	if (right < q_size && left < q_size
		&& edf((q->coders[right]), (q->coders[left]), common) == 1
		&& edf((q->coders[right]), (q->coders[current]), common) == 1)
		return ("right");
	else if (left < q_size && right < q_size
		&& edf((q->coders[left]), (q->coders[right]), common) != -1
		&& edf((q->coders[left]), (q->coders[current]), common) == 1)
		return ("left");
	else if (left < q_size && !(right < q_size)
		&& edf((q->coders[left]), (q->coders[current]), common) == 1)
		return ("left");
	else if (right < q_size && !(left < q_size)
		&& edf((q->coders[right]), (q->coders[current]), common) == 1)
		return ("right");
	return (NULL);
}

void	sort_to_down(t_queue *q, int q_size, t_common_vars common)
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

void	sort_to_top(t_queue *q, t_common_vars common)
{
	int				parent;
	int				current;
	t_coder			*temp;

	current = q->size - 1;
	if (!current)
		return ;
	parent = (current - 1) / 2;
	while (1)
	{
		if (edf((q->coders[current]), (q->coders[parent]), common) == 1)
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

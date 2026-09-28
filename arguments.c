/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 09:55:00 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/28 09:55:02 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	check_ints(int start, int end, char *args[])
{
	int	temp;

	while (start < end)
	{
		temp = atoi(args[start]);
		if (!temp || temp <= 0)
			return (0);
		start++;
	}
	return (1);
}

t_arguments	*check_arg(char *args[])
{
	t_arguments	*resault;

	resault = malloc(sizeof(t_arguments));
	if (!resault)
		return (0);
	if (check_ints(1, 8, args))
	{
		resault->number_of_coders = atoi(args[1]);
		resault->time_to_burnout = atoi(args[2]);
		resault->time_to_compile = atoi(args[3]);
		resault->time_to_debug = atoi(args[4]);
		resault->time_to_refactor = atoi(args[5]);
		resault->number_of_compiles_required = atoi(args[6]);
		resault->dongle_cooldown = atoi(args[7]);
		return (resault);
	}
	else
		return (free(resault), printf("Error in the arguments !!\n"), NULL);
}

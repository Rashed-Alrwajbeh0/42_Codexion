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

int	check_signal(char *text)
{
	int	i;
	int	ans;

	i = -1;
	ans = 0;
	while (text[++i])
	{
		if (text[i] == '-' || text[i] == '+')
			ans++;
		if (ans > 1)
			return (0);
	}
	return (1);
}

int	check_degits(char *text)
{
	int	i;
	int	temp;

	i = -1;
	while (text[++i])
	{
		temp = (int) text[i];
		if (!(temp >= 48 && temp <= 57))
		{
			if (temp != 45 && temp != 43)
				return (0);
		}
	}
	return (1);
}

int	check_zero(char *text)
{
	int	i;
	int	temp;

	if (!check_degits(text) || !check_signal(text))
		return (0);
	i = -1;
	while (text[++i])
	{
		temp = (int) text[i];
		if (temp != 48 && temp != 45 && temp != 43)
			return (0);
	}
	return (1);
}

int	check_ints(int start, int end, char *args[])
{
	int	temp;

	while (start < end)
	{
		if (!check_degits(args[start]) || !check_signal(args[start]))
			return (0);
		if (!start)
		{
			if (check_zero(args[start]))
				continue ;
			else
				return (0);
		}
		temp = atoi(args[start]);
		if (temp < 0)
			return (0);
		start++;
	}
	return (1);
}

t_arguments	*check_arg(char *args[], int argc)
{
	t_arguments	*resault;

	if (argc != 9)
		return (printf("Enter just 8 arguments, no more no less !!\n"), NULL);
	resault = malloc(sizeof(t_arguments));
	if (!resault)
		return (NULL);
	if (check_ints(1, 8, args))
	{
		resault->number_of_coders = atoi(args[1]);
		resault->time_to_burnout = atoi(args[2]);
		resault->time_to_compile = atoi(args[3]);
		resault->time_to_debug = atoi(args[4]);
		resault->time_to_refactor = atoi(args[5]);
		resault->number_of_compiles_required = atoi(args[6]);
		resault->dongle_cooldown = atoi(args[7]);
		if (!strcmp("fifo", args[8]) || !strcmp("edf", args[8]))
			resault->scheduler = args[8];
		else
			return (free(resault), printf("Error in the arguments!\n"), NULL);
		return (resault);
	}
	else
		return (free(resault), printf("Error in the arguments !!\n"), NULL);
}

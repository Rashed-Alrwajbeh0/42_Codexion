/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 10:14:15 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 10:14:16 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	single_case(t_arguments *args)
{
	printf("0 1 has taken a dongle\n");
	usleep(args->time_to_burnout * 1000);
	printf("%d 1 burned out\n", args->time_to_burnout);
	free(args);
}

int	main(int argc, char *argv[])
{
	t_arguments	*args;
	t_dongle	*dongles;
	t_queue		*my_queue;

	args = check_arg(argv, argc);
	if (!args)
		return (0);
	if (args->number_of_coders == 1)
		return (single_case(args), 0);
	dongles = init_dongles(*args);
	if (!dongles)
		return (free(args), 0);
	my_queue = init_queue(*args);
	if (!my_queue)
		return (
			free_all_dongels(dongles, args->number_of_coders),
			free(args),
			0);
	make_threads(*args, dongles, my_queue);
	free_all_dongels(dongles, args->number_of_coders);
	free(my_queue->coders);
	free(my_queue);
	free(args);
	return (0);
}

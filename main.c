/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 09:34:01 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/28 09:34:03 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int main(int argc, char *argv[])
{
	char		*scheduler;
	t_arguments	*checked_arg;

	if (argc != 9)
	{
		printf("Please enter just 8 arguments, no more no less !!\n");
		return (0);
	}
	checked_arg = check_arg(argv);
	if (!checked_arg)
		return (0);
	if (!strcmp("fifo", argv[8]) || !strcmp("edf", argv[8]))
		scheduler = argv[8];
	else
		return (free(checked_arg), printf("Error in the arguments !!\n"), 0);
	{
		printf("%d\n", checked_arg->number_of_coders);
		printf("%d\n", checked_arg->time_to_burnout);
		printf("%d\n", checked_arg->time_to_compile);
		printf("%d\n", checked_arg->time_to_debug);
		printf("%d\n", checked_arg->time_to_refactor);
		printf("%d\n", checked_arg->number_of_compiles_required);
		printf("%d\n", checked_arg->dongle_cooldown);
		printf("%s\n", scheduler);
	}
	return (free(checked_arg), 0);
}

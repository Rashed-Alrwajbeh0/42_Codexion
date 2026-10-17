/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_init_functions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 09:43:55 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/10/17 09:43:57 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all_coders(t_coder	**coders, int n)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		free(coders[i]->thread);
		free(coders[i]);
	}
	free(coders);
}

void	free_all_dongels(t_dongle *dongles, int n)
{
	int	i;

	i = -1;
	while (++i < n)
		free(dongles[i].dongle);
	free(dongles);
}

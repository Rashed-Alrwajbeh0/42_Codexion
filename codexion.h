/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralrawaj <ralrawaj@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 09:19:13 by ralrawaj          #+#    #+#             */
/*   Updated: 2026/09/28 09:19:14 by ralrawaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CODEXION_H
# define CODEXION_H

# include "structs.h"

t_arguments	*check_arg(char *args[], int argc);
t_dongle	*init_dongles(t_arguments args);
void		free_all_dongels(t_dongle *dongles, int n);
t_queue		*init_queue(t_arguments args);
int			make_threads(t_arguments args, t_dongle *dongles, t_queue *my_queue);
long		calctime(struct timeval t);
void	free_all_coders(t_coder	**coders, int n);
t_coder	*top_priority(t_queue *q, t_common_vars common);
void	add_to_queue(t_queue *q, t_coder *t, t_common_vars common);
int	edf(t_coder *t1, t_coder *t2, t_common_vars common);
void	swap(t_queue *q, int *current, char dir);
void	free_all_cond(t_coder **coders, int end);
void	free_common_variables(t_common_vars *common);
int	print(t_coder *coder, char *txt);
int	is_available(t_coder *coder, long current_time, t_common_vars *common);
int	put_cond_variables(t_coder **coders, t_common_vars *commom);

#endif

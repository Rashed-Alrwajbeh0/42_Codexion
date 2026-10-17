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

t_arguments		*check_arg(char *args[], int argc);
long			calctime(struct timeval t);
t_dongle		*init_dongles(t_arguments args);
t_queue			*init_queue(t_arguments args);
t_common_vars	*init_common_vars(t_dongle *dongles, t_arguments args,
					t_queue *my_queue);
t_coder			**init_coders(t_common_vars *common);
int				put_cond_variables(t_coder **coders, t_common_vars *commom);
int				make_threads(t_arguments args, t_dongle *dongles,
					t_queue *my_queue);
int				start_coders(t_coder **coders, t_common_vars *common);
void			*coder_work(void *data);
void			*monitor_function(void *data);
t_coder			*top_priority(t_queue *q, t_common_vars common);
void			add_to_queue(t_queue *q, t_coder *t, t_common_vars common);
int				add_to_queue_front(t_queue *q, int n, t_coder **c,
					t_common_vars common);
int				edf(t_coder *t1, t_coder *t2, t_common_vars common);
void			swap(t_queue *q, int *current, char dir);
int				is_available(t_coder *coder, t_common_vars common);
void			printf_queu(t_queue *q);
void			print(t_coder *coder, char *txt);
void			burned_out_print(t_coder *coder, char *txt);
void			free_all_dongels(t_dongle *dongles, int n);
void			free_all_coders(t_coder **coders, int n);
void			free_all_cond(t_coder **coders, int end);
void			free_common_variables(t_common_vars *common);
void			free_after_finish(t_coder **coders,
					t_common_vars common);
char			*determaine_child(t_queue *q, int q_size, int current,
					t_common_vars common);
void			sort_to_down(t_queue *q, int q_size, t_common_vars common);
void			sort_to_top(t_queue *q, t_common_vars common);
int				help_coder_work(t_threads_args *args, t_coder *coder);

#endif

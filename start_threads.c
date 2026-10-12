#include "codexion.h"

static int	init_shared_data(t_threads_args *shared, t_coder **coders,
				t_common_vars *common)
{
	shared->coders = coders;
	shared->common = common;
	shared->coder_idx = 0;
	shared->burnout_coder = malloc(sizeof(t_coder *));
	shared->finished_coders = malloc(sizeof(int));
	if (!shared->burnout_coder || !shared->finished_coders)
	{
		free(shared->burnout_coder);
		free(shared->finished_coders);
		return (0);
	}
	*shared->burnout_coder = NULL;
	*shared->finished_coders = 0;
	return (1);
}

static t_threads_args	*new_args(t_threads_args *shared, int idx)
{
	t_threads_args	*args;

	args = malloc(sizeof(t_threads_args));
	if (!args)
		return (NULL);
	*args = *shared;
	args->coder_idx = idx;
	return (args);
}

int	start_coders(t_coder **coders, t_common_vars *common)
{
	t_threads_args	shared;
	t_threads_args	*args;
	pthread_t		monitor;
	int				idx;

	if (!init_shared_data(&shared, coders, common))
		return (0);
	idx = -1;
	while (++idx < common->args.number_of_coders)
	{
		args = new_args(&shared, idx);
		if (!args)
			return (
					free(shared.burnout_coder),
					free(shared.finished_coders),
					0);
		if (pthread_create(coders[idx]->thread, NULL, &coder_work, args))
				return (
					free(shared.burnout_coder),
					free(shared.finished_coders),
					0);
	}
	args = new_args(&shared, -1);
	if (!args)
		return (0);
	idx = -1;
	if (pthread_create(&monitor, NULL, &monitor_function, args))
		return (
				free(shared.burnout_coder),
				free(shared.finished_coders),
				0);
	while (++idx < common->args.number_of_coders)
		pthread_join(*(coders[idx]->thread), NULL);
	pthread_join(monitor, NULL);
	return (
			free(shared.burnout_coder),
			free(shared.finished_coders),
			1);
}

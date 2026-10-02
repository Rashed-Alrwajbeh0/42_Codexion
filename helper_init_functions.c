#include "codexion.h"

void	free_all_coders(t_thread_vars	**coders, int n)
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


void	free_all_dongels(t_mutex_info	**dongles, int n)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		free(dongles[i]->mutex);
		free(dongles[i]);
	}
	free(dongles);
}
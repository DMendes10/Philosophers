/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 17:48:06 by diomende          #+#    #+#             */
/*   Updated: 2025/12/22 17:48:41 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	eat_limit_reached(t_master *master)
{
	int	i;

	i = 0;
	pthread_mutex_lock (&master->mutex->meal_lock);
	while (i < master->info->phil_count)
	{
		if (master->philo_arr[i].philo_hunger == IS_HUNGRY)
			break ;
		i++;
	}
	if (i == master->info->phil_count)
	{
		pthread_mutex_lock (&master->mutex->death_lock);
		master->info->all_philos_alive = false;
		pthread_mutex_unlock (&master->mutex->death_lock);
		pthread_mutex_unlock (&master->mutex->meal_lock);
		return (true);
	}
	pthread_mutex_unlock (&master->mutex->meal_lock);
	return (false);
}

bool	death_detector(t_master *master)
{
	int	i;

	i = 0;
	while (i < master->info->phil_count)
	{
		pthread_mutex_lock (&master->mutex->meal_lock);
		if (get_time() - master->philo_arr[i].lst_eat_tm > master->info->tm_die)
		{
			pthread_mutex_lock (&master->mutex->death_lock);
			master->info->all_philos_alive = false;
			pthread_mutex_lock (&master->mutex->write_lock);
			printf ("%ld %d %s\n", get_time() - master->info->start_time, \
master->philo_arr[i].phil_id, DEAD_TXT);
			pthread_mutex_unlock (&master->mutex->write_lock);
			pthread_mutex_unlock (&master->mutex->death_lock);
			pthread_mutex_unlock (&master->mutex->meal_lock);
			return (true);
		}
		pthread_mutex_unlock (&master->mutex->meal_lock);
		i++;
	}
	return (false);
}

void	waiter_routine(t_master *master)
{
	while (1)
	{
		if (master->info->eat_req != -1)
		{
			if (eat_limit_reached (master))
				return ;
		}
		if (death_detector (master))
			return ;
		usleep(500);
	}
}

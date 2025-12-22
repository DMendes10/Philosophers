/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:07:54 by diomende          #+#    #+#             */
/*   Updated: 2025/12/22 17:39:24 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_info(t_master *master, char **av)
{
	master->info = malloc (sizeof(t_info));
	if (!master->info)
		return (1);
	if (atoi (av[1]) < 0 || atoi (av[2]) < 0 || atoi (av[3]) \
< 0 || atoi (av[4]) < 0 || (av[5] && (atoi (av[5]) < 0)))
		return (write (2, ERR_INV_ARG, 43));
	master->info->all_philos_alive = true;
	master->info->start_time = get_time ();
	master->info->phil_count = atoi (av[1]);
	master->info->tm_die = ft_atoi (av[2]);
	master->info->tm_eat = ft_atoi (av[3]);
	master->info->tm_sleep = ft_atoi (av[4]);
	if (av[5])
		master->info->eat_req = ft_atoi (av[5]);
	else
		master->info->eat_req = -1;
	return (0);
}

int	fork_initializer(t_master *master)
{
	int	i;

	i = 0;
	while (i < master->info->phil_count)
	{
		if (pthread_mutex_init (&master->mutex->forks[i], NULL) != 0)
		{
			while (i >= 0)
				pthread_mutex_destroy (&master->mutex->forks[i--]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_mutex(t_master *master)
{
	int	i;

	i = 0;
	master->mutex = malloc (sizeof (t_mutex));
	if (!master->mutex)
		return (1);
	master->mutex->forks = malloc (sizeof (pthread_mutex_t) * master\
->info->phil_count);
	if (!master->mutex->forks)
		return (1);
	if (fork_initializer (master))
		return ((write (2, ERR_MUT, 30)));
	if (pthread_mutex_init (&master->mutex->write_lock, NULL) != 0)
		return (pthread_mutex_destroy (&master->mutex->write_lock), \
write (2, ERR_MUT, 30));
	if (pthread_mutex_init (&master->mutex->meal_lock, NULL) != 0)
		return (pthread_mutex_destroy (&master->mutex->meal_lock), \
write (2, ERR_MUT, 30));
	if (pthread_mutex_init (&master->mutex->death_lock, NULL) != 0)
		return (pthread_mutex_destroy (&master->mutex->death_lock), \
write (2, ERR_MUT, 30));
	return (0);
}

void	prep_philo(t_master *master, t_philo *philo, int i)
{
	philo->info = master->info;
	philo->mutex = master->mutex;
	philo->eat_count = 0;
	philo->philo_hunger = IS_HUNGRY;
	philo->lst_eat_tm = get_time();
	philo->phil_id = i + 1;
	philo->l_fork = &master->mutex->forks[i];
	if (i == 0)
		philo->r_fork = &master->mutex->forks[master->info->phil_count - 1];
	else
		philo->r_fork = &master->mutex->forks[i - 1];
}

int	init_table(t_master *master)
{
	int	i;

	i = 0;
	master->philo_arr = malloc (sizeof (t_philo) * master->info->phil_count);
	if (!master->philo_arr)
		return (1);
	while (i < master->info->phil_count)
	{
		prep_philo (master, &master->philo_arr[i], i);
		i++;
	}
	return (0);
}

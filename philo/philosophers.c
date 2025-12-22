/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:25:05 by diomende          #+#    #+#             */
/*   Updated: 2025/12/22 17:49:31 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	param_parsing(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	if (ac < 5 || ac > 6)
		return (write (2, ERR_ARG_NBR, 34));
	while (i < ac)
	{
		j = 0;
		while (av[i][j] != '\0')
		{
			if (!ft_isdigit(av[i][j]))
				return (write (2, ERR_INV_ARG, 43));
			j++;
		}
		i++;
	}
	return (0);
}

void	clean_master(t_master **master)
{
	int	i;

	i = 0;
	if ((*master)->mutex)
	{
		if ((*master)->mutex->forks)
		{
			while (i < (*master)->info->phil_count)
			{
				pthread_mutex_destroy (&(*master)->mutex->forks[i]);
				i++;
			}
		}
		pthread_mutex_destroy (&(*master)->mutex->death_lock);
		pthread_mutex_destroy (&(*master)->mutex->meal_lock);
		pthread_mutex_destroy (&(*master)->mutex->write_lock);
		free ((*master)->mutex->forks);
		free ((*master)->mutex);
	}
	if ((*master)->info)
		free ((*master)->info);
	if ((*master)->philo_arr)
		free ((*master)->philo_arr);
	free (*master);
}

bool	check_dead(t_philo *philo)
{
	bool	result;

	pthread_mutex_lock (&philo->mutex->death_lock);
	result = philo->info->all_philos_alive;
	pthread_mutex_unlock (&philo->mutex->death_lock);
	return (result);
}

void	start_diner(t_master *master)
{
	int	i;

	i = 0;
	while (i < master->info->phil_count)
	{
		pthread_create (&master->philo_arr[i].thread, NULL, \
&philo_routine, &master->philo_arr[i]);
		i++;
	}
	waiter_routine(master);
	i = 0;
	while (i < master->info->phil_count)
	{
		pthread_join(master->philo_arr[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_master	*master;
	int			p_nbr;

	if (param_parsing(ac, av))
		return (1);
	p_nbr = ft_atoi (av[1]);
	if (p_nbr == 1)
	{
		if (ft_atoi(av[2]) < 0)
			return (write (2, ERR_INV_ARG, 43));
		printf("0 1 has taken a fork\n");
		usleep(ft_atoi(av[2]) * 1000);
		printf("%li 1 died\n", ft_atoi(av[2]));
		return (0);
	}
	master = malloc (sizeof (t_master));
	if (!master)
		return (1);
	if (init_info(master, av) || init_mutex(master) || init_table(master))
		return (1);
	start_diner(master);
	clean_master (&master);
	return (0);
}

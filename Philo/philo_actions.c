/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogo <diogo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:45:13 by diogo             #+#    #+#             */
/*   Updated: 2025/12/21 19:09:14 by diogo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	grab_forks(t_philo *philo)
{
    if (philo->phil_id % 2 == 0)
    {
        pthread_mutex_lock (philo->r_fork);
        write_action (FORK_TXT, philo);
        pthread_mutex_lock (philo->l_fork);
        write_action (FORK_TXT, philo);
        return ;
    }
    pthread_mutex_lock (philo->l_fork);
    write_action (FORK_TXT, philo);
    pthread_mutex_lock (philo->r_fork);
    write_action (FORK_TXT, philo);
    return ;
}

void	philo_eats(t_philo *philo)
{
	grab_forks(philo);
	pthread_mutex_lock (&philo->mutex->meal_lock);
	write_action (EAT_TXT, philo);
	philo->lst_eat_tm = get_time ();
	philo->eat_count++;
	if (philo->eat_count == philo->info->eat_req)
		philo->philo_hunger = IS_FULL;
	pthread_mutex_unlock (&philo->mutex->meal_lock);
	usleep_manager(philo, philo->info->tm_eat);
	pthread_mutex_unlock (philo->r_fork);
	pthread_mutex_unlock (philo->l_fork);
}

void    philo_sleeps(t_philo *philo)
{
    write_action (SLEEP_TXT, philo);
    usleep_manager (philo, philo->info->tm_sleep);
}

void    philo_thinks(t_philo *philo)
{
    write_action (THINK_TXT, philo);
    usleep_manager (philo, 10);
    // ???????
}

void	*philo_routine(void *phil)
{
	t_philo *philo;
	
	philo = (t_philo *)phil;
	if (philo->phil_id % 2 == 0)
		usleep_manager (philo, 10);
	while (!check_dead(philo))
	{
		philo_eats(philo);
		if (!check_dead(philo))
			break ;
		philo_sleeps(philo);
		if (!check_dead(philo))
			break ;
		philo_thinks(philo);
	}
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:25:12 by diomende          #+#    #+#             */
/*   Updated: 2025/12/22 17:30:09 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	else
		return (0);
}

long	ft_atoi(const char *nptr)
{
	unsigned int	i;
	int				sign;
	long			result;

	i = 0;
	sign = 1;
	result = 0;
	while ((nptr[i] <= 13 && nptr[i] >= 9) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (ft_isdigit(nptr[i]) == 1)
	{
		result = (result * 10) + (nptr[i] - 48);
		i++;
	}
	return (result * sign);
}

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	write_action(char *action, t_philo *philo)
{
	long	time;

	if (!check_dead(philo))
		return ;
	pthread_mutex_lock (&philo->mutex->write_lock);
	time = get_time() - philo->info->start_time;
	printf ("%ld %d %s\n", time, philo->phil_id, action);
	pthread_mutex_unlock (&philo->mutex->write_lock);
}

void	usleep_manager(t_philo *philo, int time)
{
	long	start;
	long	elapsed;

	start = get_time();
	while (1)
	{
		if (!check_dead(philo))
			return ;
		elapsed = get_time() - start;
		if (elapsed >= time)
			break ;
		usleep(500);
	}
}

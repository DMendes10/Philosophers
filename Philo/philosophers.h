/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:25:10 by diomende          #+#    #+#             */
/*   Updated: 2025/12/16 17:13:32 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

# define ERR_ARG_NBR "ERROR: Wrong number of arguments\n"
# define ERR_INV_ARG "ERROR: Args must be a valid numeric value\n"

typedef struct s_mutex
{
	pthread_mutex_t	*forks;
}	t_mutex;

typedef struct s_info
{
	long	tm_die;
	long	tm_eat;
	long	tm_sleep;
	long	eat_req;
}t_info;

typedef struct s_philo
{
	pthread_t	thread;
	int			phil_nbr;
}	t_philo;

typedef struct s_master
{
	t_info		*info;
	t_philo		*philo_arr;
	t_mutex		*mutex;
}	t_master;

int	ft_atoi(const char *nptr);

#endif
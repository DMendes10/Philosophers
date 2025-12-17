/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:25:10 by diomende          #+#    #+#             */
/*   Updated: 2025/12/17 19:53:55 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

# define ERR_ARG_NBR "ERROR: Wrong number of arguments\n"
# define ERR_INV_ARG "ERROR: Args must be a valid numeric value\n"

typedef struct s_mutex
{
	pthread_mutex_t	*forks;
}	t_mutex;

typedef struct s_info
{
	int		phil_count;
	long	tm_die;
	long	tm_eat;
	long	tm_sleep;
	long	eat_req;
}t_info;

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	long			lst_eat_tm;
	int				phil_id;
	int				eat_count;
	int				philo_full;
}	t_philo;

typedef struct s_master
{
	t_info		*info;
	t_philo		*philo_arr;
	t_mutex		*mutex;
}	t_master;

long	ft_atoi(const char *nptr);
int		ft_isdigit(int c);
int		init_info(t_master *master, char **av);
int		init_mutex(t_master *master);
int		init_table(t_master *master);
void	prep_philo(t_master *master, t_philo *philo, int i);

#endif
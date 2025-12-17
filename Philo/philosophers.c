/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:25:05 by diomende          #+#    #+#             */
/*   Updated: 2025/12/17 20:17:49 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	param_parsing(int ac, char **av)
{
	int i;
	int j;

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
	int i;
	
	i = 0;
	if ((*master)->mutex)
	{
		if((*master)->mutex->forks)
		{
			while (i < (*master)->info->phil_count)
			{
				pthread_mutex_destroy (&(*master)->mutex->forks[i]);
				i++;
			}
		}
		free ((*master)->mutex->forks);
		free ((*master)->mutex);
	}
	if ((*master)->info)
		free ((*master)->info);
	if ((*master)->philo_arr)
		free ((*master)->philo_arr);
	free (*master);
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
	for (int i = 0; i < master->info->phil_count ;i++)
		printf ("%d\n", master->philo_arr[i].phil_id);
	start_diner();
	clean_master (&master);
	return (0);
	// start_dinner(mast)
}

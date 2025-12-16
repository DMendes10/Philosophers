/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diomende <diomende@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:25:05 by diomende          #+#    #+#             */
/*   Updated: 2025/12/16 16:25:06 by diomende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int param_parsing(int ac, char **av)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if (ac < 5 || ac > 6)
		return (write (2, ERR_ARG_NBR, 34));
	while (i < ac - 1)
	{
		j = 0;
		while (av[i][j] != '\0')
		{
			if (av[i][j] < 'A' || av[i][j] > 'Z' && av[i][j] < 'a' || av[i][j] > 'z')
				return (write (2, ERR_INV_ARG, 43));
			j++;
		}
		i++;
	}
	return (0);
}

int main(int ac, char **av)
{
	t_master	master;
	int		p_nbr;

	if (param_parsing(ac, av))
		return (1);
	p_nbr = ft_atoi (av[1]);
	phil_arr = malloc (p_nbr * sizeof (t_philo));

}

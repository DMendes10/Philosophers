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
				return (write (2, ERR_ARG_NBR, 43));
			j++;
		}
		i++;
	}
	return (0);
}

int main(int ac, char **av)
{
    t_master master;
	
	if (param_parsing(ac, av))
        return (1);
}

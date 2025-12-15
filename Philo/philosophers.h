#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

# define ERR_ARG_NBR "ERROR: Wrong number of arguments\n"
# define ERR_INV_ARG "ERROR: Args must be a valid numeric value\n"

typedef struct s_master
{
    int i;
}   t_master;

#endif
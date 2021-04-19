#ifndef INFO_H
#define INFO_H
#include <semaphore.h>

typedef struct info
{
	int salads_to_go;
	sem_t salad_lock;
	sem_t mkr_lock; // used to show whether ingredients are on the bench (ready)
	sem_t mkr_ready; // used to show whether maker is ready to get new batch of ingredients
	float bench[2]; //hold 2 ingredients given by the chef
	char ingres[3][50];
	float ingres_used[3]; //record amount of ingredients used by a maker
	int ingres_ideal[3];
}INFO;

#endif

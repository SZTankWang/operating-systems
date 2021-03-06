#ifndef INFO_H
#define INFO_H
#include <semaphore.h>

typedef struct info
{
	int salads_to_go;
	int counter; //indicate how many makers are available 
	sem_t salad_lock;
	sem_t counter_lock; // for accessing shared variable counter

	sem_t pick_lock; //used to notify the maker to get the ingredient
	sem_t mkr_ready; // used to show whether maker is ready to get new batch of ingredients
	sem_t in_need_lock; // used when access the ingers_in_need array


	float bench[2]; //hold 2 ingredients given by the chef
	char ingres[3][50];
	float ingres_used[3]; //record amount of ingredients used by a maker
	int ingres_ideal[3];
	float time_spent_making;
	double time_spent_waiting;
	int ingres_in_need[2]; //to show which ingredients \
							the maker needs, 1 for needed; 0 for not

	char filepath[12]; //the individual logging file name
	char sharedLog[12]; //the shared log file for recording busy mkr num
}INFO;

#endif

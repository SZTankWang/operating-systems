#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "myUtil.h"
#include "info.h"


int detach(INFO * shm, int mkrID){
	int detach = shmdt(shm);
	if(detach == 0){
		printf("MKR %d detached shm\n",mkrID);
		return detach;
	}
	else{
		perror("detach error");
		exit(-1);
	}


};


//get time elapsed since last set timer
long double timing(struct timeval before){
	struct timeval tv;
	double asec, desc;
	gettimeofday(&tv,NULL);

	asec = tv.tv_usec;
 	asec /= 1e6;
 	asec += tv.tv_sec;

 	desc = before.tv_usec;
 	desc /= 1e6;
 	desc += before.tv_sec;

	printf("INSIDE timing function: time elapsed is %Lf",(asec-desc));
	return (asec-desc);
};


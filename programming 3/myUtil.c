#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
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
double timing(struct timeval before,struct timeval after, int mkrID){
	struct timeval result;

	timersub(&after, &before, &result);

	double res = (double )result.tv_sec + ((double)result.tv_usec)/1e6;
	// printf("INSIDE timing function FOR MKR %d: time elapsed is %f\n",mkrID, res);
	return res;
};


char * timestamp(){
	time_t ltime;
	ltime = time(NULL);
	return asctime(localtime(&time));

};

char writeLogForWait(FILE * file,double time_used,char * timestamp){

}
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


void getTimeStamp(char * s){
	time_t ltime;
	// char * s = (char *) malloc(sizeof(char)*128);

	ltime = time(NULL);
	struct tm * p = localtime(&ltime);
	
	strftime(s,128,"%F %H:%M:%S ",p);

	// return s;
};

/*
FUNCTION writeLogForWait
int origin: specify where this wait comes from: either main loop or wait for ingredient loop \
0: main loop; 1: wait ingredient loop

*/
void writeLogForWait(FILE * file,double time_used,char * timestamp, int origin){
	if(origin ==0){
		fprintf(file,"%s: waited for chef to dispatch job for %f seconds\n",timestamp, time_used);

	}
	if(origin == 1){
		fprintf(file,"%s: waited for chef to put ingredient for %f seconds\n",timestamp, time_used);

	}


}

void writeLogForSalad(FILE * file,double time_used,char * timestamp, int saladCnt){

	fprintf(file,"%s: made another salad for %f seconds\n",timestamp, time_used);

}

void writeLogForMkrBye(FILE * file,char * timestamp, int saladCnt){
	fprintf(file,"%s: finished! I made %d salads in total\n",timestamp, saladCnt);

}


/*
function: writeLogForVege
used by salad maker, to put information when a maker receive a salad and finish weighing it
Parameter: int enough: 0: still need more for this vege; 1: already enough \
		   float weight: weight placed on the bench by chef this round
		   float total_received: shm[mkrID+1].ingresin the process for this salad, the total weight in the disposal for this vege 
*/
void writeLogForVege(FILE * file,char * timestamp, int enough,float weight,float total_received,char * ingre_name ){
	
	if(enough == 0){

		fprintf(file,"%s:  ingredient %s is not enough! JUST receive from bench: %f grams. I receive %f of %s in total\n",\
		timestamp,ingre_name,weight,total_received, ingre_name);

	}else{
		fprintf(file, "%s: ingredient %s is enough! I got %f grams! \n",timestamp,ingre_name,total_received);

	}

}

void writeLogForIngreReport(FILE * file, char * timestamp, char ingres[3][50], float * disposal){
	fprintf(file,"%s: finishes with receiving ingredients.\n", timestamp);
	for(int i=0;i<3;i++){
		fprintf(file,"%s at disposal %f grams.\n",ingres[i],disposal[i]);
	}

}

/*FUNCTION writeLineSplit
Params: int point: signify if this is at the beginning of a round (0), or the end (1)
*/
void writeLineSplit(FILE * file, int point){
	fprintf(file,"************************\n");
	if(point == 0){
		fprintf(file,"New salad making start from here\n");
	}
	else{
		fprintf(file,"\n");
		
	}
}

void writeCounterToLog(FILE * file,char * timestamp,int counter){
	fprintf(file,"%s %d\n",timestamp,counter);
}
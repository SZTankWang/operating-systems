#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "myUtil.h"
#include "info.h"



/*
Function: detach
Description: used by makers to detach from the shared memory
*/
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


/*
Function: timing
Description: take two timeval structs: before and after, and return time elapsed between them
*/
double timing(struct timeval before,struct timeval after, int mkrID){
	struct timeval result;

	timersub(&after, &before, &result);

	double res = (double )result.tv_sec + ((double)result.tv_usec)/1e6;
	// printf("INSIDE timing function FOR MKR %d: time elapsed is %f\n",mkrID, res);
	return res;
};


/*
Function: getTimeStamp
Description: get current timestamp and set it into buffer s
*/
void getTimeStamp(char * s){
	time_t ltime;
	// char * s = (char *) malloc(sizeof(char)*128);

	ltime = time(NULL);
	struct tm * p = localtime(&ltime);
	
	strftime(s,128,"%F %H:%M:%S ",p);

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


/*
Function: writeLogForSalad
Description: record for finishing a salad 
*/
void writeLogForSalad(FILE * file,double time_used,char * timestamp, int saladCnt){

	fprintf(file,"%s: made another salad for %f seconds\n",timestamp, time_used);

}


/*
Function: writeLogForMkrBye
Description: before exiting, write a log info contains total number of salads made by this maker
*/
void writeLogForMkrBye(FILE * file,char * timestamp, int saladCnt){
	fprintf(file,"%s: finished! I made %d salads in total\n",timestamp, saladCnt);

}


/*
function: writeLogForVege
used by salad maker, to put information when a maker receive a salad and finish weighing it
Parameter: fromChef: 0: this ingredient is from makers' own disposals; 1: given by chef
		   int enough: 0: still need more for this vege; 1: already enough \
		   float weight: weight placed on the bench by chef this round
		   float total_received: shm[mkrID+1].ingresin the process for this salad, the total weight in the disposal for this vege 
			
*/
void writeLogForVege(FILE * file,int fromChef,char * timestamp, int enough,float weight,float total_received,char * ingre_name ){
	
	if(enough == 0){
		if(fromChef == 1){

			fprintf(file,"%s:  ingredient %s given by chef is not enough! JUST receive from bench: %f grams. I receive %f of %s in total\n",\
			timestamp,ingre_name,weight,total_received, ingre_name);

		}
		else{

			fprintf(file,"%s:  ingredient %s picked by myself is not enough! JUST receive from bench: %f grams. I receive %f of %s in total\n",\
			timestamp,ingre_name,weight,total_received, ingre_name);

		}

	}else{

		if(fromChef==1){

			fprintf(file, "%s: ingredient %s given by chef is enough! Just received another %f grams. I got %f grams at my disposal! \n",timestamp,ingre_name,weight,total_received);
	
		}
		else{

			fprintf(file, "%s: ingredient %s picked by myself is enough! Just received another %f grams. I got %f grams at my disposal! \n",timestamp,ingre_name,weight,total_received);

		}
	}

}




/*
Function:writeLogForIngreReport
Description: write a log to file when a maker finish with receiving ingredients, that is all of them are enough
*/
void writeLogForIngreReport(FILE * file, char * timestamp, char ingres[3][50], float * disposal){
	fprintf(file,"%s: finishes with receiving ingredients.\n", timestamp);
	for(int i=0;i<3;i++){
		fprintf(file,"%s at disposal %f grams.\n",ingres[i],disposal[i]);
	}

}

/*
FUNCTION writeLineSplit
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


/*
Function:writeCounterToLog
Description: write a log to the public log file, record current number of busy workers 
*/
void writeCounterToLog(FILE * file,char * timestamp,int prev, int counter, int mkrID){
	fprintf(file,"%s | MKR %d edit %d -> | %d\n",timestamp,mkrID,prev,counter);
}



/*
Function: parseSharedLog
Description: read the public log file, and record timestamps and number of workers busy
at each timestamp in two seperate arrays. This is for the parrallel period computing function.

*/
int parseSharedLog(char * line,char * delim, int index, char ** timeStorage, int * counterStorage ){
	//write new info to allocated array

	//get first token
	char * token;
	int cnt = 0;

	token = strtok(line,delim);
	strcpy(timeStorage[index],token);
	cnt ++;

	//walk through the other token
	while(token != NULL){
		// printf("%s\n",token);
		token = strtok(NULL,delim);
		cnt ++;
		if(cnt == 3){
			//write counter info

			counterStorage[index] = atoi(token);
		}
	}

	return 0;
}



/*
FUNCTION: openAndReadLog
Description: used by chef to output each maker's work log

*/
void openAndReadLog(FILE * file){
	size_t len=0;
	ssize_t nread;
	char * line = NULL;

	while((nread = getline(&line,&len,file))!= -1){
		printf("%s",line);
	}

	fclose(file);

}

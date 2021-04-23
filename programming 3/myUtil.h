#ifndef MYUTIL_H_
#define MYUTIL_H_

#include "info.h"
#include <time.h> 	
#include <stdio.h>

int detach(INFO * shm, int mkrID);

double timing(struct timeval before,struct timeval after, int mkrID);

void getTimeStamp(char * s);

void writeLogForWait(FILE * file,double time_used,char * timestamp, int origin);

void writeLogForSalad(FILE * file,double time_used,char * timestamp, int saladCnt);

void writeLogForMkrBye(FILE * file,char * timestamp, int saladCnt);

void writeLogForVege(FILE * file,char * timestamp, int enough,float weight,float total_received,char * ingre_name );

void writeLogForIngreReport(FILE * file, char * timestamp, char ingres[3][50], float * disposal);

void writeLineSplit(FILE * file, int point);

void writeCounterToLog(FILE * file,char * timestamp,int counter);

#endif 
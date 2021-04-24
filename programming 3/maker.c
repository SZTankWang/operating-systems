#include "info.h"
#include "myUtil.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/time.h>
#include <stddef.h>

// double timing(struct timeval before,struct timeval after, int mkrID);

int main(int argc,char ** argv){
	int mkrTime;
	int mkrID;
	int shmid;
	int count=0;
	int prev; //to store previous value of counter variable

	double time_spent_waiting=0.;
	double time_elapsed;
	double time_spent_making=0.;
	double t1,t2;

	char timestamp[128]; 

	struct timeval before,after,result; //for function gettimeofday


	int ingre_used[3];

	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-m")==0){
			mkrTime = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-s")==0){
			shmid = atoi(argv[i+1]);
		}
		
		if(strcmp(argv[i],"-o")==0){
			mkrID = atoi(argv[i+1]);
		}	
	}


	INFO * shm;
	shm = shmat(shmid,NULL,0);
	// printf("number of salad %d\n",shm[0].salads_to_go);

	// printf("MKR %d: my file path is: %s\n",mkrID,shm[mkrID+1].filepath);

	//open log file
	FILE * file;
	file = fopen(shm[mkrID+1].filepath,"w");

	//open public log file
	FILE * share;
	

	//seeding random number

	int salad_count=0;

	// printf("MKR %d: the ingredients I have: %s\n",mkrID,shm[mkrID+1].ingres[2]);
	// printf("MKR %d: the ingredients I need: %s and %s\n",mkrID,shm[mkrID+1].ingres[0],shm[mkrID+1].ingres[1]);
	//sequence: wait for chef -> check salad_to_make -> get ingredients -> weight -> make ->return to wait chef
	

	//initialize t1 before entering
	gettimeofday(&before,NULL);
	while(1){
		sem_wait(&shm[mkrID+1].pick_lock);
		
		//write a split line to logs
		writeLineSplit(file,0);

		//end waiting, get elapsed time

		gettimeofday(&after,NULL);

		time_elapsed = timing(before,after,mkrID);
		time_spent_waiting += time_elapsed;

		//get timestamp
		getTimeStamp(timestamp);
		// printf("MKR %d: (LINE 67) ANOTHER WAITING FOR CHEF FOR %f seconds\n",mkrID,time_elapsed);

		//write log 
		writeLogForWait(file,time_elapsed,timestamp,0);

		//this while loop is for continuously request chef for ingredients within one \
		round of making a salad
		//because the weights might not be enough

		float disposal[3] = {0,0,0}; // the total weight of each of the two ingredient\
							provided by chef that is available to the maker so far

		int from_waiting=0;
		while(1){

			if(from_waiting==1){
				//if the maker has come from previous round of waiting for ingredient
				//end waiting, get elapsed time

				gettimeofday(&after,NULL);

				//get time stamp				
				getTimeStamp(timestamp);
				
				time_elapsed = timing(before,after,mkrID);
				time_spent_waiting += time_elapsed;
				

				//write to log
				writeLogForWait(file,time_elapsed,timestamp,1);
			}
			
			//check salad_to_make
			sem_wait(&shm[0].salad_lock);
			
			if(shm[0].salads_to_go == 0){

				sem_post(&shm[0].salad_lock);

				//record time stats
				shm[mkrID+1].time_spent_making = time_spent_making;
				shm[mkrID+1].time_spent_waiting = time_spent_waiting;
				

				//detach shared memory
				int det_result = detach(shm,mkrID);
				
				//write log file
				getTimeStamp(timestamp);
				writeLogForMkrBye(file,timestamp,count);

				//close log file 
				return 0;
			}
			sem_post(&shm[0].salad_lock);
				
			

			//run a for loop to check if any of the ingredients is still needed
			sem_wait(&shm[mkrID+1].in_need_lock);
			for(int j=0;j<2;j++){
				
				float newly_placed = shm[mkrID+1].bench[j]; 

				//clear bench
				shm[mkrID+1].bench[j] = 0;

				//get timestamp for logging
				strcpy(timestamp,"");
				getTimeStamp(timestamp);
									

				disposal[j] += newly_placed;


				//check weight in the disposal with the weight requirement, e.g: tomato >= 100 g

				if(disposal[j] < shm[mkrID+1].ingres_ideal[j]){

					shm[mkrID+1].ingres_in_need[j] = 1;

					writeLogForVege(file, timestamp, 0, newly_placed, disposal[j], shm[mkrID+1].ingres[j]);

				}else{

					shm[mkrID+1].ingres_in_need[j] = 0;
					writeLogForVege(file, timestamp, 1, newly_placed, disposal[j], shm[mkrID+1].ingres[j]);

				}


			}

			sem_post(&shm[mkrID+1].in_need_lock);



			//user grab his ingredient from his own disposal, generate randomness
			while(1){

				if(disposal[2] < shm[mkrID+1].ingres_ideal[2]){

					srand(time(NULL));

					float scalar = (float)rand()/(float)(RAND_MAX);
					int ideal = shm[mkrID+1].ingres_ideal[2];
					float weight = 0.8*ideal + scalar*(1.2*ideal - 0.8*ideal); 
					disposal[2] += weight;

				}else{
					
					break; //enough, break
				}


			}

			//run a for loop to check if all the ingredients have been enough
			//flag CONTINUE_TO_WAIT; 0: NO, ENOUGH; 1: YES, NOT ENOUGH YET
			int continue_to_wait = 0;
			for(int i=0;i<2;i++){
				if(shm[mkrID+1].ingres_in_need[i] == 1){
					continue_to_wait = 1;
				}
			}


			//tell chef that maker has got the ingredients
			sem_post(&shm[mkrID+1].mkr_ready);
				

			if(continue_to_wait == 0){

				//get timestamp for logging
				getTimeStamp(timestamp);

				writeLogForIngreReport(file, timestamp,shm[mkrID+1].ingres,disposal);
				break;
			}

			//else, wait for chef

			printf("MKR %d wait for chef! \n",mkrID);	

			from_waiting = 1;//set from waiting flag in order for time counting next round
			
			//reset timer
			gettimeofday(&before,NULL);		
			


			sem_wait(&shm[mkrID+1].pick_lock);
		}	





		//generate randomness for making salad time
		float scalar = (float)rand()/(float)(RAND_MAX);
		float time_per_salad = 0.8*mkrTime+ scalar*(mkrTime-0.8*mkrTime);

		printf("MKR %d: start making salad, will take %f seconds \n",mkrID, time_per_salad );

		//Record ingredient used 
		for(int j=0;j<3;j++){
			shm[mkrID+1].ingres_used[j] += disposal[j];
			
		}



		time_per_salad *= 1e6;

		//set timer
		gettimeofday(&before,NULL);
		
		//edit shared variable counter, increase num of mkr busy by 1
		sem_wait(&shm[0].counter_lock);
		prev = shm[0].counter;
				
		shm[0].counter ++;
		
		//open public log
		share = fopen(shm[mkrID+1].sharedLog,"a");
		getTimeStamp(timestamp);
		writeCounterToLog(share,\
			timestamp,prev, shm[0].counter,mkrID);

		//close log file
		fclose(share);
		sem_post(&shm[0].counter_lock);
		

		usleep(time_per_salad);
		gettimeofday(&after,NULL);
		getTimeStamp(timestamp);

		double salad_time = timing(before,after,mkrID);


		
		count++;

		//write to log file
		writeLogForSalad(file, salad_time,timestamp,count);

		//decrease num of mkr busy by 1
		sem_wait(&shm[0].counter_lock);
		if(shm[0].counter == 0){
			printf("error occurs on shared vairable COUNTER! \n");
		}
		prev = shm[0].counter;
		shm[0].counter --;

		//open public log file
		share = fopen(shm[mkrID+1].sharedLog,"a");

		getTimeStamp(timestamp);
		writeCounterToLog(share,\
			timestamp, prev,shm[0].counter,mkrID);

		//close log file

		fclose(share);

		sem_post(&shm[0].counter_lock);
		



		//increase time_spent for salad

		time_spent_making += salad_time;
		
		//start timing for waiting time
		gettimeofday(&before,NULL);
					
		//write splitting line to logging file

		writeLineSplit(file,1);
	}


	return 0;

}



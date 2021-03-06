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
#include <errno.h>


int main(int argc,char ** argv){
	double mkrTime;
	int mkrID;
	int shmid;
	int count=0;
	int prev; //to store previous value of counter variable

	int weight; // this is the average weight of the\
					ingredient that this maker has at his disposal\
					so that he knows how much to pick each grab

	double time_spent_waiting=0.;//initialize counter for total time spent waiting for chef
	double time_elapsed; //this one is used to calculate elapsed time for each wait 
	double time_spent_making=0.; // counter for total time spent making salad

	char timestamp[128]; //buffer for holding timestamp

	struct timeval before,after,result; //for function gettimeofday


	int ingre_used[3]; //array to check total ingredients used each kind


	//read tty arguments
	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-m")==0){
			mkrTime = atof(argv[i+1]);
		}
		if(strcmp(argv[i],"-s")==0){
			shmid = atoi(argv[i+1]);

		}
		
		if(strcmp(argv[i],"-o")==0){
			mkrID = atoi(argv[i+1]);
		}	

		if(strcmp(argv[i],"-w")==0){
			weight = atoi(argv[i+1]);
		}	

	}


	//attach to shared memory
	INFO * shm;
	shm = shmat(shmid,NULL,0);

	//handle attach failure! 
	if(shm == (void*)-1){
		printf("error occurs for MKR %d when attaching to shared memory\n",mkrID);
		printf("error is error(%d%s)\n",errno,strerror(errno));
	}

	//here'are some debugging code

	// printf("MKR %d: shmid is %d the average weight for %s is %d grams\n",mkrID,shmid,shm[mkrID+1].ingres[2]\
	// 	,weight);

	// printf("number of salad %d\n",shm[0].salads_to_go);

	// printf("MKR %d: my file path is: %s\n",mkrID,shm[mkrID+1].filepath);
	//

	//open log file
	FILE * file;
	file = fopen(shm[mkrID+1].filepath,"w");

	//open public log file
	FILE * share;
	

	//seeding random number

	int salad_count=0;


	//workflow sequence: wait for chef's ingredient -> check salad_to_make\
	 -> get ingredients -> get weight -> grab from own disposal -> \
	 return to wait ingredient if still needed\
	  -> make salad ->return to wait chef's ingredient
	

	//initialize time in before
	gettimeofday(&before,NULL);
	while(1){
		printf("MKR %d is waiting for chef\n",mkrID);

		//wait on chef's coordination for picking up ingredients (quit idle state)
		sem_wait(&shm[mkrID+1].pick_lock);
		
		//write a split line to logs
		writeLineSplit(file,0);

		//end waiting, get elapsed time, and add to total waiting time

		gettimeofday(&after,NULL);

		time_elapsed = timing(before,after,mkrID);
		time_spent_waiting += time_elapsed;

		//get current timestamp
		getTimeStamp(timestamp);
		// printf("MKR %d: (LINE 67) ANOTHER WAITING FOR CHEF FOR %f seconds\n",mkrID,time_elapsed);

		//write log for another waiting for chef event 
		writeLogForWait(file,time_elapsed,timestamp,0);

		//the following while loop is for continuously requesting chef for ingredients within one \
		round of making a salad
		//because the weights might not be enough

		float disposal[3] = {0,0,0}; // the total weight of each of the ingredients\
							that is available to the maker so far, refreshed every salad round

		int from_waiting=0; //this flag is used for determining if we need to compute\
							another waiting time after entering the loop, because once the maker\
							has got the ingredients, they might not be enough, so that still have to\
							wait
		while(1){

			if(from_waiting==1){
				//if the maker has come from previous round of waiting for ingredient
				//end waiting, get elapsed time again

				gettimeofday(&after,NULL);

				//get time stamp				
				getTimeStamp(timestamp);
				
				time_elapsed = timing(before,after,mkrID);
				time_spent_waiting += time_elapsed;
				

				//write to log
				writeLogForWait(file,time_elapsed,timestamp,1);
			}
			
			//check salad_to_make
			printf("MKR %d is waiting for salad lock\n",mkrID);
			sem_wait(&shm[0].salad_lock); //request salad_lock before accessing shared variable\
											salads_to_go
			
			if(shm[0].salads_to_go == 0){

				//this means that the maximal salad number has been met, so maker can leave
				sem_post(&shm[0].salad_lock);

				//record time stats
				shm[mkrID+1].time_spent_making = time_spent_making;
				shm[mkrID+1].time_spent_waiting = time_spent_waiting;
				

				//detach shared memory
				int det_result = detach(shm,mkrID);
				
				//write log file to record a maker's leave
				getTimeStamp(timestamp);
				writeLogForMkrBye(file,timestamp,count);

				//close log file 
				fclose(file);

				return 0;
			}
			sem_post(&shm[0].salad_lock);
			// printf("MKR %d has finished with salad lock\n",mkrID);
				
			

			//run a for loop to check if any of the ingredients is still needed
			sem_wait(&shm[mkrID+1].in_need_lock);
			for(int j=0;j<2;j++){
				
				//read from bench array to get newly place ingredient info from chef				
				float newly_placed = shm[mkrID+1].bench[j]; 

				//clear bench
				shm[mkrID+1].bench[j] = 0; //need to clear the bench

				//get timestamp for logging
				strcpy(timestamp,"");
				getTimeStamp(timestamp);
									

				disposal[j] += newly_placed; //add the bench item to disposal


				//check weight in the disposal with the weight requirement, e.g: tomato >= 100 g

				if(disposal[j] < shm[mkrID+1].ingres_ideal[j]){

					shm[mkrID+1].ingres_in_need[j] = 1; //this ingredient is not enough

					writeLogForVege(file,1, timestamp, 0, newly_placed, disposal[j], shm[mkrID+1].ingres[j]);

				}else{

					shm[mkrID+1].ingres_in_need[j] = 0; //this one is enough
					writeLogForVege(file, 1,timestamp, 1, newly_placed, disposal[j], shm[mkrID+1].ingres[j]);

				}


			}

			sem_post(&shm[mkrID+1].in_need_lock);



			//user grab his ingredient from his own disposal, generate randomness
			//the whole thing works similar with getting items from chef, but does not have
			//to wait
			float picked;
			srand(time(NULL));
			while(1){

				if(disposal[2] < shm[mkrID+1].ingres_ideal[2]){

					

					float scalar = (float)rand()/(float)(RAND_MAX);
					// int ideal = shm[mkrID+1].ingres_ideal[2];
					picked = 0.8 * weight + scalar*(1.2*weight - 0.8*weight); 
					disposal[2] += picked;

					getTimeStamp(timestamp);
					writeLogForVege(file,0,timestamp,0,picked,disposal[2],shm[mkrID+1].ingres[2]);				

				}else{
					writeLogForVege(file,0,timestamp,1,picked,disposal[2],shm[mkrID+1].ingres[2]);				
					
					break; //enough, break
				}


			}

			//run a for loop to check if all the ingredients have been enough
			//flag CONTINUE_TO_WAIT; 0: NO, ENOUGH; 1: YES, NOT ENOUGH YET
			int continue_to_wait = 0;
			for(int i=0;i<2;i++){
				if(shm[mkrID+1].ingres_in_need[i] == 1){
					continue_to_wait = 1; //then the maker has to wait for ingredients for another round
				}
			}


			//tell chef that maker has picked up the ingredients
			sem_post(&shm[mkrID+1].mkr_ready);
				
			//if all ingredients are enough, break the waiting loop, start to make! 
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
			

			//begin to wait for chef's ingredients again
			sem_wait(&shm[mkrID+1].pick_lock);
		}	





		//generate randomness for making salad time
		float scalar = (float)rand()/(float)(RAND_MAX);
		double time_per_salad = 0.8*mkrTime+ scalar*(mkrTime-0.8*mkrTime);

		printf("MKR %d: start making salad, will take %f seconds \n",mkrID, time_per_salad );

		//Record ingredient used 
		for(int j=0;j<3;j++){
			shm[mkrID+1].ingres_used[j] += disposal[j];
			
		}



		time_per_salad *= 1e6; //I'm using usleep(), which takes argument in millisecond

		//set timer
		gettimeofday(&before,NULL);
		
		//edit shared variable counter, increase num of mkr busy by 1
		sem_wait(&shm[0].counter_lock);
		prev = shm[0].counter;
				
		shm[0].counter ++;
		
		//open public log
		share = fopen(shm[mkrID+1].sharedLog,"a");
		getTimeStamp(timestamp);

		//record in public log current number of busy workers
		writeCounterToLog(share,\
			timestamp,prev, shm[0].counter,mkrID);

		//close log file
		fclose(share);

		//release the public file lock
		sem_post(&shm[0].counter_lock);
		

		//sleep, making salad
		usleep(time_per_salad);


		gettimeofday(&after,NULL);
		getTimeStamp(timestamp);

		//get time spent on making this salad
		double salad_time = timing(before,after,mkrID);


		
		count++; //keep track of salad made for his own sake

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

		//one full salad-making process ends
	}


	return 0;

}



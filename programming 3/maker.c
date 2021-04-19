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
#include <stddef.h>



int main(int argc,char ** argv){
	int mkrTime;
	int mkrID;
	int shmid;
	int count=0;

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

	// printf("MKR %d\n",mkrID);

	INFO * shm;
	shm = shmat(shmid,NULL,0);
	// printf("number of salad %d\n",shm[0].salads_to_go);

	//seeding random number

	int salad_count=0;

	printf("MKR %d: the ingredients I have: %s\n",mkrID,shm[mkrID+1].ingres[2]);
	printf("MKR %d: the ingredients I need: %s and %s\n",mkrID,shm[mkrID+1].ingres[0],shm[mkrID+1].ingres[1]);
	//sequence: wait for chef -> check salad_to_make -> get ingredients -> weight -> make ->return to wait chef
	while(1){

		//wait for chef's ingredients
		printf("MKR %d wait for chef! \n",mkrID);
		sem_wait(&shm[mkrID+1].mkr_lock);

		//check salad_to_make
		sem_wait(&shm[0].salad_lock);
		
		if(shm[0].salads_to_go == 0){
			printf("finished! maker %d made %d salads,  exits.\n",mkrID,count);

			sem_post(&shm[0].salad_lock);

			//detach shared memory
			int det_result = detach(shm,mkrID);
			return 0;
		}
		sem_post(&shm[0].salad_lock);
			
		
		//else, get ingredients, know the weight now
		float newly_used[3];
		newly_used[0]= shm[mkrID+1].bench[0];


		newly_used[1] = shm[mkrID+1].bench[1];

		//tell chef that maker has got the ingredients
		sem_post(&shm[mkrID+1].mkr_ready);

		//user grab his ingredient, generate randomness
		srand(time(NULL));

		float scalar = (float)rand()/(float)(RAND_MAX);
		int ideal = shm[mkrID+1].ingres_ideal[2];
		float weight = 0.8*ideal + scalar*(1.2*ideal - 0.8*ideal); 

		newly_used[2] = weight;

		printf("MKR %d: I used %s for %f, %s for %f, %s for %f\n", mkrID,shm[mkrID+1].ingres[0],\
			newly_used[0],shm[mkrID+1].ingres[1],newly_used[1],\
			shm[mkrID+1].ingres[2],newly_used[2] );
		//record statistic in ingres_used array
		for(int i=0;i<3;i++){
			shm[mkrID+1].ingres_used[i] += newly_used[i];
		}


		//generate randomness for making salad time
		

		scalar = (float)rand()/(float)(RAND_MAX);

		printf("MKR %d: start making salad, will take %f seconds \n",mkrID, 0.8*mkrTime+ scalar*(mkrTime-0.8*mkrTime));

		sleep(0.8*mkrTime+ scalar*(mkrTime-0.8*mkrTime));

		count++;
		printf("MKR %d finish with one salad ...\n",mkrID);

		//check by themselves the salad_to_go value again
		// sem_wait(&shm[0].salad_lock);
		
		// if(shm[0].salads_to_go == 0){
		// 	printf("finished! maker %d exits.\n",mkrID);
		// 	sem_post(&shm[0].salad_lock);
		// 	return 0;
		// }
		// sem_post(&shm[0].salad_lock);
		
	}


	return 0;

}


//make salad code
// float scalar = (float)rand()/(float)(RAND_MAX);
// printf("scalar is %f\n",scalar);

// printf("MKR %d: start making salad, will take %f seconds \n",mkrID, 0.8*mkrTime+ scalar*(mkrTime-0.8*mkrTime));

// sleep(0.8*mkrTime+ scalar*(mkrTime-0.8*mkrTime));
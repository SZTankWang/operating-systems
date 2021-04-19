#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "info.h"

#define SHMSIZE 1024



int main(int argc, char ** argv){

	int saladNum;
	float chefTIme;

	//array of ingredient info. mkr1: (1,3); mkr2: (1,2); mkr3: (2,3)
	int arr[9] = {1,1,2,3,2,3,2,3,1};

	char * ingre_seq[3];
	ingre_seq[0] = "tomato";
	ingre_seq[1] = "onion";
	ingre_seq[2] = "green_pepper";

	//ideal usage of ingredients per salad
	int ideal_usage[3] = {100,80,60};
	//this is out of order one, for each user
	int ideal_usage_alt[3] = {80,60,100};

	
	//this is for the ingredients that each mkr always has
	char * ingredients[3];
	ingredients[0] = "onion";
	ingredients[1] = "green_pepper";
	ingredients[2] = "tomato";




	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-n")==0){
			saladNum = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-m")==0){
			chefTIme = atof(argv[i+1]);			
		}
	}

	//initialize shared memory
    int shmid;
    key_t key;
    INFO * shm;

    key = 9999;
    shmid = shmget(key,SHMSIZE,IPC_CREAT | 0666);

	//check for faiure
    if (shmid < 0)
    {
        perror("shmget failure");
        exit(1);
    }

    //attach shared memory
    shm = (INFO *)shmat(shmid,NULL,0);

    //STORE AN INFO Struct in the shm
    INFO info;
    shm[0] = info;
    shm[0].salads_to_go = saladNum;

    
    //declare semaphores
    int sem_status;
    sem_status = sem_init(&shm[0].salad_lock,1,1); //salad number lock


    if(sem_status == -1){
    	perror("semaphore init failure");
    	exit(-1);
    }


    //declare semaphores for each maker
    for(int i=0;i<3;i++){
    	INFO new_info;
    	shm[i+1] = new_info;
    	sem_status = sem_init(&shm[i+1].mkr_lock,1,0);

     
        if(sem_status == -1){
	    	perror("semaphore init failure");
	    	exit(-1);


    	}

    	sem_status = sem_init(&shm[i+1].mkr_ready,1,0);
       
        if(sem_status == -1){
	    	perror("semaphore init failure");
	    	exit(-1);


    	}

    	//initialize ingredient info
    	for(int j=0;j<3;j++){
	    	memcpy(shm[i+1].ingres[j], ingre_seq[arr[i+3*j]-1],32);	

    	}


    	//tell each user the standard usage of the one material that they always have
    	shm[i+1].ingres_ideal[2] = ideal_usage_alt[i];

    	//initialize ingres_used statistic
    	for(int j=0;j<3;j++){
    		shm[i+1].ingres_used[j] = 0;
    	}

    }
    


    //convert arguments for makers to string
	char shmid_str[50];
	char mkr_time[50];

	sprintf(shmid_str,"%d",shmid);
	sprintf(mkr_time,"%d",5);



    //fork maker from here
    for(int i=0;i<3;i++){
    	int pid = fork();
    	if(pid == 0){
    		char mkrID[2];
    		sprintf(mkrID,"%d",i);
			execlp("./maker","maker","-m",mkr_time,"-s",shmid_str,"-o",mkrID,NULL);

    	}
    }

    //chef distribution logic
    int choice;
    float scalar,rest;
    while(1){
    	printf("chef starts new round ... \n");
    	//check if already made enough salad
    	sem_wait(&shm[0].salad_lock);
    	
    	if(shm[0].salads_to_go == 0){
    		printf("chef message : done with all the salad\n");
    		//do a signal on every salad mkr semaphore to let them go
    		for(int i=0;i<3;i++){
    			sem_post(&shm[i+1].mkr_lock);//this is to release any maker that might be waiting for the chef
    		}
    		sem_post(&shm[0].salad_lock);
    		break;

    	}
    	sem_post(&shm[0].salad_lock);

    	//randomly pick a maker
		srand(time(NULL));

		choice = rand()%3;

		printf("CHEF: The choice is MKR %d\n", choice);

		//pick ingredients and put at the chosen maker's bench
		int ideal;
		for(int i=0;i<2;i++){
			scalar = (float)rand()/(float)(RAND_MAX);

			ideal = ideal_usage[arr[choice + i*3]-1];
			
			shm[choice+1].bench[i] = 0.8*ideal + scalar*(1.2*ideal - 0.8*ideal); 			
		}

		//signal maker to pick up ingredients
		sem_post(&shm[choice+1].mkr_lock);


		//then wait for user to get ready
		printf("chef waiting for MKR %d to pick ingredients\n",choice);
		sem_wait(&shm[choice+1].mkr_ready);
		printf("MKR %d has got the ingredients\n",choice );

		//decrease salads_to_go value
		sem_wait(&shm[0].salad_lock);
		shm[0].salads_to_go --;
		sem_post(&shm[0].salad_lock);

		//rest for some time
		rest = 0.5*chefTIme + scalar*(chefTIme - 0.5*chefTIme);
		printf("chef rest for %f s ...\n",rest);
		sleep(rest);
    }


    int status;
    while((wait(&status))!=-1 );

    //chef read maker statistic: 
    int used_counts[3] = {0,0,0};

    for(int i=0;i<3;i++){
    	//access mkr statistic by index
    	INFO stat = shm[i+1];
    	for(int j=0;j<3;j++){
    		//determine which type of vege?
    		int type = arr[i+j*3];
    		//check in case
    		printf("MKR %d used %s for %f grams\n",i,ingre_seq[type-1], stat.ingres_used[j]);
    		// used_counts[type-1] += stat.ingres_used[j];
    	}

    }



    shmdt(shm); 

    return 0;

}
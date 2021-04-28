#include "info.h"
#include "myUtil.h"
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


#define SHMSIZE 2048



int main(int argc, char ** argv){

	int saladNum; //to be put into shm, to indicate how many salads is left to make
	int salad_made = 0; //local variable, used by chef to track number of salads made
	float chefTIme; //time for chef to rest at maximal
	float mkrTime; //time for mkr to make salad at maximal

	//array of ingredient info. mkr1: (1,3,2); mkr2: (1,2,3); mkr3: (2,3,1)
	int arr[9] = {1,1,2,3,2,3,2,3,1};

	char * ingre_seq[3];
	ingre_seq[0] = "tomato";//tomato: 1
	ingre_seq[1] = "onion"; //onion: 2
	ingre_seq[2] = "green_pepper"; //pepper:3

	int ideal_usage[3] = {100,60,80}; 	//minimal weight of each ingredient at disposal per salad
	int per_vege_weight[3] = {80,30,50}; //the weight of each single ingredient


	//reading command line arguments
	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-n")==0){
			saladNum = atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-m")==0){
			chefTIme = atof(argv[i+1]);			
		}
		if(strcmp(argv[i],"-c")==0){
			mkrTime = atof(argv[i+1]);			
		}

	}

	char * lineSplit = "********************\n";

	//initialize shared memory
    int shmid;
    key_t key;
    INFO * shm;

    key = 9999;
    shmid = shmget(key,SHMSIZE,IPC_CREAT | 0666); //create shared memory

	//check for faiure
    if (shmid < 0)
    {
        perror("shmget failure");
        exit(1);
    }

    //attach shared memory
    shm = (INFO *)shmat(shmid,NULL,0); //chef attach to shared memory

    
    //doing some initalization start from here 
    //************************
    //STORE AN INFO Struct in the shm
    INFO info;
    shm[0] = info;
    shm[0].salads_to_go = saladNum; //salads_to_go is to show how many salads still need to be made
    shm[0].counter = 0; //counter is used to show how many makers are still busy making salads at a timestamp


    //try remove the public logging file, if it exists
    if( (remove("public")) == 0){
    	printf("shard logging file is succssefully deleted\n");
    }else{
    	printf("error. Maybe the file is not there\n");
    }

    char mkr[4] = "MKR"; //string used for makers' logging files
    
    //declare semaphores
    int sem_status;
    sem_status = sem_init(&shm[0].salad_lock,1,1); //salad number lock, used for shared variable <salads_to_go> at shm[0]


    if(sem_status == -1){
    	perror("semaphore init failure");
    	exit(-1);
    }

	sem_status = sem_init(&shm[0].counter_lock,1,1); //salad number lock, used for shared variable <counter> at shm[0]


    if(sem_status == -1){
    	perror("semaphore init failure");
    	exit(-1);
    }




    //declare semaphores for each maker
    for(int i=0;i<3;i++){
    	INFO new_info;
    	shm[i+1] = new_info;


    	sem_status = sem_init(&shm[i+1].pick_lock,1,0); //pick lock is used to tell a maker that he need to pick up ingredient
       
        if(sem_status == -1){
	    	perror("semaphore init failure");
	    	exit(-1);

    	}

		//mkr_ready lock is used to tell the chef that	the maker has got the ingredients
       
    	sem_status = sem_init(&shm[i+1].mkr_ready,1,0); 
        if(sem_status == -1){
	    	perror("semaphore init failure");
	    	exit(-1);


    	}


		//in_need lock is used when either chef or maker\
		//wants to access the ingres_in_need array at shm[mkrID+1]\
		//because chef would need to check this array to tell which ingredient\
		//the maker still needs (that is not enough), and the maker needs to modify\
		//this array to tell chef his situation (which is still needed and which is not)\
		//I believe this semaphore is only needed just in case.
    	sem_status = sem_init(&shm[i+1].in_need_lock,1,1);
       				
        if(sem_status == -1){
	    	perror("semaphore init failure");
	    	exit(-1);

    	}

    	


    	//initialize ingredient info

    	//copying the three ingredients name for each maker, in a sequence of \
    	ingre1 which they don't have, ingre2 which they don't have, and ingre3 which they have \
    	at their own disposal
    	for(int j=0;j<3;j++){
	    	memcpy(shm[i+1].ingres[j], ingre_seq[arr[i+3*j]-1],32);	

    	}

    	//tell the maker the least amount of each ingredient they need to have\
    	at their own disposal. For example, a maker has to have at least 100 grams of \
    	tomatoes before starting making a salad
    	for(int j=0;j<3;j++){
    		int type = arr[i+j*3];
    		shm[i+1].ingres_ideal[j] = ideal_usage[type-1];
    		printf("CHEF: for mkr %d, the ideal usage for %s is %d grams\n ",i,shm[i+1].ingres[j],\
    			shm[i+1].ingres_ideal[j]);
    	}

		//initalize the ingres_in_need arr\
		at first, both of the two ingredients that a maker does not have\
		need to be set to 1, so that the chef knows to grab which ingredient in question.
		for(int j=0;j<2;j++){
			shm[i+1].ingres_in_need[j] = 1;
		}

    	//initialize ingres_used statistic
    	for(int j=0;j<3;j++){
    		shm[i+1].ingres_used[j] = 0;
    	}

    	//initialize logging file name for mkr
    	char index[2] ;
    	sprintf(index,"%d",i);

    	//initialize empty filepath\
    	the name of the file is "MKR<i>"

    	strcpy(shm[i+1].filepath,"");
    	strcat(shm[i+1].filepath,mkr);
		strcat(shm[i+1].filepath,index);

		//initialize shared log file path\
		the name of the public log file is 'public'\
		this file is used to take down the num of maker busy at a given timestamp (whenever the value \
		of counter changes)
		strcpy(shm[i+1].sharedLog,"");
    	strcat(shm[i+1].sharedLog,"public"); 
    }
    


    //convert arguments for makers to string
	




    //fork maker from here\
    -m: maximal time taken for one salad\
    -s: the id of shared memory created by chef in its string form\
    -o the ID of this salad mkr\
	-w: the average single weight of the type of ingredient each mkr has at their \
	own disposal
    for(int i=0;i<3;i++){
    	int pid = fork();
    	if(pid == 0){
    		char mkrID[2];
    		sprintf(mkrID,"%d",i);

    		//we also want to send the single weight of the ingredient\
    		that each mkr has at their own disposal, so that they know\
    		how much to pick for themselves. For example, tomato: 80g\
			and this ingredient is specfied as type at index: arr[2+ 3*i]-1

			int weight_to_know = per_vege_weight[arr[2+3*i]-1];
			printf("AFTER FORK: weight_to_know for maker %d is %d\n",i,weight_to_know);
			char weight[4]= {'\0'};
    		sprintf(weight,"%d",weight_to_know);

    		char shmid_str[50] = "";    		
			sprintf(shmid_str,"%d",shmid);
		
			char mkr_time[50] ="";	
			sprintf(mkr_time,"%f",mkrTime);
    	
    		printf("I am %d -th maker, shmid is %s while shmid (int) is %d\n",i,shmid_str,shmid);
			execlp("./maker","maker","-m",mkr_time,"-s",shmid_str,"-o",mkrID,"-w",weight,NULL);

    	}

    }



    //chef distribution logic
    int choice;
    float scalar,rest;
    while(1){
    	printf("%s%s\n",lineSplit,lineSplit );
    	printf("chef starts new round ... \n");

    	//check if already made enough salad\
    	to access the shared variable salads_to_go, \
    	first do a wait on salad_lock

    	sem_wait(&shm[0].salad_lock);
		    	
    	if(shm[0].salads_to_go == 0){
    		printf("chef message : done with all the salad\n");
    		//do a signal on every salad mkr semaphore to let them go
    		for(int i=0;i<3;i++){
    			sem_post(&shm[i+1].pick_lock);//this is to release any maker that might be waiting \
								    			for the chef, and when a maker has finished\
								    			with its previous one, it could directly pass the\
								    			lock for waiting chef to give ingredients and \
								    			check himself the value of salads_to_go\
								    			and finish making-salad loop

    		}
    		sem_post(&shm[0].salad_lock); //finish checking, do a post
    		break; //chef finishes with distribution logic loop

    	}
    	sem_post(&shm[0].salad_lock); //if not finished, also do a post! don't forget

    	//randomly pick a maker
		srand(time(NULL));

		choice = rand()%3; //generate a random number between 0 and 2
		printf("%s",lineSplit);

		printf("CHEF: The choice is MKR %d\n", choice);

		printf("%s",lineSplit);


		//a while loop to consistently pick ingredients that the mkr still needs\
		as long as they are still labelled as needed\
		pick ingredients and put at the chosen maker's bench
		while(1){

			//wait on in_need_lock
			sem_wait(&shm[choice+1].in_need_lock);
			if(shm[choice+1].ingres_in_need[0]==0 && shm[choice+1].ingres_in_need[1]==0){
				//both ingredients are already enough, OK to finish with supplying
				printf("CHEF: MKR %d has got at least required amount of ingredients!\n",choice);
				
				//restore ingres_in_need flags by chef\
				this is for the next time when the maker is picked 
				for(int j=0;j<2;j++){
					shm[choice+1].ingres_in_need[j] = 1;
				}


				sem_post(&shm[choice+1].in_need_lock);
				break; //the mkr has got enough ingredients, the chef can quit \
						the ingredients loop and rest for some time

			}
			else{
				//find out which one is in need,a for loop checking on ingres_in_need
				
				for(int j=0;j<2;j++){
					if(shm[choice+1].ingres_in_need[j]==1){

						//the j-th ingredient maker (choice) used in in need
						int type = arr[choice+3*j];

						scalar = (float)rand()/(float)(RAND_MAX);

						int ideal = per_vege_weight[type-1]; //average weight for one vege
						
						//put onto bench\
						the ingredient grabbed by the chef once is between the range\
						0.8*average ~ 1.2*average

						shm[choice+1].bench[j] = 0.8*ideal + scalar*(1.2*ideal - 0.8*ideal); 	

						//output some message on the tty
						printf("%s",lineSplit);
						printf("INGREDIENT MESSAGE\n");
						printf("CHEF: THIS is for MKR %d, the ingredient is %s, ideal weight %d\n",choice,\
							ingre_seq[type-1],per_vege_weight[type-1]);		
						printf("CHEF putting ingredients for MKR %d %f grams of %s\n",choice,shm[choice+1].bench[j],ingre_seq[type-1]  );
						printf("%s",lineSplit);
					}
				}

			


				sem_post(&shm[choice+1].in_need_lock);

				//notify maker to pick up his veges
				int t;
				t = sem_post(&shm[choice+1].pick_lock);
				if(t == -1){
					printf("error occurs when posting pick lock for MKR %d\n",choice);

				}else{
					printf("the post return 0\n");
				}
				//then wait for user to get ready
				printf("chef waiting for MKR %d to pick ingredients\n",choice);
				sem_wait(&shm[choice+1].mkr_ready);

				
			}	
		}


		//the chef is now out of the distribution loop for a maker
		//decrease salads_to_go value\
		increase salad_made value 
		sem_wait(&shm[0].salad_lock);
		shm[0].salads_to_go --;
		salad_made ++;
		printf("********************\n");
		printf("SALAD TO GO: %d\n", shm[0].salads_to_go);
		printf("********************\n");

		sem_post(&shm[0].salad_lock);



		//rest for some time
		rest = 0.5*chefTIme + scalar*(chefTIme - 0.5*chefTIme);
		printf("chef rest for %f s ...\n\n",rest);

		//use usleep, resolution in millisecond
		rest *= 1e6;
		usleep(rest);
    }


    int status;
    while((wait(&status))!=-1 ); //wait for all maker process to quit


	// now begin with outputting statistics
    printf("\nstart to output statistic information ... \n");

    //output num of salads made
    printf("NUMBER OF SALADS MADE: %d\n",salad_made);

    //chef read maker statistic: 
    int used_counts[3] = {0,0,0};

    for(int i=0;i<3;i++){
    	//access mkr statistic by index
    	printf("\nINFO FOR MKR %d: \n",i);
    	printf("%s",lineSplit);
    	INFO stat = shm[i+1];
    	for(int j=0;j<3;j++){
    		//determine which type of vege?
    		int type = arr[i+j*3];
    		//check in case
    		printf("MKR %d used %s for %f grams\n",i,ingre_seq[type-1], stat.ingres_used[j]);

    	}

    	//total time spent waiting for chef's ingredients
		printf("MKR %d spent %f seconds waiting for chef's ingredients\n",i,shm[i+1].time_spent_waiting);

		//total time spent making salads
		printf("MKR %d spent %f seconds making salads\n",i,shm[i+1].time_spent_making);
 		
		//output a temporal log of each maker
 		printf("READING MKR %d 's work log ...\n",i);

 		FILE * file = fopen(shm[i+1].filepath,"r");

 		openAndReadLog(file);

 		printf("%s",lineSplit);
    	
    }

    printf("%s\n",lineSplit);

    //part to show number of maker busy at a given timestamp when \
    the counter variable get edited


    printf("computing time period that multiple maker were busy...\n");

    //open share file\
    the information is recorded in file <public>

    FILE * share = fopen("public","r");

    if(share == NULL){
    	printf("error opening public log file, exit!\n");
    	exit(-1);
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t nread;
    int line_cnt = 0;
    
    //get line count of the file
    while((nread = getline(&line,&len,share))!=-1){
    	printf("%s",line);
    	line_cnt ++;

    }

    fclose(share);

    //allocate memory to store data
    //timeStorage: hold time stamps
    //counterStorage: hold num of mkr busy at each timestamp
    
    char ** timeStorage = (char **) malloc(sizeof(char*)*line_cnt);
    int * counterStorage = (int*)malloc(sizeof(int)*line_cnt);

    for(int i=0;i<line_cnt;i++){
    	timeStorage[i] = (char*)malloc(sizeof(char)*128);
    }

    //now open file again
    share = fopen("public","r");
    char * delim = "|";
    int index = 0;
    
    while((nread = getline(&line,&len,share))!=-1){
    	parseSharedLog(line,delim,index,timeStorage,counterStorage);
    	index ++;	
    }

    //free line
    free(line);
    printf("%s\n",lineSplit);
    printf("Outputting busy period ...\n");
    
    //perform iteration and searching on counterStorage
    //a time period with multiple mkrs busy starts by finding the first \
    counter value > 1, end with the first counter value == 1
    
    int start = -1;
    int end = -1;
    for(int i=0;i<line_cnt;i++){
    	if(counterStorage[i] > 1 && start == -1){
    		//found start for a new period
    		start = i;
    	}
    	if(counterStorage[i] == 1 && start != -1){
    		end = i;

    		//go and display period on tty
    		printf("time period found from %s to %s\n",timeStorage[start],timeStorage[end]);

    		//reset start and end for another period
    		start = -1;
    		end = -1;
    	}
    }


    //free semaphores
    sem_destroy(&shm[0].salad_lock);
	sem_destroy(&shm[0].counter_lock);

	//run a for loop to destroy individual semaphores of each maker
	for(int i=0;i<3;i++){
		sem_destroy(&shm[i+1].mkr_ready);
		sem_destroy(&shm[i+1].pick_lock);
		sem_destroy(&shm[i+1].in_need_lock);
	}

	printf("semaphores purged!\n");

    shmdt(shm); 

    printf("shared memory detached!\n");
    printf("now removing shared memory\n");

    int remove;
    struct shmid_ds buf;
    remove = shmctl(shmid,IPC_RMID,&buf);
    if(remove == -1){
    	printf("error occurs when deleting shared memory\n");
    }
    else{
    	printf("shared memory purged! \n");
    }

    //free allocated memory

    printf("freeing allocted memories\n");

    for(int i=0;i<line_cnt;i++){
    	free(timeStorage[i]);
    }



    free(timeStorage);
    free(counterStorage);

    printf("allocted memories freed. Chef exits. Bye!\n");
    return 0;

}
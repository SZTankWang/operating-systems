#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
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


}

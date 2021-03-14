#include <iostream>
#include <signal.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include "myfifo.h"
#define errExit(msg) do {perror(msg); exit(EXIT_FAILURE);}while(0)

using namespace std;



void signal_handler_1(int signNo);
void signal_handler_2(int signNo);
void exit_handler(int signNo);//handle CTRL+C

int getMaxFd(myfifo ** fifoList);

void closeFifo(myfifo ** fifoList);

void setFdToSet(myfifo ** fifoList, fd_set * setPtr);





int main(int argc, char ** argv){
    //register signal here
    signal(12,signal_handler_2);
    signal(10,signal_handler_1);
    signal(SIGINT,exit_handler);

	char * inputFile;
	char * outputFile;
	int workerNum;
	int rangeFlag;
	int attrNum;
	char * orderFlag;

    int sig1_cnt = 0;


    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],"-i")==0){
            inputFile = argv[i+1];
        }
        if(strcmp(argv[i],"-k")==0){
            workerNum = atoi(argv[i+1]);
        }
        if(strcmp(argv[i],"-r")==0){
            rangeFlag=0;
        }
        if(strcmp(argv[i],"-a")==0){
           attrNum = atoi(argv[i+1]);
        }
        if(strcmp(argv[i],"-o")==0){
            orderFlag = argv[i+1];
        }
        if(strcmp(argv[i],"-s")==0){
            outputFile = argv[i+1];
        }


    }

    //get number of records(lines)
    FILE * fp = fopen(inputFile,"r");
    if(fp==NULL){
        cout<<"FILE NOT FOUND"<<endl;
        exit(-1);
    }

    char * line =NULL;
    size_t len=0;
    ssize_t nread;
    int line_cnt = 0;

    //this is to get the total number of records to sort
    while((nread=getline(&line,&len,fp))!= -1){
        line_cnt ++;
    }

    free(line);
    fclose(fp);


	//fork coord here
	int id_h1 = fork();
	int status;



	if(id_h1==0){

            //inside coord
	    	cout<<"I'm the coord node"<<endl;
	    	pid_t ppid = getppid();
	    	cout<<"root node is"<<ppid<<endl;

	    	//declare named pipes
	    	myfifo ** pipeList = (myfifo**)malloc(sizeof(myfifo*)*(workerNum+1));


	    	for(int i=0;i<workerNum;i++){
                char * str_num = (char*)malloc(sizeof(char)*4);
                sprintf(str_num,"%d",i);
                mkfifo(str_num,0777);
                //new fifo object
                myfifo * newFifo = new myfifo(str_num,0);
                pipeList[i] = newFifo;
	    	}

            pipeList[workerNum] = NULL;


            //first create merger
            pid_t id_merger = fork();
            if(id_merger == 0){
                //merger code goes here

                cout<<"this is merger"<<endl;
                //use select to wait for reading from fifos
                int watchListLen = workerNum;
                fd_set fds; 

                //declare an array to hold buffer for reading each fifo
                char ** bufferList= (char**)malloc(sizeof(char*)*(workerNum));  
                int * buffer_size_list = (int*)malloc(sizeof(int)*(workerNum*2)); //list to hold size and read byte for each buffer, position 2i:buffer size, 2i+1: written byte 
                //initialize this buffer list
                for(int i=0;i<workerNum;i++){
                    bufferList[i] = (char*)malloc(sizeof(char)*2048);
                    if(bufferList[i]==NULL){
                        errExit("malloc");
                    }
                    buffer_size_list[2*i]=2048;
                    buffer_size_list[2*i+1]=0;
                }


                
                //open fifo here, before creation of sorter/merger

                


                //use poll
                int nfds,num_open_fds;
                struct pollfd *pfds;

                num_open_fds = nfds = workerNum;
                pfds = (struct pollfd *)calloc(nfds,sizeof(struct pollfd));

                if(pfds==NULL){
                    errExit("malloc");
                }

                for(int j=0;j<nfds;j++){
                    char * fifoname = pipeList[j]->pathname;
                    pfds[j].fd = open(fifoname,O_RDONLY|O_NONBLOCK);
                    if(pfds[j].fd == -1){
                        errExit("open");
                    }

                    cout<<"OPENED"<<fifoname<<"on fd"<<pfds[j].fd<<endl;
                    pfds[j].events = POLLIN;

                }
                //now keep calling poll
                while(num_open_fds>0){
                    int ready;

                    cout<<"about to poll()"<<endl;
                    ready = poll(pfds,nfds,-1);
                    
                    if(ready==-1){
                        errExit("poll");

                    }
                    cout<<"ready: "<<ready<<endl;

                    for(int j=0;j<nfds;j++){
                        

                        if(pfds[j].revents !=0){
                            printf("  fd=%d; events: %s%s%s\n", pfds[j].fd,
                               (pfds[j].revents & POLLIN)  ? "POLLIN "  : "",
                               (pfds[j].revents & POLLHUP) ? "POLLHUP " : "",
                               (pfds[j].revents & POLLHUP) ? "POLLNVAL " : "",                            
                               (pfds[j].revents & POLLERR) ? "POLLERR " : "");


                            if(pfds[j].revents & POLLIN){
                                ssize_t s = read(pfds[j].fd,bufferList[j],buffer_size_list[2*j]/2);
                                int total_read = s+buffer_size_list[2*j+1];
                                buffer_size_list[2*j+1] = total_read;
                                if(total_read >= buffer_size_list[2*j]){
                                    char * newBuffer = (char*)realloc(bufferList[j],buffer_size_list[2*j]*2);
                                    bufferList[j] = newBuffer;
                                    buffer_size_list[2*j] = buffer_size_list[2*j]*2;
                                }

                                if(s==-1){
                                    errExit("read");
                                }

                                cout<<"read"<<s<<" bytes"<<endl;

                            }
                            else if(pfds[j].revents & POLLHUP){
                                cout<<"closing fd"<<pfds[j].fd<<endl;
                                if(close(pfds[j].fd) == -1){
                                    errExit("close");
                                }
                                cout<<"line 215: content for fifo"<<j<<"\n"<<bufferList[j]<<endl;
                                num_open_fds--;
                            }

                        }
                    }

                }
                cout<<"all fifos are closed!"<<endl;



                // //finish with reading, close fifo
                // closeFifo(pipeList);

                //free buffer in the end
                for(int i=0;i<workerNum;i++){
                    cout<<bufferList[i]<<endl;
                    }

                cout<<"free buffer now"<<endl;
                for(int i=0;i<workerNum;i++){
                    free(bufferList[i]);
                    }


                //merger send SIGUSR2 to root
                int coord_id = getppid();
                kill(coord_id,12);

            }
            else{
                //coord code goes here
                cout<<"creating sorters..."<<endl;

                int start;
                int offset = line_cnt/workerNum;
                int startIndex;
                int endIndex;
                

                int f;
                for(f=0;f<workerNum;f++){
                    pid_t id_h2 = fork();
                    if(id_h2==0){

                        //inside sorter
                        //compute start and end index for each sorter
                        char * this_fifo = pipeList[f]->pathname; 
                        // cout<<"checking before exec --> pathname of fifo -->"<<this_fifo<<endl;


                        if(f == workerNum-1){
                            startIndex = 1 + (offset*f);
                            offset = line_cnt - (f*offset);
                            // cout<<"offset: "<<offset<<endl;
                            // cout<<"current f value:"<<f<<endl;
                            endIndex = startIndex+offset-1;
                            //convert int to string
                            int this_pid = getpid();
                            // cout<<"last sorter:" <<this_pid<<"start at"<<startIndex<<"end at "<<endIndex<<endl;
                            char char_start[5];
                            char char_end[5];
                            char char_attrNum[5];
                            char char_ppid[5];
                            sprintf(char_start,"%d",startIndex);
                            sprintf(char_end,"%d",endIndex);
                            sprintf(char_attrNum,"%d",attrNum);
                            sprintf(char_ppid,"%d",ppid);

                            // cout<<"trying to exec"<<endl;
                            execlp("./bubble","bubble","-f",inputFile,"-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);
                        }
                        else{
                            int this_pid = getpid();
                            // cout<<"current f value:"<<f<<endl;
                            // cout<<"f * offset is"<<1+offset*f<<endl;
                            startIndex = 1 + offset*f;
                            endIndex = startIndex + offset - 1;
                            // cout<<f<<" sorter:" <<this_pid<<"start at"<<startIndex<<"end at "<<endIndex<<endl;
                            char char_start[5];
                            char char_end[5];
                            char char_attrNum[5];
                            char char_ppid[5];
                            sprintf(char_start,"%d",startIndex);
                            sprintf(char_end,"%d",endIndex);
                            sprintf(char_attrNum,"%d",attrNum);
                            sprintf(char_ppid,"%d",ppid);

                            // cout<<"trying to exec"<<endl;

                            execlp("./bubble","bubble","-f",inputFile,"-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);

                        }
                        
                    }
                }

                //still coord, going to wait for signals
                cout<<"coord is waiting for SIGUSR2..."<<endl;
                while(1){
                    sleep(1);
                }
            }

	}
	else{
        // in the root
        //root is going to wait for coord
	    	cout<<"ROOT WATING FOR SIGUSR1..."<<endl;
            int status;
            waitpid(id_h1,&status,0);
            cout<<"coord is now finished, root terminate"<<endl;
	    }






    return 0;
}


/*
signal handler for SIGUSR1
*/
void signal_handler_1(int signNO){

    signal(10,signal_handler_1);
    cout<<"receive one sigusr1"<<endl;


}

void signal_handler_2(int signNo){
    cout<<"receive SIGUSR2 from merger, coord exit"<<endl;
    exit(0);
}


void exit_handler(int signNo){
    exit(-1);
}

/*
Function:getMaxFd
Descri: find the maximal file decriptor for select
*/
int getMaxFd(myfifo ** fifoList){
    int max = 0;
    int i =0;
    while(fifoList[i] != NULL){
        char * path = fifoList[i]->pathname;
        int new_fd = open(path,O_RDWR);
        fifoList[i]->fd = new_fd;
        if(new_fd > max){
            max = new_fd;
        }

        i++;
    }

    return max;
}

void closeFifo(myfifo ** fifoList){
    int i=0;
    while(fifoList[i] != NULL){
        close(fifoList[i]->fd);
        cout<<"remove a fifo"<<endl;
        remove(fifoList[i]->pathname);
        delete fifoList[i];
        i++;
    }
}


/*
function: setFdToSet
Descri: set a file despcriptor to the watching set of select 
*/
void setFdToSet(myfifo ** fifoList,fd_set * setPtr){
    int i=0;
    while(fifoList[i]!=NULL){

        FD_SET(fifoList[i]->fd,setPtr);

        i++;

    }
    // cout<<"set FD function should be exiting"<<endl;

}
#include <iostream>
#include <signal.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include "myfifo.h"
#include "Taxpayer.h"
#include "utility.h"
#include "algorithm.h"

//define error message 
#define errExit(msg) do {perror(msg); exit(EXIT_FAILURE);}while(0)

using namespace std;

int SIGNAL1=0;//for counting number of SIGUSR1 received


void signal_handler_1(int signNo);
void signal_handler_2(int signNo);
void exit_handler(int signNo);//handle CTRL+C


int main(int argc, char ** argv){
    // root timing stuff goes here
    double main_t1,main_t2,main_cpu_time;
    struct tms main_tb1,main_tb2;
    double main_ticspersec;

    main_ticspersec = (double)sysconf(_SC_CLK_TCK);
    main_t1 = (double)times(&main_tb1);

    //register signal here
    signal(12,signal_handler_2);
    signal(10,signal_handler_1);
    signal(SIGINT,exit_handler);

	char * inputFile;
	char * outputFile;
	int workerNum;
	int rangeFlag=1; //range flag: -r --> rangeflag = 0;else, rangeflag=1
	int attrNum;
	char * orderFlag;

    int sig1_cnt = 0;


    //handling command line argument
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

    //this is to get the total number of records to sort, since we need to determine 
    //the sorting range for each sorter
    while((nread=getline(&line,&len,fp))!= -1){
        line_cnt ++;
    }

    free(line);
    fclose(fp);


	//fork coord here
	int id_h1 = fork();

	if(id_h1==0){

            //inside coord
	    	
	    	pid_t ppid = getppid();
            cout<<"*********COORD CREATED BY ROOT "<<ppid<<" ********"<<endl;
	    	
            //allocate a buffer to hold pointer to fifo information object
	    	myfifo ** pipeList = (myfifo**)malloc(sizeof(myfifo*)*(workerNum+1));

            //declare named pipes
	    	for(int i=0;i<workerNum;i++){
                char * str_num = (char*)malloc(sizeof(char)*4);
                sprintf(str_num,"%d",i); //give each fifo a unique name specifed by index number
                mkfifo(str_num,0777); //make fifo
                //new fifo object
                myfifo * newFifo = new myfifo(str_num,0); //send fifo information to myfifo object to record them
                pipeList[i] = newFifo;
	    	}

            pipeList[workerNum] = NULL;


            //first create merger
            pid_t id_merger = fork();
            if(id_merger == 0){
                //merger code goes here
                //initialize time computing staff

                double t1,t2;
                struct tms tb1,tb2;
                double ticspersec;
                int i,sum=0;

                ticspersec = (double)sysconf(_SC_CLK_TCK);
                t1 = (double)times(&tb1);


                pid_t ppid = getppid();
                cout<<"************MERGER CREATED BY COORD "<<ppid<< " ***********"<<endl;

                //declare an array to hold buffer for reading each fifo
                char ** bufferList= (char**)malloc(sizeof(char*)*(workerNum));  
                //an array to hold the (size, used byte) info for each fifo
                int * buffer_size_list = (int*)malloc(sizeof(int)*(workerNum*2)); //list to hold size and read byte for each buffer, position 2i:buffer size, 2i+1: written byte 
                

                //initialize this buffer list
                for(int i=0;i<workerNum;i++){
                     bufferList[i] = (char*)malloc(sizeof(char)*2048);
                     if(bufferList[i]==NULL){
                         errExit("malloc");
                     }
                    buffer_size_list[2*i]=2048; //set initial buffer size 2048
                    buffer_size_list[2*i+1]=0;//set used byte to be 0
                }


                


                


                // first polling: read sorting result from each fifo
                int nfds,num_open_fds;
                struct pollfd *pfds;

                num_open_fds = nfds = workerNum;

                //calloc an array to hold struct pollfd, which is used for poll to check
                //if files are ready to perform I/O

                pfds = (struct pollfd *)calloc(nfds,sizeof(struct pollfd));
                if(pfds==NULL){
                    errExit("malloc");
                }

                //open every fifo in this for loop
                for(int j=0;j<nfds;j++){
                    char * fifoname = pipeList[j]->pathname;

                    //open fifo in NON_BLOCK MODE TO GET THE file descriptor
                    pfds[j].fd = open(fifoname,O_RDONLY|O_NONBLOCK);
                    if(pfds[j].fd == -1){
                        errExit("open");
                    }

                    cout<<"OPENED"<<fifoname<<"on fd "<<pfds[j].fd<<endl;
                    pfds[j].events = POLLIN; //set event POLLIN for poll to look for

                }

                //now start to keep calling poll while we still have non-closed fifo to read from
                while(num_open_fds>0){
                    int ready;

                    // cout<<"about to poll()"<<endl;

                    //poll function call
                    ready = poll(pfds,nfds,-1); 
                    
                    if(ready==-1){
                        errExit("poll");

                    }
                    // cout<<"ready: "<<ready<<endl;

                    //looping the pfds array to find the ready fifo
                    for(int j=0;j<nfds;j++){
                        

                        if(pfds[j].revents !=0){
                            // printf("  fd=%d; events: %s%s%s\n", pfds[j].fd,
                            //    (pfds[j].revents & POLLIN)  ? "POLLIN "  : "",
                            //    (pfds[j].revents & POLLHUP) ? "POLLHUP " : "",
                            //    (pfds[j].revents & POLLHUP) ? "POLLNVAL " : "",                            
                            //    (pfds[j].revents & POLLERR) ? "POLLERR " : "");

                            //bitwise operator, to check if the event is POLLIN, which means we can read
                            if(pfds[j].revents & POLLIN){

                                //offset value, tells us how many bytes have been used on the buffer
                                int offset = buffer_size_list[2*j+1];

                                //read function, we need to write onto bufferList[j] plus an offset, which is the used byte
                                ssize_t s = read(pfds[j].fd,bufferList[j]+offset,buffer_size_list[2*j]/2);
                                
                                //update read-bytes record for the specified buffer
                                int total_read = s+buffer_size_list[2*j+1];
                                buffer_size_list[2*j+1] = total_read;
                                
                                //we would need to resize  the buffer when needed, that is when read-bytes >= size/2
                                if(total_read >= buffer_size_list[2*j]/2){
                                    // cout<<"******going to resize with size"<<buffer_size_list[2*j]
                                    char * newBuffer = (char*)realloc(bufferList[j],buffer_size_list[2*j]*2);

                                    if(newBuffer==NULL){
                                        errExit("realloc");
                                    }
                                    bufferList[j] = newBuffer;//set new buffer as the buffer address for the named pipe
                                    buffer_size_list[2*j] = buffer_size_list[2*j]*2; //double the size
                                }

                                if(s==-1){
                                    errExit("read");
                                }

                                // cout<<"**********START**********"<<endl;
                                // cout<<"POLLIN TEST bufferList["<<j<<"]content "<<endl<<bufferList[j]<<endl;
                                // cout<<"POLLIN TEST bufferList["<<j<<"]size "<<endl<<buffer_size_list[j*2]/2<<endl;
                                // cout<<"***********END*********"<<endl;
                                // cout<<"read"<<s<<" bytes"<<endl;

                            }

                            //if we receive POLLHUP, it means the writing end of fifo has been closed
                            // we close the read end
                            else if(pfds[j].revents & POLLHUP){
                                cout<<"closing fd"<<pfds[j].fd<<endl;
                                if(close(pfds[j].fd) == -1){
                                    errExit("close");
                                }
                                // cout<<"line 215: content for fifo"<<j<<"\n"<<bufferList[j]<<endl;
                                num_open_fds--; //decrese the number of unfinished fifo
                            }

                        }
                    }

                }
                cout<<"all fifos are closed!"<<endl;




                //FIRST WRITE TO OUTPUT FILE HERE

                //check if file is already there
                if(access(outputFile,F_OK)==0){
                    remove(outputFile);
                }
               

                
                //first write all the sorting output to the output.txt
                for(int i=0;i<workerNum;i++){
                    FILE * fp = fopen(outputFile,"a+");
                    if(fp==NULL){
                        errExit("create");
                    }
                    fputs(bufferList[i],fp);
                    fclose(fp);
                

                }
                

            // now freeing the reading buffer for fifo
            cout<<"free buffer now"<<endl;
            for(int i=0;i<workerNum;i++){
                free(bufferList[i]);
            }

            cout<<"*****MERGER DOING FINAL SORT*********THIS COULD BE SLOW, PLEASE WAIT**********"<<endl;

            //DO FINAL MERGE, this part is essentially the same as a sorter
            Taxpayer ** payerList = (Taxpayer**)malloc(sizeof(Taxpayer*)*line_cnt);//store a list of ptr to payers

            if(payerList == NULL){
                cout<<"MALLOC FAILURE ON MAIN:288"<<endl;
            }

            FILE *fp = fopen(outputFile,"r") ;
            if(fp==NULL){
                cout<<"FILE NOT FOUND IN MERGER"<<endl;
                exit(-1);
            }               

            char * buffer = (char*)malloc(sizeof(char)*128); //buffer to hold each line
            size_t len =128;
            ssize_t final_read;

            char * token;
            Taxpayer * newPayer;             

            int cnt = 1;//used for counting the right line to start reading
            int attr_cnt = 0; //used to specify which attributes
            int payer_index = 0;
            int byteCount=0;//track the size of records in byte
            int startIndex = 1;
            int endIndex = line_cnt;

            //need to first translate text file into Taxpayer object before we sort
            while((final_read = getline(&buffer,&len,fp)) != -1){
            // cout<<"start in while loop"<<endl;
                byteCount += final_read;
                // cout<<"sorter"<<this_pid<<"read a line of length"<<read<<endl;
                // cout<<"line says:"<<buffer<<endl;
                //do strtok
                newPayer = new Taxpayer;
                attr_cnt = 0;//reset attr cnt every line

                if(cnt < startIndex){
                    cnt ++;
                    delete newPayer;
                }
                else{

                    token = strtok(buffer," ");
                    
                    //to do
                    while(token != NULL){
                        //set up taxpayer info
                        switch(attr_cnt){
                        case 0:
                        {

                            //set RID
                            int RID = atoi(token);
                            newPayer->setRid(RID);
                            break;
                        }


                        case 1:
                        {
                            //set first name
                            char * firstName = (char*)malloc(sizeof(char)*16);
                            strcpy(firstName,token);
                            newPayer->setFirstName(firstName);   
                            break;                 
                        }



                        case 2:
                        {
                              //set last name
                            char * lastName = (char*)malloc(sizeof(char)*16);
                            strcpy(lastName,token);
                            newPayer->setLastName(lastName);  
                            break;                
                        }


                        case 3:
                        {
                            //set depd
                            
                            int numOfDepd = atoi(token);
                            newPayer->setNumOfDepd(numOfDepd);
                            break;

                        }


                        case 4:
                        {


                            double income = strtod(token,NULL);
                            newPayer->setIncome(income);
                            break;

                        }
                        case 5:
                        {
                            int posCode = atoi(token);
                            newPayer->setPosCode(posCode);
                            break;
                        }


                        }
                        token = strtok(NULL," ");
                        attr_cnt ++;

                    }



                    char * test_name = newPayer->getFirstName();
                    int test_code = newPayer->getPosCode();

                    if(test_name == NULL){
                        cout<<"this is going wrong"<<endl;
                    }
                    // cout<<"payer info"<<test_name<<" "<<test_code<<endl;




                    payerList[payer_index] = newPayer;
                    

                    if(cnt == endIndex){
                        // cout<<"reaching endIndex "<<cnt<<endl;
                        // cout<<"payer_index at "<<payer_index<<endl;
                        break;
                    }

                    payer_index = payer_index+1;
               
                    cnt = cnt+1;
                    // cout<<"now should jump to next while"<<endl;

                }

            }
            if(buffer!= NULL){
                free(buffer);
            }
            
            fclose(fp);



            // cout<<"********MERGER Using bubble sort********"<<endl;
       
            if(strcmp(orderFlag,"a")==0){
                BubbleSort(payerList,endIndex-startIndex+1,0,attrNum);
    
            }
            else{
                BubbleSort(payerList,endIndex-startIndex+1,1,attrNum);

            }
                        

            //NOW FINAL WRITE

            FILE * final_fp = fopen(outputFile,"w+");
            if(final_fp == NULL){
                cout<<"OPEN FILE ERROR ON FINAL WRITE"<<endl;
            }
            for(int i=0;i<line_cnt;i++){
                fputs(joinPayerInfo(payerList[i]),fp);

            }

            fclose(final_fp);
            cout<<"**********MERGER FINISH FINAL WRITE*****************"<<endl;

            //second polling: read time statistics
            //begin to read time stat

            //use poll
            int stat_nfds,stat_num_open_fds;
            struct pollfd *stat_pfds;
            ssize_t stat_read;

            stat_num_open_fds = stat_nfds = workerNum;
            stat_pfds = (struct pollfd *)calloc(stat_nfds,sizeof(struct pollfd));

            if(stat_pfds==NULL){
                errExit("malloc");
            }

            for(int j=0;j<stat_nfds;j++){
                char * fifoname = pipeList[j]->pathname;
                stat_pfds[j].fd = open(fifoname,O_RDONLY|O_NONBLOCK);
                if(stat_pfds[j].fd == -1){
                    errExit("open");
                }

                cout<<"OPENED"<<fifoname<<"on fd"<<stat_pfds[j].fd<<endl;
                stat_pfds[j].events = POLLIN;

            }
            //now keep calling poll
            while(stat_num_open_fds>0){
                int ready;

                // cout<<"about to poll()"<<endl;
                ready = poll(stat_pfds,stat_nfds,-1);
                
                if(ready==-1){
                    errExit("poll");

                }
                // cout<<"ready: "<<ready<<endl;

                for(int j=0;j<stat_nfds;j++){
                    

                    if(stat_pfds[j].revents !=0){
                        // printf("  fd=%d; events: %s%s%s\n", stat_pfds[j].fd,
                        //    (stat_pfds[j].revents & POLLIN)  ? "POLLIN "  : "",
                        //    (stat_pfds[j].revents & POLLHUP) ? "POLLHUP " : "",
                        //    (stat_pfds[j].revents & POLLHUP) ? "POLLNVAL " : "",                            
                        //    (stat_pfds[j].revents & POLLERR) ? "POLLERR " : "");


                        if(stat_pfds[j].revents & POLLIN){
                            char * stat_buffer = (char*)malloc(sizeof(char)*128); 
                            stat_read = read(stat_pfds[j].fd,stat_buffer,128);
                            if(stat_read ==-1){
                                cout<<"error on line 521, reading from stat fifo"<<endl;
                            }
                            cout<<"*********************"<<endl;
                            cout<<"time taken for sorter "<<j<<"to finish the job: "<<stat_buffer<<endl;
                            cout<<"*********************"<<endl;
                            free(stat_buffer);
                          
                        }
                        else if(stat_pfds[j].revents & POLLHUP){
                            cout<<"closing fd for stat"<<stat_pfds[j].fd<<endl;
                            if(close(stat_pfds[j].fd) == -1){
                                errExit("close");
                            }
                            // cout<<"line 215: content for fifo"<<j<<"\n"<<bufferList[j]<<endl;
                            stat_num_open_fds--;
                        }

                    }
                }

            }


            //remove the fifo afterwards
            int remove_stat;
            for(int i=0;i<workerNum;i++){
                char * pathname = pipeList[i]->pathname;
                if((remove_stat=remove(pathname))==0){
                    cout<<"fifo "<<pathname<<" safely moved"<<endl;
                }
            }

            free(pipeList);

            t2 = (double)times(&tb2);
            float statResult = (t2-t1)/ticspersec; //calculating the time spent by merger until finishing its job

            cout<<"******************MERGER TIME INFO**************"<<endl;
            cout<<"*****************SPEND "<<statResult<<" TO FINISH ALL MERGE JOB************"<<endl;
            
            //merger send SIGUSR2 to coord
            int coord_id = getppid();
            kill(coord_id,12);

            }
            else{
                //coord code goes here
                cout<<"creating sorters..."<<endl;

                //those are for fixed size assignment
                int start=1;
                int offset = line_cnt/workerNum;
                int startIndex;
                int endIndex;
                
                //those are used for random size assignment
                int r_offset=0;
                int temp_r_offset;
                int r_start = 1;
                int r_end=0; 
                int r_start_list[workerNum];
                int r_end_list[workerNum];
                if(rangeFlag == 0){
                    //set random number generator
                    time_t t;
                    srand((unsigned)time(&t));

                    //compute random start and end range for each sorter
                    for(int i=0;i<workerNum;i++){
                        cout<<"inside for loop"<<endl;
                        if(r_offset == line_cnt){
                            cout<<"REACH LINE LIMIT, ONLY GOING TO GERERATE "<<i<<" SORTERS"<<endl;
                            workerNum--;
                            break;
                        }
                        //random range
                        if(i==workerNum-1){
                            //last one, go for all the rest, instead of another random number
                            temp_r_offset = line_cnt-r_offset;

                           
                            r_start = r_end + 1;
                            r_end = r_end + temp_r_offset;
                           
                            r_start_list[i] = r_start;
                            r_end_list[i] = r_end;

                            break;
                        }

                        temp_r_offset = rand()%(line_cnt-r_offset); //temp_r_offset shows the number of records this sorter should sort
                        
                        //handle case when temp_r_offset gives 0, which would cause error
                        // if(temp_r_offset == 0){
                        //     r_start = r_end;
                            
                        // }

                        r_start = r_end + 1;
                        r_end = r_end + temp_r_offset;
                 


                        r_offset = r_end; 
                        r_start_list[i] = r_start;
                        r_end_list[i] = r_end;


                    }

                    //start forking sorters under random range mode
                    int f;
                    for(f=0;f<workerNum;f++){
                        pid_t id_h2 = fork();
                        // cout<<id_h2<<endl;
                        if(id_h2==-1){
                            cout<<"forking sorter failed!"<<endl;
                            exit(-1);
                        }
                        if(id_h2==0){
                            //inside sorter
                            // cout<<"inside sorter"<<endl;
                            pid_t ppid= getppid();

                            cout<<"************SORTER CREATED BY COORD "<<ppid<<" **************"<<endl;

                            // those are for translating information that the sorter need into string format in order for exec
                            char * this_fifo = pipeList[f]->pathname; 
                            char * char_start = (char *)malloc(sizeof(char)*16);
                            char * char_end = (char*)malloc(sizeof(char)*16);
                            char char_attrNum[5];
                            char char_ppid[5];
                            sprintf(char_start,"%d",r_start_list[f]);
                            sprintf(char_end,"%d",r_end_list[f]);
                            sprintf(char_attrNum,"%d",attrNum);
                            sprintf(char_ppid,"%d",ppid);
                            cout<<"************TEST START INDEX ****** IS "<<char_start<<" END INDEX IS "<<char_end<<"*********"<<endl;

                            
                           

                            // cout<<"trying to exec"<<endl;
                            if((f%2)==0){
                                //even Index, use bubble (-algo b)
                                execlp("./sorter","sorter","-f",inputFile,"-algo","b","-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);
                            }else{
                                //odd index, use insertion sort (-algo i)
                                execlp("./sorter","sorter","-f",inputFile,"-algo","i","-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);

                            }                           

                        }
                    }

                }

                //forking children under even size mode
                if(rangeFlag==1){
                    int f;
                    for(f=0;f<workerNum;f++){
                        pid_t id_h2 = fork();
                        if(id_h2==0){
                            pid_t ppid= getppid();
                            cout<<"************SORTER CREATED BY COORD "<<ppid<<" **************"<<endl;

                            //inside sorter
                            //compute start and end index for each sorter
                            char * this_fifo = pipeList[f]->pathname; 
                            // cout<<"checking before exec --> pathname of fifo -->"<<this_fifo<<endl;


                            if(f == workerNum-1){

                                    startIndex = 1 + (offset*f);
                                    // cout<<"*********TEST AT LINE 573 STARTINDEX IS "<<startIndex<<endl;
                                    offset = line_cnt - (f*offset);
                                    // cout<<"offset: "<<offset<<endl;
                                    // cout<<"current f value:"<<f<<endl;
                                    endIndex = startIndex+offset-1;
                                    //convert int to string
                                    int this_pid = getpid();
                                    // cout<<"last sorter:" <<this_pid<<"start at"<<startIndex<<"end at "<<endIndex<<endl;
                                    char * char_start = (char *)malloc(sizeof(char)*16);
                                    char * char_end = (char*)malloc(sizeof(char)*16);
                                    char char_attrNum[5];
                                    char char_ppid[5];
                                    sprintf(char_start,"%d",startIndex);
                                    sprintf(char_end,"%d",endIndex);
                                    sprintf(char_attrNum,"%d",attrNum);
                                    sprintf(char_ppid,"%d",ppid);
                                    cout<<"************TEST START INDEX ****** IS "<<char_start<<endl;
                                   

                                    // cout<<"trying to exec"<<endl;
                                    if((f%2)==0){
                                        //odd Index, use sorter
                                        execlp("./sorter","sorter","-f",inputFile,"-algo","b","-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);
                                    }else{
                                        execlp("./sorter","sorter","-f",inputFile,"-algo","i","-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);

                                    }



                            }
                            else{
                                //HANDLE RANDOM RANGE MODE/NON-RANDOM, rangeFlag = 1 when non random
                                int this_pid = getpid();

                                    startIndex = 1 + offset*f;
                                    // cout<<"*********TEST AT LINE 573 STARTINDEX IS "<<startIndex<<endl;

                                    endIndex = startIndex + offset - 1;
                                    // cout<<f<<" sorter:" <<this_pid<<"start at"<<startIndex<<"end at "<<endIndex<<endl;
                                    char * char_start = (char *)malloc(sizeof(char)*16);
                                    char * char_end = (char*)malloc(sizeof(char)*16);
                                    char char_attrNum[5];
                                    char char_ppid[5];
                                    sprintf(char_start,"%d",startIndex);
                                    sprintf(char_end,"%d",endIndex);
                                    sprintf(char_attrNum,"%d",attrNum);
                                    sprintf(char_ppid,"%d",ppid);
                                    // cout<<"************TEST START INDEX ****** IS "<<char_start<<endl;

                                    // cout<<"trying to exec"<<endl;
                                    if((f%2)==0){
                                        //odd Index, use bubble
                                        execlp("./sorter","sorter","-f",inputFile,"-algo","b","-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);
                                    }else{
                                        execlp("./sorter","sorter","-f",inputFile,"-algo","i","-ppid",char_ppid,"-p",this_fifo,"-o",orderFlag,"-a",char_attrNum,"-s",char_start,"-e",char_end,(char*)NULL);

                                    }



                            }
                            
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
        //root is going to wait for coord to finish first, then it exits
            cout<<"**********************************"<<endl;

	    	cout<<"ROOT WATING FOR SIGUSR1..."<<endl;
            cout<<"**********************************"<<endl;

            int status;
            waitpid(id_h1,&status,0);
            main_t2 = (double)times(&main_tb2);

            float main_statResult = (main_t2-main_t1)/main_ticspersec;

            cout<<"*************MESSAGE FROM ROOT*************"<<endl;
            
            //print out the turnaround time for the entire program
            cout<<"************TURNAROUND TIME: TAKE AN ENTIRE TIME OF "<<main_statResult<<" FOR THE ENTIRE SORTING TIME TO FINISH***********"<<endl;
            // cout<<"************NUMBER OF SIGUS1 CATCHED BY ROOT: "<<SIGNAL1<<" ***********"<<endl;

            cout<<"**************coord is now finished, root terminate***********"<<endl;
            cout<<"BYE"<<endl;
	    }






    return 0;
}


/*
signal handler for SIGUSR1
*/
void signal_handler_1(int signNO){
    // signal(10,signal_handler_1);
    cout<<"******RECIEVE SIGUSR1 FOR "<<++SIGNAL1<<"TIMES *************"<<endl;
    

}


/*
signal handler for SIGUSR2
*/
void signal_handler_2(int signNo){
    cout<<"receive SIGUSR2 from merger, coord exit"<<endl;
    exit(0);
}


void exit_handler(int signNo){
    exit(-1);
}






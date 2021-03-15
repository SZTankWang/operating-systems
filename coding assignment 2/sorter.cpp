#include "Taxpayer.h"
#include "algorithm.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <fcntl.h>
#include <cstdio>
#include <ostream>
#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

void exitHandler(int signNo);//handler for ctrl+C




//invoke by command: ./bubble -f <fileName> "-algo",<i/b>,"-ppid",char_ppid, -o <order> -a <attrNum> -s <start> -e <end>
int main(int argc,char ** argv){
    //initialize time computing staff
    // clock_t tStart = clock();
    double t1,t2,cpu_time;
    struct tms tb1,tb2;
    double ticspersec;
    int i,sum=0;

    ticspersec = (double)sysconf(_SC_CLK_TCK);
    t1 = (double)times(&tb1);



    //those are for reading argument passed by exec
	char * fileName;
    char * fifoName; //the fifo name for this sorter
    char * sort_algo;
	int attrNum; //field to sort (1,4,5,6)
	int order=0; //0 by default
	int startIndex; //start line number
	int endIndex;//end line number
    int ppid;
    int this_pid = getpid(); 

    //register ctrl+C signal handler
    signal(SIGINT,exitHandler);
    // signal(SIGSEGV,exitHandler);
    // cout<<"new sorter"<<endl;

	for(int i=0;i<argc;i++){
		if(strcmp(argv[i],"-f")==0){
			fileName = argv[i+1];
		}

        if(strcmp(argv[i],"-algo")==0){
            sort_algo = argv[i+1];
        }
		if(strcmp(argv[i],"-o")==0){
			if(strcmp(argv[i+1],"d")==0){
				order = 1;
			}
		}

        if(strcmp(argv[i],"-p")==0){
            fifoName = argv[i+1];
        }
        
		if(strcmp(argv[i],"-a")==0){
            attrNum = atoi(argv[i+1]);

		}
		if(strcmp(argv[i],"-s")==0){
			startIndex = atoi(argv[i+1]);
            // cout<<"start index for "<<this_pid<<": "<<startIndex<<endl;
		}
		if(strcmp(argv[i],"-e")==0){
			endIndex = atoi(argv[i+1]);
            // cout<<"end index for "<<this_pid<<": "<<endIndex<<endl;
        }

        if(strcmp(argv[i],"-ppid")==0){
            ppid = atoi(argv[i+1]);
        }
	}


	int  n = endIndex-startIndex+1; //n is the total number of Taxpayer for this sorter
    if(endIndex >= startIndex){
            Taxpayer ** payerList = (Taxpayer**)malloc(sizeof(Taxpayer*)*n);
        if(payerList == NULL){
            cout<<"MALLOC FAILURE ON LINE 294"<<endl;
        }

        //open file, read taxpayer
        FILE *fp = fopen(fileName,"r") ;
        if(fp==NULL){
            cout<<"FILE NOT FOUND IN SORTER"<<endl;
            exit(-1);
        }

        char * buffer = (char*)malloc(sizeof(char)*128); //buffer to hold each line
        size_t len =128;//max line size
        ssize_t read; //read byte from each read

        char * token;
        Taxpayer * newPayer;


        int cnt = 1;//used for counting the right line to start reading
        int attr_cnt = 0; //used to specify which attributes
        int payer_index = 0;
        int byteCount=0;//track the size of records in byte, this would be used for writing to fifo

        while((read = getline(&buffer,&len,fp)) != -1){
            // cout<<"start in while loop"<<endl;
            byteCount += read;
            // cout<<"sorter"<<this_pid<<"read a line of length"<<read<<endl;
            // cout<<"line says:"<<buffer<<endl;
            //do strtok
            newPayer = new Taxpayer;
            attr_cnt = 0;//reset attr cnt every line


            // we need to increase to the correct starting line to read
            if(cnt < startIndex){
                cnt ++;
                delete newPayer;
            }
            else{

                //use strtok to extract the attrbute fields in each line
                //and set them individually to a new sorter object
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

                //just for test
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

        // cout<<"*****TEST PAYER ****is ******* "<<payerList[n-1]->getFirstName()<<endl;
        // int length_list = sizeof(payerList)/sizeof(payerList[0]);

        // cout<<"sorter "<<this_pid<<"has "<<length_list-1<<"payers!"<<endl;


        if(buffer != NULL){
            free(buffer);
        }

        fclose(fp);

        // do sorting
        // cout<<"**********BEFORE BUBBLE SORT**********"<<endl;

        if(strcmp(sort_algo,"b")==0){
            cout<<"********Using bubble sort********"<<endl;
            BubbleSort(payerList,endIndex-startIndex+1,order,attrNum);
        }
        if(strcmp(sort_algo,"i")==0){
            cout<<"********Using Insertion sort********"<<endl;

            insertionSort(payerList,endIndex-startIndex+1,order,attrNum);
        }
        //use a buffer to read all the data of size (byteCount)
        char * fifo_buffer = (char *)malloc(sizeof(char)*(byteCount+n)); 

        if(fifo_buffer == NULL){
            cout<<"malloc fail!"<<endl;
            exit(-1);

        }
        // cout<<"**********AFTER BUBBLE SORT**********"<<endl;


        
        // cout<<"we are now in the join payerinfo part"<<endl;

        // cout<<"trying to access the first payer"<<endl;
        // int test_temp = payerList[n-1]->getPosCode();
        // cout<<"*******FYI, poscode IS*****  "<<test_temp<<endl;
        // cout<<"test last name "<<test_temp<<endl;

        //iteratively concatenate the fields of a payer into a whole string
        for(int a=0;a<n;a++){
            char * temp = joinPayerInfo(payerList[a]);
            if(a==0){
                strcpy(fifo_buffer,temp);
            }
            else{
                strcat(fifo_buffer,temp);
            }
            free(temp);
            


        }

        // cout<<"start line is "<<startIndex<<" and end Index is "<<endIndex<<endl;
        // cout<<"number of payers (should be ?)"<<n<<endl;

        // cout<<"num of record for sorter"<<this_pid<<"is "<<n<<endl;

        //store an array of ptr to payers



        // write to fifo, open fifo for the first time
        int fd = open(fifoName,O_WRONLY);
        if(fd == -1){
            cout<<"fifo opened wrongly, fifo path name is "<<fifoName<<endl;
        }
        // cout<<" trying to write to fifo"<<fifoName<<"in 3 seconds..."<<endl;
        // sleep(3);
        // cout<<"content from sorter"<<fifoName<<""<<fifo_buffer<<endl;

        //start to write to the fifo here
        write(fd,fifo_buffer,byteCount+n);

        // char killSignal[18] = "kill this";
        // write(fd,killSignal,sizeof());

        // close fifo 
        close(fd);

        //delete all the payer 
        for(int a=0;a<n;a++){
            delete payerList[a];
        }

        //free write fifo_buffer in the end
        free(fifo_buffer);

    }
    else{
        int fd = open(fifoName,O_WRONLY);
         if(fd == -1){
            cout<<"fifo opened wrongly, fifo path name is "<<fifoName<<endl;
        }       
        // close fifo 
        close(fd);        
    }

    //send SIGUSR1 to coord
    kill(ppid,10);

    t2 = (double)times(&tb2);//this is the time when the work for the sorter has finished
    
    //open pipe again, send time info
    sleep(2); //sleep for some sec, or it cause error
    int new_fd = open(fifoName,O_WRONLY);
    if(new_fd == -1){
        cout<<"fifo opened wrongly, fifo path name is "<<fifoName<<endl;
    }

    char * stat = (char*)malloc(sizeof(char)*128);
    sprintf(stat,"%lf",(t2-t1)/ticspersec);
    // cout<<"******************sorter trying to write stat *********"<<stat<<endl;
    
    //write to fifo for the second time
    write(new_fd,stat,128);

    close(new_fd);//close fifo

    float statResult = (t2-t1)/ticspersec;

    // cout<<"*******RUN TIME FOR SORTER******"<<endl<<"*****"<<statResult<<"*********"<<endl;

	return 0;
}

void exitHandler(int signNo){
    cout<<"sorter exit"<<endl;

    exit(-1);

}
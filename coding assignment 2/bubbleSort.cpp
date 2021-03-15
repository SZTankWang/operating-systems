#include "Taxpayer.h"
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

/*
join all the info fields of a taxpayer into a char buffer for writing in the fifo
*/
void catStrWithSpace(char * dest, char * src){
    strcat(dest,src);
    strcat(dest," ");

}


char * joinPayerInfo(Taxpayer * payer){
    char * temp = (char *)malloc(sizeof(char)*256);
    char RID[16];
    int RID_INT = payer->getRid(); 
    sprintf(RID,"%d",RID_INT);
    strcpy(temp,RID);
    strcat(temp," ");
    catStrWithSpace(temp,payer->getFirstName());
    catStrWithSpace(temp,payer->getLastName());

    char depd[16];
    int depd_INT = payer->getNumOfDepd();
    sprintf(depd,"%d",depd_INT);
    catStrWithSpace(temp,depd);

    char income[16];
    double income_INT = payer->getIncome();
    sprintf(income,"%f",income_INT);
    catStrWithSpace(temp,income);

    char posCode[16];
    int pos_INT = payer->getPosCode();
    // cout<<"*****INSIDE joinInfo<<<<poscode is "<<pos_INT<<endl;
    sprintf(posCode,"%d",pos_INT);
    catStrWithSpace(temp,posCode);
    //add newline character at the end
    strcat(temp,"\n");

    return temp;
}




void swap(Taxpayer * ptr1, Taxpayer * ptr2){
    // cout<<"******INSIDE SWAP*****"<<endl;
 //    char * temp_first = (char*)malloc(sizeof(char)*16);
 //    if(temp_first==NULL){
 //        cout<<"MALLOC ERROR ON 63"<<endl;
 //    }
 //    char * temp_last = (char*)malloc(sizeof(char)*16);
 //    if(temp_first==NULL){
 //        cout<<"MALLOC ERROR ON 67"<<endl;
 //    }
	// Taxpayer * temp = new Taxpayer;
 //    //copy name
 //    strcpy(temp_first,ptr1->getFirstName());
 //    temp->setFirstName(temp_first);

 //    strcpy(temp_last,ptr1->getLastName());
 //    temp->setLastName(temp_last);

 //    temp->setPosCode(ptr1->getPosCode());
 //    temp->setNumOfDepd(ptr1->getNumOfDepd());
 //    temp->setIncome(ptr1->getIncome());
 //    temp->setRid(ptr1->getRid());

    Taxpayer * temp = (Taxpayer*)malloc(sizeof(Taxpayer));
    *temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = *temp;
    free(temp);
    // delete temp;
}


//sortAttr: attribute to sort; sortFlag:0:ASCE; 1: DESC
int doSorting(Taxpayer ** payerList,int sortAttr,int sortFlag,int start,int end, int swapped){
    // cout<<"***********INSIDE DO SORTING*******"<<endl;
    if(sortFlag==0){

        switch(sortAttr){

        case 1:
        {

            //sort RID
            if(payerList[start]->getRid()>payerList[end]->getRid()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }
            break;

        }


        case 4:

        {

             //sort num of depd
            if(payerList[start]->getNumOfDepd()>payerList[end]->getNumOfDepd()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }     

            break;      
        }


        case 5:
        {
            //sort income
            if(payerList[start]->getIncome()>payerList[end]->getIncome()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }
            break;
        }


        case 6:
        {

            //sort posCode
            if(payerList[start]->getPosCode()>payerList[end]->getPosCode()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }

            break;            
        }

        default:
            cout<<"invalid sorting attribute!"<<endl;
            break;
        }

        return swapped;
    }
    else if(sortFlag==1){

        switch(sortAttr){

        case 1:
        {

             //sort RID
            if(payerList[start]->getRid()<payerList[end]->getRid()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }      
            break;     
        }


        case 4:
        {
             //sort num of depd
            if(payerList[start]->getNumOfDepd()<payerList[end]->getNumOfDepd()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }   
            break;         
        }


        case 5:
        {
             //sort income
            if(payerList[start]->getIncome()<payerList[end]->getIncome()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }   
            break;         
        }


        case 6:
        {
             //sort posCode
            if(payerList[start]->getPosCode()<payerList[end]->getPosCode()){
                swap(payerList[start],payerList[end]);
                swapped = 1;
            }       
            break;     
        }


        default:
            cout<<"invalid sorting attribute!"<<endl;
            break; 
        }

        return swapped;
    }

    else{
        cout<<"invalid sort flag"<<endl; //invalid sortFlag
        return swapped;
    }


}

//flag=0: sort in ASCE; flag=1: DESC
void BubbleSort(Taxpayer ** payerList,int length,int flag,int sortAttr){
	int swapped = 0;
    int start;
    int end;
    // cout<<"*********INSIDE BUBBLE********"<<endl;
	while(1){
		for(int i=0;i<length-1;i++){
            start = i;
            end = i+1;
            swapped = doSorting(payerList,sortAttr,flag,start,end,swapped);

		}
		if(swapped==0){
			// cout<<"*************SWAPPED IS ZERO**********"<<endl;
            break;

		}
		else{
			swapped = 0;
		}

	}
}


void insertionSort(Taxpayer ** payerList, int length, int flag, int sortAttr){
    int i,j;
    Taxpayer * key;
    for(i=1;i<length;i++){
        key = payerList[i];
        j=i-1;

        if(flag == 0){
                //ASEC
            switch(sortAttr){

            case 1:
            {

                 //sort RID
                while(j>=0 && payerList[j]->getRid()>key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;     
            }


            case 4:
            {
                 //sort num of depd
                while(j>=0 && payerList[j]->getNumOfDepd()>key->getNumOfDepd()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 5:
            {
                 //sort income
                while(j>=0 && payerList[j]->getIncome()>key->getIncome()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 6:
            {
                 //sort posCode
                while(j>=0 && payerList[j]->getPosCode()>key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;     
                break;     
            }


            default:
                cout<<"invalid sorting attribute!"<<endl;
                break; 
            }

        }
         else if(flag == 1){
        //SORT DESC
            switch(sortAttr){

            case 1:
            {

                 //sort RID
                while(j>=0 && payerList[j]->getRid()<key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;     
            }


            case 4:
            {
                 //sort num of depd
                while(j>=0 && payerList[j]->getNumOfDepd()<key->getNumOfDepd()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 5:
            {
                 //sort income
                while(j>=0 && payerList[j]->getIncome()<key->getIncome()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;
                break;         
            }


            case 6:
            {
                 //sort posCode
                while(j>=0 && payerList[j]->getPosCode()<key->getRid()){
                    payerList[j+1] = payerList[j];
                    j = j-1;

                }
                payerList[j+1] = key;     
                break;     
            }


            default:
                cout<<"invalid sorting attribute!"<<endl;
                break; 
            }

        }

    }
}
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



	char * fileName;
    char * fifoName;
    char * sort_algo;
	int attrNum;
	int order=0; //0 by default
	int startIndex;
	int endIndex;
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

	//open file, read taxpayer
	int  n = endIndex-startIndex+1;
    // cout<<"start line is "<<startIndex<<" and end Index is "<<endIndex<<endl;
    // cout<<"number of payers (should be ?)"<<n<<endl;

    // cout<<"num of record for sorter"<<this_pid<<"is "<<n<<endl;
    Taxpayer ** payerList = (Taxpayer**)malloc(sizeof(Taxpayer*)*n);//store a list of ptr to payers
    if(payerList == NULL){
        cout<<"MALLOC FAILURE ON LINE 294"<<endl;
    }

    FILE *fp = fopen(fileName,"r") ;
    if(fp==NULL){
        cout<<"FILE NOT FOUND IN SORTER"<<endl;
        exit(-1);
    }

    char * buffer = (char*)malloc(sizeof(char)*128); //buffer to hold each line
    size_t len =128;
    ssize_t read;

    char * token;
    Taxpayer * newPayer;


    int cnt = 1;//used for counting the right line to start reading
    int attr_cnt = 0; //used to specify which attributes
    int payer_index = 0;
    int byteCount=0;//track the size of records in byte

    while((read = getline(&buffer,&len,fp)) != -1){
        // cout<<"start in while loop"<<endl;
        byteCount += read;
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
    //use a buffer to read all the data
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


    // write to fifo
    int fd = open(fifoName,O_WRONLY);
    if(fd == -1){
        cout<<"fifo opened wrongly, fifo path name is "<<fifoName<<endl;
    }
    // cout<<" trying to write to fifo"<<fifoName<<"in 3 seconds..."<<endl;
    // sleep(3);
    // cout<<"content from sorter"<<fifoName<<""<<fifo_buffer<<endl;

    write(fd,fifo_buffer,byteCount+n);

    // char killSignal[18] = "kill this";
    // write(fd,killSignal,sizeof());

    close(fd);

    //delete all the payer 
    for(int a=0;a<n;a++){
        delete payerList[a];
    }

    //free write fifo_buffer in the end
    free(fifo_buffer);

    //send SIGUSR1 to root
    kill(ppid,10);

    t2 = (double)times(&tb2);
    //open pipe again, send time info
    sleep(2);
    fd = open(fifoName,O_WRONLY);
    if(fd == -1){
        cout<<"fifo opened wrongly, fifo path name is "<<fifoName<<endl;
    }

    char * stat = (char*)malloc(sizeof(char)*128);
    sprintf(stat,"%lf",(t2-t1)/ticspersec);
    // cout<<"******************sorter trying to write stat *********"<<stat<<endl;
    write(fd,stat,128);

    close(fd);

    float statResult = (t2-t1)/ticspersec;

    // cout<<"*******RUN TIME FOR SORTER******"<<endl<<"*****"<<statResult<<"*********"<<endl;

	return 0;
}

void exitHandler(int signNo){
    cout<<"sorter exit"<<endl;

    exit(-1);

}
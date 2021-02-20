#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashTable.h"
#include "helper.h"

using namespace std;

int main( int argc, char **argv ) {
    cout << "starting mvote..." << endl;
    int numOfLine;
    int bucketSize = atoi(argv[4]);
    auto * Helper = new helper();
    numOfLine = Helper->getNumOfLine(argv[2]);

    int run_flag = 1;

    char * readPtr;
    char ACTION_EXIT[] = "exit";
    char ACTION_LOOKUP[] = "l";
    char ACTION_INSERT[] = "i";
    char ACTION_REGISTER[] = "r";
    char ACTION_COUNT[] = "v";
    char ACTION_DELETE[] = "d";
    char ACTION_PERC[] = "perc";
    char ACTION_ZIP[] = "z";
    char ACTION_BV[] = "bv";
    char ACTION_O[] = "o";

    char * actionBuffer;
    char * action;
    auto * myHash = new hashTable(bucketSize);
//    int size = 10;
    voter ** tablePtr = myHash->getTablePtr();
    for(int i=0;i<bucketSize;i++){
        tablePtr[i] = nullptr;
    }
    int result; //result flag for inserting a voter


    char * spacePtr; // to indicate if there are spaces in the command

    int paramIndex = 0;//used for reading parameter from user input
    //parse command line argument
    char * filename = argv[2];

    FILE * fp;
    char line[64] ;

    //initialize pointer for strtok
    char * pch;
    fp = fopen(filename,"r");
    if(fp == NULL){
        cout<<"file not exists";
        return -1;
    }


    /* read file, build up the hash table
     *
     * */
    while(fgets(line,64,fp)!= NULL){

        //split this line by space
        pch = strtok(line," ");
        voter * newVoter = new voter();//this should now be empty
        char * arr[4];
        int counter =0;
        while(pch!=NULL && counter<5){
//            cout<<pch<<"\n";
            arr[counter] = pch;

            pch = strtok(NULL," ");

            counter ++;
        }

        //now use arr to build up voter
        newVoter->setUpVoter(arr);
        result = myHash->insertVoter(newVoter);
        if(result == 0){
            cout<<"inserted !"<<"\n";
        }
        if(result == -1){
            cout<<"duplicated!"<<"\n";
        }

    }
    fclose(fp);


    //output welcome message
    cout<<"mvote|| version 1.0|| author: Zhenming Wang"<<endl;
    cout<<"command: {l <RIN>} to look up a user with RIN|| {i <RIN> <firstName> <lastName> <zipCode>} to insert a voter||"<<endl;
    cout<<"{d <rin>} to delete a voter with ID <rin> || {r <rin>} to register for a voter as voted || {bv <filename>} to vote for all the keys in the file. Note: you can use voters50.csv, for example, and put it in the same directory!||"<<endl;
    cout<<"{v} to present number of people voted so far || {perc} to display voted / total number ratio. || {z <zipcode>} to print participants that have voted in <zipcode>|| {o} to produce a sorted list of zipcode by number of voted people in DESC order||"<<endl;
    cout<<"{exit} to quit mvote. Enjoy :)"<<endl;
    cout<<"----------------------------------------"<<endl;


    /*start command loop
     *
     * */


    while(run_flag == 1){
        actionBuffer = (char*)malloc(100*sizeof(char));
        action = (char*)malloc(200*sizeof(char));

        char * helperPtr; // this is used for strtok, to seperate params from user input
        char tempBuffer [100]; // as well

        paramIndex = 0; //reset this index everytime

        cout<<"--------------------------------------"<<endl;
        cout<<"$mvote:";
        cin.getline(actionBuffer,100);

        //check if the command has spaces in it or not
        spacePtr = Helper->parseCommand(actionBuffer);

        if(spacePtr == nullptr){

            /*
             * ACTION EXIT
             * */
            if(strcmp(actionBuffer, ACTION_EXIT)==0){


                run_flag=0;

            }

            /*
             * ACTION COUNT(v)
             * */
            if(strcmp(actionBuffer,ACTION_COUNT)==0){
                int result;
                result = myHash->getTotalVote();
                cout<<"total number of votes registered: "<<result<<endl;
            }

            /*
             * ACTION perc
             * */
            if(strcmp(actionBuffer,ACTION_PERC)==0){
                int total=0;
                int voteCount = 0;
                float result;
                total = myHash->getSize();
                voteCount = myHash->getTotalVote();
                result = float(voteCount * 100 / total);
                cout<<"voted people/total : "<<result<<"%"<<endl;
            }


            /*ACTION o: OUTPUT ZIPCODE IN DECS ORDER BY PEOPLE NUM
             * */
            if(strcmp(actionBuffer,ACTION_O)==0){
                posCodeList * listPtr = myHash->getListPointer();
                listPtr->outputDesc();
            }

        }


        // if command contain space, need to read parameters
        if(spacePtr != nullptr){



            //readPtr indicates the first occurrence of space in the command
            readPtr = strchr(actionBuffer,' ');


            int size = readPtr-actionBuffer;
            //extract the operation
            action[size] = '\0';

            strncpy(action,actionBuffer,size);

            cout<<"operation:"<<action<<endl;



            /*ACTION BULK VOTE
             * VOTE FOR A GROUP OF USERS
             * */
            if(strcmp(action,ACTION_BV)==0){
                char file[20]="";
                strncpy(file,readPtr+1,sizeof(file));
                cout<<file<<endl;
                int RIN;


                FILE * fp;
                char line[50] ;
                fp = fopen(file,"r");


                if(fp == nullptr){
                    cout<<"file not found"<<endl;
                }
                else{
//                    while(fgets(line,10,fp)!=NULL){
//
//                        RIN = atoi(line);
//                        myHash->doVote(RIN);
//
//                    }

                    while(fgets(line,50,fp)!=NULL){
                        pch = strtok(line," ");
                        //RIN is *pch
                        RIN = atoi(pch);
                        myHash->doVote(RIN);

                    }




                }
            }


            /*
             * ACTION: Z <zipcode>, list the number of voted people of the given zipcode
             * */
            if(strcmp(action,ACTION_ZIP)==0){
                char key[10]="";
                char * paramPtr = Helper->findParam(readPtr);
                int lookUpZip;
                lookUpZip = Helper->translateKey(paramPtr,readPtr,key,paramIndex);

                //look for the specific node
                posCodeList * listPtr = myHash->getListPointer();
                if(listPtr->isEmpty() !=0){
                    posNode * nodePtr = listPtr->findNode(lookUpZip);
                    if(nodePtr!= nullptr){
                        nodePtr->showVoterList();

                    }else{
                        cout<<"no such zip code"<<endl;
                    }
                }
                else{
                    cout<<"there is no people voted at this time"<<endl;
                }


            }


            /*
             * case: operation l <key>, LOOK UP a voter with given key as RIN
             * */
            if(strcmp(action,ACTION_LOOKUP)==0){
                // get search key
                char key[10] = "";

                //call helper function getParam
                //the returned paramPtr would be used to help check the input
                char * paramPtr = Helper->findParam(readPtr);

                //call lookup() function
                int lookUpKey;

                //use translateKey to make sure there are no extra params. We only need an integer key
                lookUpKey = Helper->translateKey(paramPtr,readPtr,key,paramIndex);
                if(lookUpKey != -1){
                    int hashSize = myHash->getSize();
                    cout<<"FYI,the size of hashtable: "<< hashSize<<endl;
                    info lookUpInfo =myHash->lookUpVoter(lookUpKey);
                    //handle not found case
                    if(lookUpInfo.getPtr()== nullptr){
                        cout<<"voter not exists"<<endl;
                    }
                    else{
                        voter * resultPtr = lookUpInfo.getPtr();
                        cout<<"RIN:"<<resultPtr->getRIN()<<endl;
                        cout<<"First Name: "<<resultPtr->getFirstName()<<endl;
                        cout<<"Last Name: "<<resultPtr->getLastName()<<endl;
                        cout<<"zip code: "<<resultPtr->getPosCode()<<endl;
                        cout<<"voted? "<<resultPtr->getFlag()<<endl;
                    }
                }

            }

            /*ACTION: DELETION
             * first delete the record in the posNodeList, then delete in hashtable.
             * */
            if(strcmp(action,ACTION_DELETE)==0){
                // get search key
                //malloc space
                char key[10] = "";
                //call helper function getParam
                char * paramPtr = Helper->findParam(readPtr);

                //call lookup() function
                int lookUpKey;
                lookUpKey = Helper->translateKey(paramPtr,readPtr,key,paramIndex);
                if(lookUpKey!=-1){
                    int result;

                    //first check if voter exists in the hash table
                    info lookUpInfo =myHash->lookUpVoter(lookUpKey);
                    if(lookUpInfo.getPtr()!= nullptr){
                        voter * voterPtr = lookUpInfo.getPtr();
                        // exists
                        //NOT VOTED YET, ONLY DELETE IN HASH TABLE

                        //VOTED, first delete in posNodeList
                        if(lookUpInfo.getFlag()=='Y'){
                            posCodeList * listPtr = myHash->getListPointer();
                            int posCode = voterPtr->getPosCode();
                            posNode * nodePtr = listPtr->findNode(posCode);
                            result = nodePtr->deleteVotedRecord(lookUpKey);

                        }
                        myHash->deleteVoter(lookUpKey);

                        if(result == 0){
                            cout<<"successfully deleted"<<endl;
                        }
                        else if(result == -1){
                            cout<<"voter does not exist"<<endl;
                        }
                    }
                    else{
                        cout<<"voter does not exist"<<endl;
                    }
                }
                else{
                    cout<<"error in your command"<<endl;
                }
            }

            /*ACTION: INSERT A USER
             *
             * */
            if(strcmp(action,ACTION_INSERT)==0){

                char ** param = (char**)malloc(sizeof(char)*12);
                int i = 0;
                int result;
                voter * newVoter = new voter();

                //copy params to tempBuffer from actionBuffer
                strncpy(tempBuffer,readPtr,100);

                //use strtok
                helperPtr = strtok(tempBuffer," ");
                while(helperPtr != NULL && i<4){
                    param[i] = helperPtr;
                    helperPtr = strtok(NULL," ");
                    i ++;
                }
                if(i<4){
                    cout<<"you did not provide enough info!"<<endl;

                }
                else{
                    newVoter->setUpVoter(param);
                    result = myHash ->insertVoter(newVoter);
                    if(result == -1){
                        cout<<"voter already exists!"<<endl;
                        delete(newVoter);
                    }
                    else if(result == 0){
                        cout<<"voter inserted!"<<endl;
                    }

                }
                free(param);
            }

            /*ACTION REGISTER SINGLE USER
             *
             * */
            if(strcmp(action,ACTION_REGISTER)==0){
                char * param[16];
                int i=0;

                int key;
                strncpy(tempBuffer,readPtr,sizeof(tempBuffer));

                //use strtok
                helperPtr = strtok(tempBuffer," ");
                while(helperPtr != nullptr && i<1){
                    param[i] = helperPtr;
                    helperPtr = strtok(NULL," ");
                    i ++;
                }
                key = atoi(param[0]);
                myHash->doVote(key);
            }
        }

        //on finishing this command cycle, free all the temporary buffer
//        free(tempBuffer);
        free(actionBuffer);
        free(action);
}

    // on exiting the program, free the hash table

    cout<<"releasing memory at location "<<&myHash<<endl;
    cout<<"bye!"<<endl;
    delete(myHash);
    return 0;


}

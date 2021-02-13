//
// Created by User on 2021/2/12.
//

#include <cstring>
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "hashTable.h"
using namespace std;
/*
 * return: 0 for command without a space; 1 for command with space
 * */
char* helper::parseCommand(char *bufferPtr) {
    //if there are spaces in the input
    char * spacePtr;

    spacePtr = strchr(bufferPtr,' ');
    if(spacePtr == nullptr){
        return nullptr;
    }
    else{
        return spacePtr;
    }
}

/*params: char* bufferPtr
 * return: pointer to the position of next space
 * */
char *helper::findParam(char *bufferPtr) {
    //find next space in the input
    char * spacePtr;
    spacePtr = strchr(bufferPtr+1,' ');
    if(spacePtr!= nullptr){
        return spacePtr;
    }
    else{
        return nullptr;
    }
}

int helper::translateKey(char *paramPtrArg, char * readPtrArg, char *keyArr, int paramIndex) {
    if(paramPtrArg!= nullptr){
        cout<<"having unknown params! "<<endl;
        return -1;
    }
    else{
        //promote readPtR 1 to the right
        readPtrArg ++;

        while(*readPtrArg!='\0'){
//            cout<<*readPtrArg;
            keyArr[paramIndex] = *readPtrArg;
            paramIndex++;
            readPtrArg++;

        }

//        cout<<"\nsearching for RIN:"<<keyArr<<endl;
        int result = atoi(keyArr);
        cout<<result<<endl;
        return result;
    }

    return 0;
}



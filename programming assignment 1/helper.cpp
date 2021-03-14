//
// Created by User on 2021/2/12.
//

#include <cstring>
#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*
 * function: parse command
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


/*
 * function: translateKey.
 * Description: translate a string represented integer into integer form
 * params: paramPtrArg: indicate the next space in the string; readPtr: pointer to the occrrence of first space
 * keyArr: buffer, used to hold key character; paramIndex: help us traverse through the parameter
 *
 * */
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
//        cout<<result<<endl;
        return result;
    }

    return 0;
}

/*
 * function getNumOfLine
 * Description: return the number of line of a given file
 * params: fileName: buffer of file name
 * return : int
 * */
int helper::getNumOfLine(char * fileName) {
    FILE * fp;
    fp = fopen(fileName,"r");
    int count=0;
    char c;
    if (fp == NULL)
    {
        printf("Could not open file %s", fileName);
        return 0;
    }

    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;

    // Close the file
    fclose(fp);
    printf("The file %s has %d lines\n ", fileName, count);

    return count;

}



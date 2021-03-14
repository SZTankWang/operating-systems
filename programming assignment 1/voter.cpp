//
// Created by User on 2021/2/2.
//

#include <cstdlib>
#include <iostream>
#include <cstring>
#include "voter.h"

using namespace std;
voter::voter() {
    firstName = (char*)malloc(16*sizeof(char));
    lastName = (char*)malloc(16*sizeof(char));
    flag = 'N';
    next = nullptr;
    RIN = 0;
    posCode = 0;
}

char * voter::getFirstName() {
    return firstName;
}

char * voter::getLastName() {
    return lastName;
}

int voter::getRIN() {
    return RIN;
}

int voter::getPosCode() {
    return posCode;
}

char voter::getFlag() {
    return flag;
}

void voter::setFirstName(char * newName) {
    strcpy(firstName,newName);
}

void voter::setLastName(char * newName) {
    strcpy(lastName,newName);
}


void voter::setRIN(int newRIN) {
    RIN = newRIN;
}

void voter::setPosCode(int newPosCode) {
    posCode = newPosCode;
}

void voter::setFlag(char newFlag) {
    flag = newFlag;
}

int voter::setNextVoter(voter *nextPtr) {
    next = nextPtr;
    return 0;
}

voter *voter::getNextVoter() {
    return next;
}

/*
 * function: setUpVoter
 * Description: on inserting a voter into the hashtable, first set up all the information of this new voter
 * */
int voter::setUpVoter(char **arr) {
    setRIN(atoi(arr[0]));
    setLastName(arr[1]);
    setFirstName(arr[2]);
    setPosCode(atoi(arr[3]));
    cout<<"set up voter with RIN: "<<arr[0]<<endl;
    return 0;
}


//
// Created by User on 2021/2/2.
//

#include "voter.h"

voter::voter(char firstName, char lastName, int RIN, int posCode) {
    firstName = firstName;
    lastName = lastName;
    RIN = RIN;
    posCode = posCode;
    flag = 'Y';
    next = nullptr;
}

char voter::getFirstName() {
    return firstName;
}

char voter::getLastName() {
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

void voter::setFirstName(char newName) {
    firstName = newName;
}

void voter::setLastName(char newName) {
    lastName = newName;
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

int voter::setNextVoter(voter *next) {
    next = next;
    return 0;
}

voter *voter::getNextVoter() {
    return next;
}




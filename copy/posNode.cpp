//
// Created by User on 2021/2/11.
//

#include "posNode.h"

#include <iostream>
#include "voter.h"

using namespace std;

posNode::posNode(voter *voterPtr) {
    *posCode = voterPtr->getPosCode();
    voterHead = voterPtr;
    nextPosNode = nullptr;
}

int posNode::getPosCode() {
    return *posCode;
}

int posNode::addVoter(voter *voterPtr) {
    voter * tempPtr = voterHead;
    while(tempPtr!= nullptr){
        if(tempPtr->getNextVoter() == nullptr){
            tempPtr->setNextVoter(voterPtr);
            return 0;
        }
        tempPtr = tempPtr->getNextVoter();

    }
    //error
    return -1;
}

int posNode::listVoter(){
    voter * tempPtr = voterHead;
    int voterRin;
    while(tempPtr!= nullptr){
        if(tempPtr->getFlag()=='Y'){
            voterRin = tempPtr->getRIN();
            cout<<"RIN:"<<voterRin;
        }
        tempPtr = tempPtr->getNextVoter();

    }
    return 0;
}

posNode *posNode::getNextPosNode() {
    return nextPosNode;
}

int posNode::getVotedNum() {
    voter * tempPtr = voterHead;
    int voterCount=0;
    while(tempPtr!= nullptr){
        if(tempPtr->getFlag()=='Y'){
            voterCount ++;

        }
        tempPtr = tempPtr->getNextVoter();

    }
    return voterCount;
}

int posNode::showVoterList() {
    int voterCount;
    voterCount = getVotedNum();
    cout<<"voted number of participants:"<<"/n";
    listVoter();
    return 0;
}

int posNode::setNextPosNode(posNode *posNodeArg) {
    nextPosNode = posNodeArg;
    return 0;
}

